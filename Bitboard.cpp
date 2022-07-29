#include "Utilities.hpp"
#include "Bitboard.hpp"

namespace JoyChess {

    Bitboard SQUARE[NUM_SQUARES]{0};
    Bitboard RANK[NUM_SQUARES]{0};
    Bitboard FILE[NUM_SQUARES]{0};
    Bitboard DIAGONAL[NUM_SQUARES]{0};
    Bitboard ANTIDIAGONAL[NUM_SQUARES]{0};
    Bitboard KNIGHT_MASK[NUM_SQUARES]{0};
    Bitboard ADJACENT_MASK[NUM_SQUARES]{0};
    Bitboard PAWN_PUSH[NUM_COLORS][NUM_SQUARES]{0};
    Bitboard PAWN_ATTACKS[NUM_COLORS][NUM_SQUARES]{0};

    int PAWN_START_RANK[NUM_COLORS]{0};
    int PAWN_PROMO_FROM_RANK[NUM_COLORS]{0};
    int PAWN_EP_CAPTURE_FROM_RANK[NUM_COLORS]{0};

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

        for (int i = 0; i < NUM_SQUARES; i++) {
            if (Rank(i) != RANK_8)
                PAWN_PUSH[White][i] |= SQUARE[i + NORTH];
            if (File(i) != A_FILE && Rank(i) != RANK_8)
                PAWN_ATTACKS[White][i] |= SQUARE[i + NORTH_WEST];
            if (File(i) != H_FILE && Rank(i) != RANK_8)
                PAWN_ATTACKS[White][i] |= SQUARE[i + NORTH_EAST];

            if (Rank(i) != RANK_1)
                PAWN_PUSH[Black][i] |= SQUARE[i + SOUTH];
            if (File(i) != A_FILE && Rank(i) != RANK_1)
                PAWN_ATTACKS[Black][i] |= SQUARE[i + SOUTH_WEST];
            if (File(i) != H_FILE && Rank(i) != RANK_1)
                PAWN_ATTACKS[Black][i] |= SQUARE[i + SOUTH_EAST];
        }

        PAWN_START_RANK[White] = RANK_2;
        PAWN_START_RANK[Black] = RANK_7;
        PAWN_PROMO_FROM_RANK[White] = RANK_7;
        PAWN_PROMO_FROM_RANK[Black] = RANK_2;
        PAWN_EP_CAPTURE_FROM_RANK[White] = RANK_5;
        PAWN_EP_CAPTURE_FROM_RANK[Black] = RANK_4;
    }

    /**
     * bitScanForward
     * @author Kim Walisch (2012)
     * @param bb bitboard to scan
     * @precondition bb != 0
     * @return index (0..63) of least significant one bit
     */
    int bitScanForward(Bitboard bb) {
        static const int index64[64] = {
            0, 47,  1, 56, 48, 27,  2, 60,
            57, 49, 41, 37, 28, 16,  3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11,  4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30,  9, 24,
            13, 18,  8, 12,  7,  6,  5, 63
        };
        const Bitboard debruijn64 = int64_t(0x03f79d71b4cb0a89);
        return index64[((bb ^ (bb-1)) * debruijn64) >> 58];
    }

    std::string BitboardToString(Bitboard b) {
        std::string out = "  +-----------------+\n";
        for (int i = NUM_RANKS - 1; i >= 0; i--) {
            out += std::to_string(i + 1);
            out += " | ";
            for (int j = 0; j < NUM_FILES; j++) {
                if (b & SQUARE[i * NUM_RANKS + j]) out += "1 ";
                else out += "0 ";
            }
            out += "|\n";
        }
        out += "  +-----------------+\n";
        out += "    a b c d e f g h\n";
        return out;
    }
}