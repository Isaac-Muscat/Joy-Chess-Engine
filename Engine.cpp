#include "Engine.hpp"

namespace JoyChess {

	std::vector<Move> GeneratePseudoLegalMoves(const Board &board) {
		Color friendColor = board.boardStateInfo->sideToMove;
		Color opponentColor = GetOpponentColor(friendColor);
		Bitboard friendOccupied = board.GetOccupied(board.boardStateInfo->sideToMove);
		Bitboard enemyOccupied = board.GetOccupied(opponentColor);
		Bitboard occupied = friendOccupied | enemyOccupied;

		std::vector<Move> moves;
		
		// Generate knight moves.
		Bitboard knights = board.pieceBB[friendColor][Knight];
		while (knights) {
			int from = bitScanForward(knights);
			Bitboard knightMoves = KNIGHT_MASK[from] & (~friendOccupied);
			Bitboard quietMoves = knightMoves & (~enemyOccupied);
			Bitboard captures = knightMoves & enemyOccupied;
			BitboardToMoves(moves, captures, from, Capture);
			BitboardToMoves(moves, quietMoves, from, QuietMove);
			knights &= ~SQUARE[from];
		}

		// Generate king moves.
		Bitboard king = board.pieceBB[friendColor][King];
		{
			int from = bitScanForward(king);
			Bitboard kingMoves = ADJACENT_MASK[from] & (~friendOccupied);
			Bitboard quietMoves = kingMoves & (~enemyOccupied);
			Bitboard captures = kingMoves & enemyOccupied;
			BitboardToMoves(moves, captures, from, Capture);
			BitboardToMoves(moves, quietMoves, from, QuietMove);

			// Castling
			switch (friendColor) {
				case White:
				{
					if (board.boardStateInfo->castleRights == NoCastleRights) {
						break;
					}
					else if (board.boardStateInfo->castleRights & CastleRights::WKingCastle && !(CASTLE_MASK[White][CASTLE_EAST] & ~occupied)) {
						moves.push_back({from, g1, KingCastle});
					}
					else if (board.boardStateInfo->castleRights & CastleRights::WQueenCastle && !(CASTLE_MASK[White][CASTLE_WEST] & ~occupied)) {
						moves.push_back({from, c1, QueenCastle});
					}
					break;
				}
				case Black:
				{
					if (board.boardStateInfo->castleRights == NoCastleRights) {
						break;
					}
					else if (board.boardStateInfo->castleRights & CastleRights::BKingCastle && !(CASTLE_MASK[Black][CASTLE_EAST] & occupied)) {
						moves.push_back({from, g8, KingCastle});
					}
					else if (board.boardStateInfo->castleRights & CastleRights::BQueenCastle && !(CASTLE_MASK[Black][CASTLE_WEST] & occupied)) {
						moves.push_back({from, c8, QueenCastle});
					}
					break;
				}
			}
		}

		// Generate pawn moves.
		Bitboard pawns = board.pieceBB[friendColor][Pawn];
		int pawnDirection = GetPawnDirection(friendColor);
		while (pawns) {
			int from = bitScanForward(pawns);
			int to = bitScanForward(PAWN_PUSH[friendColor][from]);

			// Pawn pushes.
			if (PAWN_PUSH[friendColor][from] && board.squares[to] == None) {

				// Pawn push promotion
				if (Rank(from) == PAWN_PROMO_FROM_RANK[friendColor]) {
					moves.push_back({from, to, KnightPromotion});
					moves.push_back({from, to, BishopPromotion});
					moves.push_back({from, to, RookPromotion});
					moves.push_back({from, to, QueenPromotion});

				// Pawn push.
				} else {
					moves.push_back({from, to, QuietMove});
				}

				// Double pawn push.
				if (Rank(from) == PAWN_START_RANK[friendColor] && board.squares[to + pawnDirection] == None) {
					moves.push_back({from, to, DoublePawnPush});
				}
			}

			// Pawn captures.
			Bitboard pawnCaptures = PAWN_ATTACKS[friendColor][from] & enemyOccupied;
			while (pawnCaptures) {
				to = bitScanForward(pawnCaptures);
				
				// Pawn Promotion
				if (Rank(from) == PAWN_PROMO_FROM_RANK[friendColor]) {
					moves.push_back({from, to, KnightPromoCapture});
					moves.push_back({from, to, BishopPromoCapture});
					moves.push_back({from, to, RookPromoCapture});
					moves.push_back({from, to, QueenPromoCapture});
				} else {
					moves.push_back({from, to, Capture});
				}

				pawnCaptures &= ~SQUARE[to];
			}

			// En Passant.
			if (board.boardStateInfo->enPassantTarget != NONE && PAWN_ATTACKS[friendColor][from] & SQUARE[board.boardStateInfo->enPassantTarget]) {
				to = board.boardStateInfo->enPassantTarget;
				moves.push_back({from, to, EPCapture});
			}

			pawns &= ~SQUARE[from];
		}

		// Generate bishop moves.
		Bitboard bishops = board.pieceBB[friendColor][Bishop];
		while (bishops) {
			int from = bitScanForward(bishops);
			Bitboard bishopMoves = 0;

			bishopMoves |= GetNegativeDirectionAttacks(occupied, SOUTH_WEST, from)
				| GetPositiveDirectionAttacks(occupied, NORTH_WEST, from)
				| GetPositiveDirectionAttacks(occupied, NORTH_EAST, from)
				| GetNegativeDirectionAttacks(occupied, SOUTH_EAST, from);
			Bitboard captures = bishopMoves & enemyOccupied;
			Bitboard quietMoves = bishopMoves & ~occupied;
			BitboardToMoves(moves, captures, from, Capture);
			BitboardToMoves(moves, quietMoves, from, QuietMove);
			
			bishops &= ~SQUARE[from];
		}

		// Generate rook moves.
		Bitboard rooks = board.pieceBB[friendColor][Rook];
		while (rooks) {
			int from = bitScanForward(rooks);
			Bitboard rookMoves = 0;

			rookMoves |= GetNegativeDirectionAttacks(occupied, WEST, from)
				| GetPositiveDirectionAttacks(occupied, NORTH, from)
				| GetPositiveDirectionAttacks(occupied, EAST, from)
				| GetNegativeDirectionAttacks(occupied, SOUTH, from);
			Bitboard captures = rookMoves & enemyOccupied;
			Bitboard quietMoves = rookMoves & ~occupied;
			BitboardToMoves(moves, captures, from, Capture);
			BitboardToMoves(moves, quietMoves, from, QuietMove);
			
			rooks &= ~SQUARE[from];
		}

		// Generate queen moves.
		Bitboard queens = board.pieceBB[friendColor][Queen];
		while (queens) {
			int from = bitScanForward(queens);
			Bitboard queenMoves = 0;

			queenMoves |= GetNegativeDirectionAttacks(occupied, WEST, from)
				| GetPositiveDirectionAttacks(occupied, NORTH, from)
				| GetPositiveDirectionAttacks(occupied, EAST, from)
				| GetNegativeDirectionAttacks(occupied, SOUTH, from)
				| GetNegativeDirectionAttacks(occupied, SOUTH_WEST, from)
				| GetPositiveDirectionAttacks(occupied, NORTH_WEST, from)
				| GetPositiveDirectionAttacks(occupied, NORTH_EAST, from)
				| GetNegativeDirectionAttacks(occupied, SOUTH_EAST, from);

			Bitboard captures = queenMoves & enemyOccupied;
			Bitboard quietMoves = queenMoves & ~occupied;
			BitboardToMoves(moves, captures, from, Capture);
			BitboardToMoves(moves, quietMoves, from, QuietMove);
			
			queens &= ~SQUARE[from];
		}

		return moves;

	}

