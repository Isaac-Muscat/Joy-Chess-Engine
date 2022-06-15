#pragma once

#include <iostream>

#include "Bitboard.hpp"

namespace JoyChess {

    const std::string FEN_START = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const std::string FEN_TEST1 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2 ";

    enum class Color { White, Black };
    
    struct Board {
        Bitboard wPawn;
        Bitboard wKnight;
        Bitboard wBishop;
        Bitboard wRook;
        Bitboard wQueen;
        Bitboard wKing;
        Bitboard bPawn;
        Bitboard bKnight;
        Bitboard bBishop;
        Bitboard bRook;
        Bitboard bQueen;
        Bitboard bKing;
    
        bool wKingCastle;
        bool wQueenCastle;
        bool bKingCastle;
        bool bQueenCastle;
    
        Color nextMove;
    };
}