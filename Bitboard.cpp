#include "Bitboard.hpp"

namespace JoyChess {
    Bitboard SQUARE[NUM_SQUARES]{0};
    Bitboard RANK[NUM_SQUARES]{0};
    Bitboard FILE[NUM_SQUARES]{0};
    Bitboard DIAGONAL[NUM_SQUARES]{0};
    Bitboard ANTIDIAGONAL[NUM_SQUARES]{0};
    Bitboard KNIGHT_MASK[NUM_SQUARES]{0};
    Bitboard ADJACENT_MASK[NUM_SQUARES]{0};

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
            int row = Rank(i);
            int col = File(i);
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
            int row = Rank(i);
            int col = File(i);
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
        
        for (int i = 0; i < NUM_SQUARES; i++) {
            int row = Rank(i);
            int col = File(i);
            if (row > 1 && col > 0) KNIGHT_MASK[i] |= BIT(i - 17);
            if (row > 1 && col < 7) KNIGHT_MASK[i] |= BIT(i - 15);
            if (row > 0 && col > 1) KNIGHT_MASK[i] |= BIT(i - 10);
            if (row > 0 && col < 6) KNIGHT_MASK[i] |= BIT(i - 6);
            if (row < 7 && col > 1) KNIGHT_MASK[i] |= BIT(i + 6);
            if (row < 7 && col < 6) KNIGHT_MASK[i] |= BIT(i + 10);
            if (row < 6 && col > 0) KNIGHT_MASK[i] |= BIT(i + 15);
            if (row < 6 && col < 7) KNIGHT_MASK[i] |= BIT(i + 17);
        }
        
        for (int i = 0; i < NUM_SQUARES; i++) {
            int row = Rank(i);
            int col = File(i);
            if (row > 0 && col > 0) ADJACENT_MASK[i] |= BIT(i - 9);
            if (row > 0)            ADJACENT_MASK[i] |= BIT(i - 8);
            if (row > 0 && col < 7) ADJACENT_MASK[i] |= BIT(i - 7);
            if (col > 0)            ADJACENT_MASK[i] |= BIT(i - 1);
            if (col < 7)            ADJACENT_MASK[i] |= BIT(i + 1);
            if (row < 7 && col > 0) ADJACENT_MASK[i] |= BIT(i + 7);
            if (row < 7)            ADJACENT_MASK[i] |= BIT(i + 8);
            if (row < 7 && col < 7) ADJACENT_MASK[i] |= BIT(i + 9);
        }

        
        
    }

    std::string BitboardToString(Bitboard b) {
        std::string out = "  +-----------------+\n";
        for (int i = NUM_RANKS - 1; i >= 0; i--) {
            out += std::to_string(i + 1);
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