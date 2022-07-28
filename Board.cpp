#include "Utilities.hpp"
#include "Board.hpp"

namespace JoyChess {
	
	Board::Board() {
        for (int i = 0; i < NUM_SQUARES; i++) squares[i] = Piece::None;
        for (int piece = WPawn; piece < NUM_PIECE_TYPES; piece++) pieceBB[piece] = 0;
        
        boardStateInfo = new BoardStateInfo();

        boardStateInfo->castleRights = CastleRights::NoCastleRights;
        boardStateInfo->halfMoveClock = 0;
        boardStateInfo->fullMoveCounter = 0;
        boardStateInfo->sideToMove = Color::White;
        boardStateInfo->enPassantTarget = Square::NONE;
    }

    Board::Board(const std::string& fen) : Board() {
        int i = 56;
        int stringIndex = 0;

        for (; stringIndex < fen.length() && fen[stringIndex] != ' '; stringIndex++) {
            char c = fen[stringIndex];
            if (c == '/') {
                i-=16;
                continue;
            }
            else if (c == 'p') { squares[i] = BPawn; pieceBB[BPawn]     |= BIT(i); }
            else if (c == 'n') { squares[i] = BKnight; pieceBB[BKnight] |= BIT(i); }
            else if (c == 'b') { squares[i] = BBishop; pieceBB[BBishop] |= BIT(i); }
            else if (c == 'r') { squares[i] = BRook; pieceBB[BRook]     |= BIT(i); }
            else if (c == 'q') { squares[i] = BQueen; pieceBB[BQueen]   |= BIT(i); }
            else if (c == 'k') { squares[i] = BKing; pieceBB[BKing]     |= BIT(i); }
            else if (c == 'P') { squares[i] = WPawn; pieceBB[WPawn]     |= BIT(i); }
            else if (c == 'N') { squares[i] = WKnight; pieceBB[WKnight] |= BIT(i); }
            else if (c == 'B') { squares[i] = WBishop; pieceBB[WBishop] |= BIT(i); }
            else if (c == 'R') { squares[i] = WRook; pieceBB[WRook]     |= BIT(i); }
            else if (c == 'Q') { squares[i] = WQueen; pieceBB[WQueen]   |= BIT(i); }
            else if (c == 'K') { squares[i] = WKing; pieceBB[WKing]     |= BIT(i); }
            
            if (c > '0' && c < '9') {i += static_cast<int>(c - '0');}
            else {i++;}

        }

        stringIndex++;
        if (fen[stringIndex] == 'w') boardStateInfo->sideToMove = Color::White;
        else boardStateInfo->sideToMove = Color::Black;
        stringIndex += 2;

        for (; stringIndex < fen.length() && fen[stringIndex] != ' '; stringIndex++) {
            char c = fen[stringIndex];
            if (c == 'k') boardStateInfo->castleRights |= CastleRights::BKingCastle;
            if (c == 'q') boardStateInfo->castleRights |= CastleRights::BQueenCastle;
            if (c == 'K') boardStateInfo->castleRights |= CastleRights::WKingCastle;
            if (c == 'Q') boardStateInfo->castleRights |= CastleRights::WQueenCastle;
        }

        stringIndex++;
        if (fen[stringIndex] != '-') {
            int epTarget = ((fen[stringIndex+1] - '0')) * NUM_FILES + (fen[stringIndex] - 'a');
            boardStateInfo->enPassantTarget = static_cast<Square>(epTarget);
        }
        stringIndex += 2;
        
    }

	Bitboard Board::GetOccupied(Color color) const {
		Bitboard o = 0;
		for (int i = Pawn + color; i <= King + color; i++) {
			o |= pieceBB[i];
		}
		return o;
	}

	Bitboard Board::GetOccupied() const {
		return (GetOccupied(Color::White) | GetOccupied(Color::Black));
	}

    std::string Board::ToString() {
        std::string out = "[Info]\n";

        out += "Side To Move -----> ";
        if (boardStateInfo->sideToMove == Color::White) out += "White\n";
        else out += "Black\n";
            
        out += "Castling Rights --> ";
        if (boardStateInfo->castleRights & CastleRights::WKingCastle) out += 'K';
        if (boardStateInfo->castleRights & CastleRights::WQueenCastle) out += 'Q';
        if (boardStateInfo->castleRights & CastleRights::BKingCastle) out += 'k';
        if (boardStateInfo->castleRights & CastleRights::BQueenCastle) out += 'q';

        out += "\nEP Target Square -> ";
        out += SquareToString(boardStateInfo->enPassantTarget);

        out += "\nHalf Move Clock --> ";
        out += std::to_string(boardStateInfo->halfMoveClock);
        
        out += "\nFull Move Clock --> ";
        out += std::to_string(boardStateInfo->fullMoveCounter);

        out += "\n  +-----------------+\n";
        for (int i = NUM_RANKS - 1; i >= 0; i--) {
            out += std::to_string(NUM_RANKS - i);
            out += " | ";
            for (int j = 0; j < NUM_FILES; j++) {
                if (pieceBB[WPawn] & SQUARE[i * NUM_FILES + j]) out += "P ";
                else if (squares[Ind(i, j)] == WKnight && pieceBB[WKnight] & SQUARE[i * NUM_FILES + j]) out += "N ";
                else if (squares[Ind(i, j)] == WBishop && pieceBB[WBishop] & SQUARE[i * NUM_FILES + j]) out += "B ";
                else if (squares[Ind(i, j)] == WRook && pieceBB[WRook] & SQUARE[i * NUM_FILES + j]) out += "R ";
                else if (squares[Ind(i, j)] == WQueen && pieceBB[WQueen] & SQUARE[i * NUM_FILES + j]) out += "Q ";
                else if (squares[Ind(i, j)] == WKing && pieceBB[WKing] & SQUARE[i * NUM_FILES + j]) out += "K ";
                else if (squares[Ind(i, j)] == BPawn && pieceBB[BPawn] & SQUARE[i * NUM_FILES + j]) out += "p ";
                else if (squares[Ind(i, j)] == BKnight && pieceBB[BKnight] & SQUARE[i * NUM_FILES + j]) out += "n ";
                else if (squares[Ind(i, j)] == BBishop && pieceBB[BBishop] & SQUARE[i * NUM_FILES + j]) out += "b ";
                else if (squares[Ind(i, j)] == BRook && pieceBB[BRook] & SQUARE[i * NUM_FILES + j]) out += "r ";
                else if (squares[Ind(i, j)] == BQueen && pieceBB[BQueen] & SQUARE[i * NUM_FILES + j]) out += "q ";
                else if (squares[Ind(i, j)] == BKing && pieceBB[BKing] & SQUARE[i * NUM_FILES + j]) out += "k ";
                else out += "- ";
            }
            out += "|\n";
        }
        out += "  +-----------------+\n";
        out += "    a b c d e f g h\n";
        return out;
    }
}
