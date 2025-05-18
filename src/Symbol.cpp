#include "Symbol.hpp"

Symbol::Symbol(char symbol) : symbol(symbol) {}

char Symbol::getSymbol() const
{
	return symbol;
}

std::ostream& operator<<(std::ostream& os, const Symbol& s)
{
	os << s.symbol;
	return os;
}
