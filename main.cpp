
#include "ChessEngine.hpp"

int main() {
    JoyChess::InitBitboards();
    for (int i = 0; i < JoyChess::NUM_SQUARES; i++)
        std::cout << JoyChess::BitboardToString(JoyChess::ANTIDIAGONAL[i]);
    std::cout << "Program finished!\n";
}