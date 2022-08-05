#pragma once

#include "Utilities.hpp"
#include "Board.hpp"

#include <vector>

namespace JoyChess {

	Move GetBestMove(const Board &board);

	std::vector<Move> GeneratePseudoLegalMoves(const Board &board);
	std::vector<Move> GenerateLegalMoves(const Board &board);

	Bitboard GetPositiveDirectionAttacks(const Bitboard& occupied, Direction dir, int square);
	Bitboard GetNegativeDirectionAttacks(const Bitboard& occupied, Direction dir, int square);


}