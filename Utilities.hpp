#pragma once

#include <iostream>
#include <string>

#define BIT(x) (((uint64_t)1) << (x))

namespace JoyChess {
    
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

    enum Direction {
        SOUTH_WEST = -9, SOUTH = -8, SOUTH_EAST = -7,
        WEST = -1, EAST = 1,
        NORTH_WEST = 7, NORTH = 8, NORTH_EAST = 9
    };

    const std::string FEN[] = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ",
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "
    };

    enum CastleRights {
        NoCastleRights = 0,
        WKingCastle = 1,
        WQueenCastle = WKingCastle << 1,
        BKingCastle = WKingCastle << 2,
        BQueenCastle = WKingCastle << 3
    };

    enum Piece {
        WPawn, WKnight, WBishop, WRook, WQueen, WKing,        
        BPawn, BKnight, BBishop, BRook, BQueen, BKing,
        NUM_PIECE_TYPES, None
    };

    // For easy color + GenericPiece to index white vs black pieces.
    enum GenericPiece {
        Pawn = 0, Knight, Bishop, Rook, Queen, King
    };

    // Add 6 for color + GenericPiece to index white vs black pieces (6x generic pieces).
    enum Color { White = 0, Black = 6 };

    enum MoveType {
        QuietMove, DoublePawnPush, Capture, KingCastle, QueenCastle, EPCapture,
        KnightPromotion, BishopPromotion, RookPromotion, QueenPromotion,
        KnightPromoCapture, BishopPromoCapture, RookPromoCapture, QueenPromoCapture
    };

    enum Square { 
        a1 = 0, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8,
        NONE
    };

    struct Move {
        int from;
        int to;
        MoveType moveType;
    };

    inline int Rank(int i) { return i / NUM_FILES; }
    inline int File(int i) { return i % NUM_RANKS; }
    inline int Ind(int rank, int file) { return rank * NUM_FILES + file; }
    inline Color GetOpponentColor(Color color) { 
        if (color == White) return Black;
        else return White;
    }
    inline Direction GetPawnDirection(Color color) {
        if (color == White) return NORTH;
        else return SOUTH;
    }

    std::string SquareToString(Square s);
}