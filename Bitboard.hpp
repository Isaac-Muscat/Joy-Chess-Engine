#pragma once

#include <cstdint>
#include <array>
#include <string>

/*
    00 01 02 03 04 05 06 07
    08 09 10 11 12 13 14 15
    08 09 10 11 12 13 14 15
    08 09 10 11 12 13 14 15
    08 09 10 11 12 13 14 15
    08 09 10 11 12 13 14 15
    08 09 10 11 12 13 14 15
    08 09 10 11 12 13 62 63
*/

#define BIT(x) (((uint64_t)1) << (x))

namespace JoyChess {
    typedef uint64_t Bitboard;

    constexpr int NUM_FILES = 8;
    constexpr int NUM_RANKS = 8;
    constexpr int NUM_SQUARES = 64;

    extern Bitboard SQUARE[NUM_SQUARES];
    extern Bitboard RANK[NUM_SQUARES];
    extern Bitboard FILE[NUM_SQUARES];
    extern Bitboard DIAGONAL[NUM_SQUARES];
    extern Bitboard ANTIDIAGONAL[NUM_SQUARES];
    extern Bitboard KNIGHT_MASK[NUM_SQUARES];
    extern Bitboard ADJACENT_MASK[NUM_SQUARES];

    void InitBitboards();
    std::string BitboardToString(Bitboard b);
    inline int Row(int i) { return i / NUM_FILES; }
    inline int Col(int i) { return i % NUM_RANKS; }
    inline int Ind(int row, int col) { return row * NUM_FILES + col; }

}