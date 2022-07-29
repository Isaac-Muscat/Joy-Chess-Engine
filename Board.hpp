#pragma once
#include "Utilities.hpp"
#include "Bitboard.hpp"

namespace JoyChess {

	// Info to be copied every make/unmake move.
    struct BoardStateInfo {
        int castleRights = 0;
        int halfMoveClock = 0;
        int fullMoveCounter = 0;
        Square enPassantTarget = Square::NONE;
        Color sideToMove = Color::White;
		Piece capturedPiece = None;
    };
    
    class Board {
    public:
        // Info to be modified every make/unmake move.
        Piece squares[NUM_SQUARES];
        Bitboard pieceBB[NUM_COLORS][NUM_GENERIC_PIECE_TYPES];
        
        // Info to be copied every make/unmake move.
        BoardStateInfo* boardStateInfo;
        
    public:
        Board();
        Board(const std::string& fen);

		Bitboard GetOccupied(Color color) const;
		Bitboard GetOccupied() const;

        std::string ToString();
    };
}