#include "Utilities.hpp"

namespace JoyChess {

    std::string SquareToString(Square s) {
        int i = static_cast<int>(s);

        if (i == 64) return "None";
        else if (i > 64) return std::to_string(i);
        int rank = Rank(i);
        char file = File(i);

        std::string out = "";
        out.push_back('a' + file);
        out += std::to_string(rank);
        return out;
    }
}