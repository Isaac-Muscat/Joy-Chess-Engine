#include "Engine.hpp"

namespace JoyChess {

	std::vector<Move> GeneratePseudoLegalMoves(const Board &board) {
		Color friendColor = board.boardStateInfo->sideToMove;
		Color opponentColor = GetOpponentColor(friendColor);
		Bitboard friendOccupied = board.GetOccupied(board.boardStateInfo->sideToMove);
		Bitboard enemyOccupied = board.GetOccupied(opponentColor);

		std::vector<Move> moves;
		
		// Generate knight moves.
		Bitboard knights = board.pieceBB[Knight + friendColor];
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
		Bitboard king = board.pieceBB[King + friendColor];
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
		Bitboard pawns = board.pieceBB[Pawn + friendColor];
		while (pawns) {
			int from = bitScanForward(pawns);
			int to = from + GetPawnDirection(friendColor);
			if (board.squares[to] == None && ) moves.push_back({from, to, QuietMove})
			
			pawns &= ~SQUARE[from];
		}

		// Generate bishop moves.
		// Generate rook moves.
		// Generate queen moves.

		return moves;

	}
}