	/**
	 * @brief Makes a move on the board. Modifies boardStateInfo.
	 * Assumes the move is valid before executing
	 * @param Board to change
	 * @param Move to execute.
	 * 
	 */
	void make(Board& board, const Move& move) {
		// Convert Piece to Color and GenericPiece
		Piece piece = board.squares[move.from];
		int color = piece / NUM_GENERIC_PIECE_TYPES;
		int genericPiece = piece % NUM_GENERIC_PIECE_TYPES;

		// Deal with castling rights on king move.
		if (piece == WKing) {
			board.boardStateInfo->castleRights &= ~WKingCastle & ~WQueenCastle;
		} else if (piece == BKing) {
			board.boardStateInfo->castleRights &= ~BKingCastle & ~BQueenCastle;
		}

		// Castle rights for each rook.
		if ((piece == WRook && move.from == a1) || (move.to == a1 && board.boardStateInfo->capturedPiece == WRook)) {
			board.boardStateInfo->castleRights &= ~WQueenCastle;
		} else if ((piece == WRook && move.from == h1) || (move.to == h1 && board.boardStateInfo->capturedPiece == WRook)) {
			board.boardStateInfo->castleRights &= ~WKingCastle;
		} else if ((piece == BRook && move.from == a8) || (move.to == a8 && board.boardStateInfo->capturedPiece == BRook)) {
			board.boardStateInfo->castleRights &= ~BQueenCastle;
		} else if ((piece == BRook && move.from == h8) || (move.to == h8 && board.boardStateInfo->capturedPiece == BRook)) {
			board.boardStateInfo->castleRights &= ~BKingCastle;
		}

		// Update move clock
		board.boardStateInfo->fullMoveCounter++;
		
		switch (move.moveType) {
			case QuietMove:
			{
				board.pieceBB[color][genericPiece] &= ~SQUARE[move.from];
				board.pieceBB[color][genericPiece] |= SQUARE[move.to];
				board.squares[move.from] = None;
				board.squares[move.to] = piece;

				board.boardStateInfo->sideToMove = GetOpponentColor(board.boardStateInfo->sideToMove);
				board.boardStateInfo->capturedPiece = None;
				board.boardStateInfo->enPassantTarget = Square::NONE;
				board.boardStateInfo->halfMoveClock++;
				
				break;
			}
			case DoublePawnPush:
			{
				board.pieceBB[color][genericPiece] &= ~SQUARE[move.from];
				board.pieceBB[color][genericPiece] |= SQUARE[move.to];
				board.squares[move.from] = None;
				board.squares[move.to] = piece;

				board.boardStateInfo->sideToMove = GetOpponentColor(board.boardStateInfo->sideToMove);
				board.boardStateInfo->capturedPiece = None;
				board.boardStateInfo->enPassantTarget = static_cast<Square>(bitScanForward(FILE[move.from] & PAWN_EP_CAPTURE_TO_RANK[GetOpponentColor(color)]));
				board.boardStateInfo->halfMoveClock++;

				break;
			}
			case Capture:
			{
				board.boardStateInfo->capturedPiece = board.squares[move.to];
				board.squares[move.from] = None;
				board.squares[move.to] = piece;
				board.pieceBB[color][genericPiece] &= ~SQUARE[move.from];
				board.pieceBB[color][genericPiece] |= SQUARE[move.to];
				board.pieceBB[GetOpponentColor(board.boardStateInfo->sideToMove)][board.boardStateInfo->capturedPiece % NUM_GENERIC_PIECE_TYPES] &= ~SQUARE[move.to];

				board.boardStateInfo->sideToMove = GetOpponentColor(board.boardStateInfo->sideToMove);
				board.boardStateInfo->capturedPiece = None;
				board.boardStateInfo->enPassantTarget = Square::NONE;
				board.boardStateInfo->halfMoveClock++;
				
				break;
			}
			case KingCastle:
			{
				board.pieceBB[color][genericPiece] &= ~SQUARE[move.from];
				board.pieceBB[color][genericPiece] |= SQUARE[move.to];
				board.squares[move.from] = None;
				board.squares[move.to] = piece;
				board.boardStateInfo->sideToMove = GetOpponentColor(board.boardStateInfo->sideToMove);
				board.boardStateInfo->capturedPiece = None;
				board.boardStateInfo->enPassantTarget = Square::NONE;
				board.boardStateInfo->halfMoveClock++;
				if (color == White) {
					board.pieceBB[color][Rook] &= ~SQUARE[h1];
					board.pieceBB[color][Rook] |= SQUARE[f1];
					board.squares[h1] = None;
					board.squares[f1] = WRook;
				} else {
					board.pieceBB[GetOpponentColor(board.boardStateInfo->sideToMove)][Rook] &= ~SQUARE[h8];
					board.pieceBB[GetOpponentColor(board.boardStateInfo->sideToMove)][Rook] |= SQUARE[f8];
					board.squares[h8] = None;
					board.squares[f8] = BRook;
				}
				break;
			}
			case QueenCastle:
			{
				board.pieceBB[color][genericPiece] &= ~SQUARE[move.from];
				board.pieceBB[color][genericPiece] |= SQUARE[move.to];
				board.squares[move.from] = None;
				board.squares[move.to] = piece;
				board.boardStateInfo->sideToMove = GetOpponentColor(board.boardStateInfo->sideToMove);
				board.boardStateInfo->capturedPiece = None;
				board.boardStateInfo->enPassantTarget = Square::NONE;
				board.boardStateInfo->halfMoveClock++;
				if (color == White) {
					board.pieceBB[color][Rook] &= ~SQUARE[a1];
					board.pieceBB[color][Rook] |= SQUARE[d1];
					board.squares[a1] = None;
					board.squares[d1] = WRook;
				} else {
					board.pieceBB[GetOpponentColor(board.boardStateInfo->sideToMove)][Rook] &= ~SQUARE[a8];
					board.pieceBB[GetOpponentColor(board.boardStateInfo->sideToMove)][Rook] |= SQUARE[d8];
					board.squares[a8] = None;
					board.squares[d8] = BRook;
				}
				break;
			}
			case EPCapture:
			{
				board.boardStateInfo->capturedPiece = board.squares[move.to];
				board.squares[move.from] = None;
				board.squares[move.to] = piece;
				board.pieceBB[color][genericPiece] &= ~SQUARE[move.from];
				board.pieceBB[color][genericPiece] |= SQUARE[move.to];
				board.pieceBB[GetOpponentColor(board.boardStateInfo->sideToMove)][board.boardStateInfo->capturedPiece % NUM_GENERIC_PIECE_TYPES] &= ~SQUARE[move.to];
				
				board.boardStateInfo->sideToMove = GetOpponentColor(board.boardStateInfo->sideToMove);
				board.boardStateInfo->capturedPiece = None;
				board.boardStateInfo->enPassantTarget = Square::NONE;
				board.boardStateInfo->halfMoveClock++;
				break;
			}
			case KnightPromotion:
			{
				board.pieceBB[color][genericPiece] &= ~SQUARE[move.from];
				board.pieceBB[color][Knight] |= SQUARE[move.to];
				board.squares[move.from] = None;
				board.squares[move.to] = GenericPieceToPiece(Knight, color);

				board.boardStateInfo->sideToMove = GetOpponentColor(board.boardStateInfo->sideToMove);
				board.boardStateInfo->capturedPiece = None;
				board.boardStateInfo->enPassantTarget = Square::NONE;
				board.boardStateInfo->halfMoveClock++;
				
				break;
			}
			case BishopPromotion:
			case RookPromotion:
			case QueenPromotion:

        	case KnightPromoCapture:
			case BishopPromoCapture:
			case RookPromoCapture:
			case QueenPromoCapture:
		}

		
	}

	void BitboardToMoves(std::vector<Move>& moves, Bitboard moveBB, int from, MoveType moveType) {
		while (moveBB) {
				int to = bitScanForward(moveBB);
				moves.push_back({from, to, moveType});
				moveBB &= ~SQUARE[to];
		}
	}

	Bitboard GetPositiveDirectionAttacks(const Bitboard& occupied, Direction dir, int square) {
		Bitboard attacks = DIRECTIONS[dir][square];
		Bitboard blocker = attacks & occupied;
		if ( blocker ) {
			square = bitScanForward(blocker);
			attacks ^= DIRECTIONS[dir][square];
		}
		return attacks;
	}

	Bitboard GetNegativeDirectionAttacks(const Bitboard& occupied, Direction dir, int square) {
		Bitboard attacks = DIRECTIONS[dir][square];
		Bitboard blocker = attacks & occupied;
		if ( blocker ) {
			square = bitScanReverse(blocker);
			attacks ^= DIRECTIONS[dir][square];
		}
		return attacks;
	}
}