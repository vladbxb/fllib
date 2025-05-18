#pragma once

#include <iostream>

constexpr char LAMBDA = '\0';

class Symbol
{
public:
	Symbol(char symbol);
	char getSymbol() const;

	static Symbol lambda() { return Symbol(LAMBDA); }
	bool isLambda() const { return symbol == LAMBDA; }

	bool operator<(const Symbol& other) const { return symbol < other.symbol; }
	bool operator==(const Symbol& other) const { return symbol == other.symbol; }
	bool operator!=(const Symbol& other) const { return symbol != other.symbol; }

	friend std::ostream& operator<<(std::ostream& os, const Symbol& s);
private:
	char symbol;
};
