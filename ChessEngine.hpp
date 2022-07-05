#pragma once

#include <iostream>

#include "Bitboard.hpp"

namespace JoyChess {

    const std::string FEN_START = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const std::string FEN_TEST1 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2 ";

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
    enum class Color { White, Black };
    enum class MoveType {
        QuietMove, DoublePawnPush, Capture, KingCastle, QueenCastle, EPCapture,
        KnightPromotion, BishopPromotion, RookPromotion, QueenPromotion,
        KnightPromoCapture, BishopPromoCapture, RookPromoCapture, QueenPromoCapture
    };

    enum Square { 
        a8 = 0, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1,
        NONE
    };

    struct Move {
        int from;
        int to;
        Piece capturedPiece;
        MoveType moveType;
    };

    // Info to be copied every make/unmake move.
    struct BoardStateInfo {
        int castleRights = 0;
        int halfMoveClock = 0;
        int fullMoveCounter = 0;
        Square enPassantTarget = Square::NONE;
        Color sideToMove = Color::White;
    };
    
    class Board {
    public:
        // Info to be modified every make/unmake move.
        Piece squares[NUM_SQUARES];
        Bitboard pieceBB[NUM_PIECE_TYPES];
        
        // Info to be copied every make/unmake move.
        BoardStateInfo* boardStateInfo;
        
    public:
        Board();
        Board(const std::string& fen);

        std::string ToString();
        void MakeMove(const Move& move);
    };

    std::string SquareToString(Square s);
}