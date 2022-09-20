#pragma once

static constexpr char notSymbol[] = "!";
static constexpr char andSymbol[] = "&";
static constexpr char orSymbol[] = "|";
static constexpr char xorSymbol[] = "^";
static constexpr char implicationSymbol[] = "->";
static constexpr char equivalenceSymbol[] = "<->";
static constexpr char globallySymbol[] = "G";
static constexpr char nextWeakSymbol[] = "W";
static constexpr char nextStrongSymbol[] = "X";
static constexpr char futureSymbol[] = "F";
static constexpr char untilSymbol[] = "U";
static constexpr char releaseSymbol[] = "R";
static constexpr char previousSymbol[] = "P";
static constexpr char onceSymbol[] = "O";
static constexpr char sinceSymbol[] = "S";
static constexpr char historicallySymbol[] = "H";
static constexpr char universalSymbol[] = "A";
static constexpr char existentialSymbol[] = "E";
static constexpr char quoteSymbol[] = "Q";
static constexpr char countingSymbol[] = "C";
static constexpr std::array<char, 5> futureOperators{'G', 'W', 'X', 'F', 'U'};
static constexpr std::array<char, 5> pastOperators{'R', 'P', 'O', 'S', 'H'};
static constexpr std::array<char, 4> firstOrderOperators{'A', 'E', 'Q', 'C'};
