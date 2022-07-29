#pragma once

#include <cstdint>
#include <array>
#include <string>

/*  Bitboard Format
    56 57 58 59 60 61 62 63
    48 49 50 51 52 53 54 55
    40 41 42 43 44 45 46 47
    32 33 34 35 36 37 38 39
    24 25 26 27 28 29 30 31
    16 17 18 19 20 21 22 23
    08 09 10 11 12 13 14 15
    00 01 02 03 04 05 06 07
*/

namespace JoyChess {

    typedef uint64_t Bitboard;

    extern Bitboard SQUARE[NUM_SQUARES];
    extern Bitboard RANK[NUM_SQUARES];
    extern Bitboard FILE[NUM_SQUARES];
    extern Bitboard DIAGONAL[NUM_SQUARES];
    extern Bitboard ANTIDIAGONAL[NUM_SQUARES];
    extern Bitboard KNIGHT_MASK[NUM_SQUARES];
    extern Bitboard ADJACENT_MASK[NUM_SQUARES];
    extern Bitboard PAWN_PUSH[NUM_COLORS][NUM_SQUARES];
    extern Bitboard PAWN_ATTACKS[NUM_COLORS][NUM_SQUARES];

    extern int PAWN_START_RANK[NUM_COLORS];
    extern int PAWN_PROMO_FROM_RANK[NUM_COLORS];
    extern int PAWN_EP_CAPTURE_FROM_RANK[NUM_COLORS];

    int bitScanForward(Bitboard bb);
    void InitBitboards();
    std::string BitboardToString(Bitboard b);
}