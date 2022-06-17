
#include "ChessEngine.hpp"

int main() {
    JoyChess::InitBitboards();

    JoyChess::Board board { JoyChess::FEN_TEST1 };
    std::cout << board.ToString();
    std::cout << "Program finished!\n";
}