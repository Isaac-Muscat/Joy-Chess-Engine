#include "Utilities.hpp"
#include "Board.hpp"

namespace JoyChess {
	
	Board::Board() {
        for (int i = 0; i < NUM_SQUARES; i++) squares[i] = Piece::None;
		for (int color = White; color < NUM_COLORS; color++) {
			for (int piece = Pawn; piece < NUM_GENERIC_PIECE_TYPES; piece++) {
				pieceBB[color][piece] = 0;
			}
		}

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

        for (; stringIndex < int(fen.length()) && fen[stringIndex] != ' '; stringIndex++) {
            char c = fen[stringIndex];
            if (c == '/') {
                i-=16;
                continue;
            }
            else if (c == 'p') { squares[i] = BPawn; pieceBB[Black][Pawn]     |= BIT(i); }
            else if (c == 'n') { squares[i] = BKnight; pieceBB[Black][Knight] |= BIT(i); }
            else if (c == 'b') { squares[i] = BBishop; pieceBB[Black][Bishop] |= BIT(i); }
            else if (c == 'r') { squares[i] = BRook; pieceBB[Black][Rook]     |= BIT(i); }
            else if (c == 'q') { squares[i] = BQueen; pieceBB[Black][Queen]   |= BIT(i); }
            else if (c == 'k') { squares[i] = BKing; pieceBB[Black][King]     |= BIT(i); }
            else if (c == 'P') { squares[i] = WPawn; pieceBB[White][Pawn]     |= BIT(i); }
            else if (c == 'N') { squares[i] = WKnight; pieceBB[White][Knight] |= BIT(i); }
            else if (c == 'B') { squares[i] = WBishop; pieceBB[White][Bishop] |= BIT(i); }
            else if (c == 'R') { squares[i] = WRook; pieceBB[White][Rook]     |= BIT(i); }
            else if (c == 'Q') { squares[i] = WQueen; pieceBB[White][Queen]   |= BIT(i); }
            else if (c == 'K') { squares[i] = WKing; pieceBB[White][King]     |= BIT(i); }
            
            if (c > '0' && c < '9') {i += static_cast<int>(c - '0');}
            else {i++;}

        }

        stringIndex++;
        if (fen[stringIndex] == 'w') boardStateInfo->sideToMove = Color::White;
        else boardStateInfo->sideToMove = Color::Black;
        stringIndex += 2;

        for (; stringIndex < int(fen.length()) && fen[stringIndex] != ' '; stringIndex++) {
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
		for (int i = Pawn; i <= King; i++) {
			o |= pieceBB[color][i];
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
                if (pieceBB[White][Pawn] & SQUARE[i * NUM_FILES + j]) out += "P ";
                else if (squares[Ind(i, j)] == WKnight && pieceBB[White][Knight] & SQUARE[i * NUM_FILES + j]) out += "N ";
                else if (squares[Ind(i, j)] == WBishop && pieceBB[White][Bishop] & SQUARE[i * NUM_FILES + j]) out += "B ";
                else if (squares[Ind(i, j)] == WRook && pieceBB[White][Rook] & SQUARE[i * NUM_FILES + j]) out += "R ";
                else if (squares[Ind(i, j)] == WQueen && pieceBB[White][Queen] & SQUARE[i * NUM_FILES + j]) out += "Q ";
                else if (squares[Ind(i, j)] == WKing && pieceBB[White][King] & SQUARE[i * NUM_FILES + j]) out += "K ";
                else if (squares[Ind(i, j)] == BPawn && pieceBB[Black][Pawn] & SQUARE[i * NUM_FILES + j]) out += "p ";
                else if (squares[Ind(i, j)] == BKnight && pieceBB[Black][Knight] & SQUARE[i * NUM_FILES + j]) out += "n ";
                else if (squares[Ind(i, j)] == BBishop && pieceBB[Black][Bishop] & SQUARE[i * NUM_FILES + j]) out += "b ";
                else if (squares[Ind(i, j)] == BRook && pieceBB[Black][Rook] & SQUARE[i * NUM_FILES + j]) out += "r ";
                else if (squares[Ind(i, j)] == BQueen && pieceBB[Black][Queen] & SQUARE[i * NUM_FILES + j]) out += "q ";
                else if (squares[Ind(i, j)] == BKing && pieceBB[Black][King] & SQUARE[i * NUM_FILES + j]) out += "k ";
                else out += "- ";
            }
            out += "|\n";
        }
        out += "  +-----------------+\n";
        out += "    a b c d e f g h\n";
        return out;
    }
}
