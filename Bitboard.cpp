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
    Bitboard DIRECTIONS[NUM_DIRECTIONS][NUM_SQUARES]{0};
    Bitboard CASTLE_MASK[NUM_COLORS][NUM_CASTLING_DIRECTIONS]{0};

    int PAWN_START_RANK[NUM_COLORS]{0};
    int PAWN_PROMO_FROM_RANK[NUM_COLORS]{0};
    int PAWN_EP_CAPTURE_TO_RANK[NUM_COLORS]{0};

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
                PAWN_PUSH[White][i] |= SQUARE[i + PAWN_NORTH];
            if (File(i) != A_FILE && Rank(i) != RANK_8)
                PAWN_ATTACKS[White][i] |= SQUARE[i + PAWN_NORTH_WEST];
            if (File(i) != H_FILE && Rank(i) != RANK_8)
                PAWN_ATTACKS[White][i] |= SQUARE[i + PAWN_NORTH_EAST];

            if (Rank(i) != RANK_1)
                PAWN_PUSH[Black][i] |= SQUARE[i + PAWN_SOUTH];
            if (File(i) != A_FILE && Rank(i) != RANK_1)
                PAWN_ATTACKS[Black][i] |= SQUARE[i + PAWN_SOUTH_WEST];
            if (File(i) != H_FILE && Rank(i) != RANK_1)
                PAWN_ATTACKS[Black][i] |= SQUARE[i + PAWN_SOUTH_EAST];
        }

        // Generate direction masks
        for (int i = 0; i < NUM_SQUARES; i++) {
            int rank {0};
            int file {0};

            // North
            rank = Rank(i);
            file = File(i);
            while (rank <= RANK_8 && rank >= RANK_1 && file <= H_FILE && file >= A_FILE) {
                DIRECTIONS[NORTH][i] |= SQUARE[Ind(rank, file)];
                rank++;
            }
            DIRECTIONS[NORTH][i] &= ~SQUARE[i];
            
            // Northeast
            rank = Rank(i);
            file = File(i);
            while (rank <= RANK_8 && rank >= RANK_1 && file <= H_FILE && file >= A_FILE) {
                DIRECTIONS[NORTH_EAST][i] |= SQUARE[Ind(rank, file)];
                rank++;
                file++;
            }
            DIRECTIONS[NORTH_EAST][i] &= ~SQUARE[i];

            // East
            rank = Rank(i);
            file = File(i);
            while (rank <= RANK_8 && rank >= RANK_1 && file <= H_FILE && file >= A_FILE) {
                DIRECTIONS[EAST][i] |= SQUARE[Ind(rank, file)];
                file++;
            }
            DIRECTIONS[EAST][i] &= ~SQUARE[i];

            // Southeast
            rank = Rank(i);
            file = File(i);
            while (rank <= RANK_8 && rank >= RANK_1 && file <= H_FILE && file >= A_FILE) {
                DIRECTIONS[SOUTH_EAST][i] |= SQUARE[Ind(rank, file)];
                rank--;
                file++;
            }
            DIRECTIONS[SOUTH_EAST][i] &= ~SQUARE[i];

            // South
            rank = Rank(i);
            file = File(i);
            while (rank <= RANK_8 && rank >= RANK_1 && file <= H_FILE && file >= A_FILE) {
                DIRECTIONS[SOUTH][i] |= SQUARE[Ind(rank, file)];
                rank--;
            }
            DIRECTIONS[SOUTH][i] &= ~SQUARE[i];

            // Southwest
            rank = Rank(i);
            file = File(i);
            while (rank <= RANK_8 && rank >= RANK_1 && file <= H_FILE && file >= A_FILE) {
                DIRECTIONS[SOUTH_WEST][i] |= SQUARE[Ind(rank, file)];
                rank--;
                file--;
            }
            DIRECTIONS[SOUTH_WEST][i] &= ~SQUARE[i];

            // West
            rank = Rank(i);
            file = File(i);
            while (rank <= RANK_8 && rank >= RANK_1 && file <= H_FILE && file >= A_FILE) {
                DIRECTIONS[WEST][i] |= SQUARE[Ind(rank, file)];
                file--;
            }
            DIRECTIONS[WEST][i] &= ~SQUARE[i];

            // Northwest
            rank = Rank(i);
            file = File(i);
            while (rank <= RANK_8 && rank >= RANK_1 && file <= H_FILE && file >= A_FILE) {
                DIRECTIONS[NORTH_WEST][i] |= SQUARE[Ind(rank, file)];
                rank++;
                file--;
            }
            DIRECTIONS[NORTH_WEST][i] &= ~SQUARE[i];
        }

        CASTLE_MASK[White][CASTLE_WEST] = SQUARE[b1] | SQUARE[c1] | SQUARE[d1];
        CASTLE_MASK[White][CASTLE_EAST] = SQUARE[f1] | SQUARE[g1];
        CASTLE_MASK[Black][CASTLE_WEST] = SQUARE[b8] | SQUARE[c8] | SQUARE[d8];
        CASTLE_MASK[Black][CASTLE_EAST] = SQUARE[f8] | SQUARE[g8];

        PAWN_START_RANK[White] = RANK_2;
        PAWN_START_RANK[Black] = RANK_7;
        PAWN_PROMO_FROM_RANK[White] = RANK_7;
        PAWN_PROMO_FROM_RANK[Black] = RANK_2;
        PAWN_EP_CAPTURE_TO_RANK[White] = RANK_6;
        PAWN_EP_CAPTURE_TO_RANK[Black] = RANK_3;
    }

    // Used in bitscan routines
    const int index64[64] = {
        0, 47,  1, 56, 48, 27,  2, 60,
        57, 49, 41, 37, 28, 16,  3, 61,
        54, 58, 35, 52, 50, 42, 21, 44,
        38, 32, 29, 23, 17, 11,  4, 62,
        46, 55, 26, 59, 40, 36, 15, 53,
        34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30,  9, 24,
        13, 18,  8, 12,  7,  6,  5, 63
    };

    /**
     * bitScanForward
     * @author Kim Walisch (2012)
     * @param bb bitboard to scan
     * @precondition bb != 0
     * @return index (0..63) of least significant one bit
     */
    int bitScanForward(Bitboard bb) {
        const Bitboard debruijn64 = int64_t(0x03f79d71b4cb0a89);
        return index64[((bb ^ (bb-1)) * debruijn64) >> 58];
    }

    /**
     * bitScanReverse
     * @authors Kim Walisch, Mark Dickinson
     * @param bb bitboard to scan
     * @precondition bb != 0
     * @return index (0..63) of most significant one bit
     */
    int bitScanReverse(Bitboard bb) {
        const Bitboard debruijn64 = int64_t(0x03f79d71b4cb0a89);
        bb |= bb >> 1; 
        bb |= bb >> 2;
        bb |= bb >> 4;
        bb |= bb >> 8;
        bb |= bb >> 16;
        bb |= bb >> 32;
        return index64[(bb * debruijn64) >> 58];
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