#pragma once

#include <cstdint>
#include <array>
#include <string>

/*
    56 57 58 59 60 61 62 63
    48 49 50 51 52 53 54 55
    40 41 42 43 44 45 46 47
    32 33 34 35 36 37 38 39
    24 25 26 27 28 29 30 31
    16 17 18 19 20 21 22 23
    08 09 10 11 12 13 14 15
    00 01 02 03 04 05 06 07
*/

#define BIT(x) (((uint64_t)1) << (x))

namespace JoyChess {
    typedef uint64_t Bitboard;

    constexpr int NUM_FILES = 8;
    constexpr int NUM_RANKS = 8;
    constexpr int NUM_SQUARES = 64;

    enum File {
        A_FILE = 0,
        B_FILE,
        C_FILE,
        D_FILE,
        E_FILE,
        F_FILE,
        G_FILE,
        H_FILE
    };

    enum Rank {
        RANK_1 = 0,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8
    };

    extern Bitboard SQUARE[NUM_SQUARES];
    extern Bitboard RANK[NUM_SQUARES];
    extern Bitboard FILE[NUM_SQUARES];
    extern Bitboard DIAGONAL[NUM_SQUARES];
    extern Bitboard ANTIDIAGONAL[NUM_SQUARES];
    extern Bitboard KNIGHT_MASK[NUM_SQUARES];
    extern Bitboard ADJACENT_MASK[NUM_SQUARES];

    void InitBitboards();
    std::string BitboardToString(Bitboard b);
    inline int Rank(int i) { return i / NUM_FILES; }
    inline int File(int i) { return i % NUM_RANKS; }
    inline int Ind(int rank, int file) { return (rank) * NUM_FILES + file; }

}