#include "ChessEngine.hpp"

namespace JoyChess {

    Board::Board() {
        wPawn = 0;
        wKnight = 0;
        wBishop = 0;
        wRook = 0;
        wQueen = 0;
        wKing = 0;
        bPawn = 0;
        bKnight = 0;
        bBishop = 0;
        bRook = 0;
        bQueen = 0;
        bKing = 0;

        wKingCastle = 0;
        wQueenCastle = 0;
        bKingCastle = 0;
        bQueenCastle = 0;

        halfMoveClock = 0;
        fullMoveCounter = 0;

        sideToMove = Color::White;
        enPassantTarget = Square::e4;
    }

    Board::Board(const std::string& fen) : Board() {
        int i = 0;
        int stringIndex = 0;

        for (; stringIndex < fen.length() && fen[stringIndex] != ' '; stringIndex++) {
            char c = fen[stringIndex];
            if (c == '/') continue;
            else if (c == 'p') bPawn     |= BIT(i);
            else if (c == 'n') bKnight   |= BIT(i);
            else if (c == 'b') bBishop   |= BIT(i);
            else if (c == 'r') bRook     |= BIT(i);
            else if (c == 'q') bQueen    |= BIT(i);
            else if (c == 'k') bKing     |= BIT(i);
            else if (c == 'P') wPawn     |= BIT(i);
            else if (c == 'N') wKnight   |= BIT(i);
            else if (c == 'B') wBishop   |= BIT(i);
            else if (c == 'R') wRook     |= BIT(i);
            else if (c == 'Q') wQueen    |= BIT(i);
            else if (c == 'K') wKing     |= BIT(i);
            
            if (c > '0' && c < '9') {i += static_cast<int>(c - '0');}
            else {i++;}

        }

        stringIndex++;
        if (fen[stringIndex] == 'w') sideToMove = Color::White;
        else sideToMove = Color::Black;
        stringIndex += 2;

        for (; stringIndex < fen.length() && fen[stringIndex] != ' '; stringIndex++) {
            char c = fen[stringIndex];
            if (c == 'k') bKingCastle = true;
            if (c == 'q') bQueenCastle = true;
            if (c == 'K') wKingCastle = true;
            if (c == 'Q') wQueenCastle = true;
        }

        stringIndex++;
        if (fen[stringIndex] != '-') {
            int epTarget = (8 - (fen[stringIndex+1] - '0')) * NUM_FILES + (fen[stringIndex] - 'a');
            enPassantTarget = static_cast<Square>(epTarget);
        }
        stringIndex += 2;
        
    }

    std::string Board::ToString() {
        std::string out = "[Info]\n";

        out += "Side To Move -----> ";
        if (sideToMove == Color::White) out += "White\n";
        else out += "Black\n";
            
        out += "Castling Rights --> ";
        if (wKingCastle) out += 'K';
        if (wQueenCastle) out += 'Q';
        if (bKingCastle) out += 'k';
        if (bQueenCastle) out += 'q';

        out += "\nEP Target Square -> ";
        out += SquareToString(enPassantTarget);

        out += "\nHalf Move Clock --> ";
        out += std::to_string(halfMoveClock);
        
        out += "\nFull Move Clock --> ";
        out += std::to_string(fullMoveCounter);

        out += "\n  +-----------------+\n";
        for (int i = 0; i < NUM_RANKS; i++) {
            out += std::to_string(NUM_RANKS - i);
            out += " | ";
            for (int j = 0; j < NUM_FILES; j++) {
                if (wPawn & SQUARE[i * NUM_FILES + j]) out += "P ";
                else if (wKnight & SQUARE[i * NUM_FILES + j]) out += "N ";
                else if (wBishop & SQUARE[i * NUM_FILES + j]) out += "B ";
                else if (wRook & SQUARE[i * NUM_FILES + j]) out += "R ";
                else if (wQueen & SQUARE[i * NUM_FILES + j]) out += "Q ";
                else if (wKing & SQUARE[i * NUM_FILES + j]) out += "K ";
                else if (bPawn & SQUARE[i * NUM_FILES + j]) out += "p ";
                else if (bKnight & SQUARE[i * NUM_FILES + j]) out += "n ";
                else if (bBishop & SQUARE[i * NUM_FILES + j]) out += "b ";
                else if (bRook & SQUARE[i * NUM_FILES + j]) out += "r ";
                else if (bQueen & SQUARE[i * NUM_FILES + j]) out += "q ";
                else if (bKing & SQUARE[i * NUM_FILES + j]) out += "k ";
                else out += "- ";
            }
            out += "|\n";
        }
        out += "  +-----------------+\n";
        out += "    a b c d e f g h\n";
        return out;
    }

    std::string SquareToString(Square s) {
        int i = static_cast<int>(s);

        if (i == 64) return "None";
        else if (i > 64) return std::to_string(i);
        int row = Row(i);
        char col = Col(i);

        std::string out = "";
        out.push_back('a' + col);
        out += std::to_string(8 - row);
        return out;
    }
}