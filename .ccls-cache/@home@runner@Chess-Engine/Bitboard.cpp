#include "Bitboard.hpp"

namespace JoyChess {
    Bitboard SQUARE[NUM_SQUARES];
    Bitboard RANK[NUM_SQUARES];
    Bitboard FILE[NUM_SQUARES];
    Bitboard DIAGONAL[NUM_SQUARES];
    Bitboard ANTIDIAGONAL[NUM_SQUARES];
    Bitboard KNIGHT_MASK[NUM_SQUARES];
    Bitboard ADJACENT_MASK[NUM_SQUARES];

    void InitBitboards() {
        for (int i = 0; i < NUM_SQUARES; i++) {
            SQUARE[i] = BIT(i);
        }

        for (int i = 0; i < NUM_SQUARES; i++) {
            for (int j = 0; j < NUM_FILES; j++) {
                RANK[i] |= BIT((i / NUM_RANKS) * NUM_FILES + j);
            }
        }
        
        for (int i = 0; i < NUM_SQUARES; i++) {
            for (int j = 0; j < NUM_RANKS; j++) {
                FILE[i] |= BIT(j * NUM_FILES + (i / NUM_RANKS));
            }
        }
        
        for (int i = 0; i < NUM_SQUARES; i++) {
            int row = Row(i);
            int col = Col(i);
            while (row > 0 && col < NUM_FILES - 1) {
                row--;
                col++;
            }
            while (row <= NUM_RANKS - 1 && col >= 0) {
                DIAGONAL[i] |= BIT(Ind(row, col));
                row++;
                col--;
            }
        }
        
        for (int i = 0; i < NUM_SQUARES; i++) {
            int row = Row(i);
            int col = Col(i);
            while (row > 0 && col > 0) {
                row--;
                col--;
            }
            while (row < NUM_RANKS && col < NUM_FILES) {
                ANTIDIAGONAL[i] |= BIT(Ind(row, col));
                row++;
                col++;
            }
        }

        
        
    }

    std::string BitboardToString(Bitboard b) {
        std::string out = "  +-----------------+\n";
        for (int i = 0; i < NUM_RANKS; i++) {
            out += std::to_string(NUM_RANKS - i);
            out += " | ";
            for (int j = 0; j < NUM_FILES; j++) {
                if(b & SQUARE[i * NUM_RANKS + j]) out += "1 ";
                else out += "0 ";
            }
            out += "|\n";
        }
        out += "  +-----------------+\n";
        out += "    a b c d e f g h\n";
        return out;
    }
}