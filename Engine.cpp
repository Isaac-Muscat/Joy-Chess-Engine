#include "Engine.hpp"

namespace JoyChess {

	std::vector<Move> GeneratePseudoLegalMoves(const Board &board) {
		Color friendColor = board.boardStateInfo->sideToMove;
		Color opponentColor = GetOpponentColor(friendColor);
		Bitboard friendOccupied = board.GetOccupied(board.boardStateInfo->sideToMove);
		Bitboard enemyOccupied = board.GetOccupied(opponentColor);

		std::vector<Move> moves;
		
		// Generate knight moves.
		Bitboard knights = board.pieceBB[friendColor][Knight];
		while (knights) {
			int from = bitScanForward(knights);
			Bitboard knightMoves = KNIGHT_MASK[from] & (~friendOccupied);
			Bitboard quietMoves = knightMoves & (~enemyOccupied);
			Bitboard captures = knightMoves & enemyOccupied;
			while (quietMoves) {
				int to = bitScanForward(quietMoves);
				moves.push_back({from, to, QuietMove});
				quietMoves &= ~SQUARE[to];
			}
			while (captures) {
				int to = bitScanForward(captures);
				moves.push_back({from, to, Capture});
				captures &= ~SQUARE[to];
			}
			knights &= ~SQUARE[from];
		}

		// Generate king moves.
		Bitboard king = board.pieceBB[friendColor][King];
		{
			int from = bitScanForward(king);
			Bitboard kingMoves = ADJACENT_MASK[from] & (~friendOccupied);
			Bitboard quietMoves = kingMoves & (~enemyOccupied);
			Bitboard captures = kingMoves & enemyOccupied;
			while (quietMoves) {
				int to = bitScanForward(quietMoves);
				moves.push_back({from, to, QuietMove});
				quietMoves &= ~SQUARE[to];
			}
			while (captures) {
				int to = bitScanForward(captures);
				moves.push_back({from, to, Capture});
				captures &= ~SQUARE[to];
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
			if (board.boardStateInfo->enPassantTarget != NONE && (from) == PAWN_EP_CAPTURE_FROM_RANK[friendColor] && abs(File(board.boardStateInfo->enPassantTarget) - File(from))) {
				to = bitScanForward(PAWN_ATTACKS[friendColor][from] & FILE[board.boardStateInfo->enPassantTarget]);
				moves.push_back({from, to, EPCapture});
			}

			pawns &= ~SQUARE[from];
		}

		// Generate bishop moves.
		// Generate rook moves.
		// Generate queen moves.

		return moves;

	}
}