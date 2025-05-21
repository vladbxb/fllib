#include "Symbol.hpp"

#include <cctype>
#include <cassert>

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

bool Symbol::isOperator() const
{
	return symbol == Symbol::star().getSymbol() || symbol == Symbol::concat().getSymbol() || symbol == Symbol::_union().getSymbol();
}

bool Symbol::isUnaryOperator() const
{
	return symbol == Symbol::star().getSymbol();
}

bool Symbol::isLiteral() const
{
	return std::isalnum(static_cast<unsigned char>(symbol));
}

size_t Symbol::getOpPrecedence() const
{
	assert(isOperator());

	switch (symbol)
	{
		case Symbol::STAR:
			return 3;
		case Symbol::CONCAT:
			return 2;
		case Symbol::UNION:
			return 1;
		default:
			return 0;
	}
	return 0;
}
