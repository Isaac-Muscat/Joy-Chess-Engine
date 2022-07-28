#pragma once

#include "Utilities.hpp"
#include "Board.hpp"

#include <vector>

namespace JoyChess {

	Move GetBestMove(const Board &board);

	std::vector<Move> GeneratePseudoLegalMoves(const Board &board);

}