
#include "Utilities.hpp"
#include "Bitboard.hpp"
#include "Board.hpp"
#include "Engine.hpp"

using namespace JoyChess;

int main() {

    InitBitboards();

    Board board { FEN[0] };
    std::cout << board.ToString();
    GeneratePseudoLegalMoves(board);
    std::cout << "Program finished!\n";
}