#include "Word.hpp"

#include <exception>

Word::Word(const std::string& symbols)
{
	std::string::const_iterator it;
	for (it = symbols.begin(); it != symbols.end(); ++it)
	{
		this->symbols.push_back(Symbol(*it));
	}
}

Word::Word(const char* symbols, const size_t arrsize)
{
	for (size_t i = 0; i < arrsize; ++i)
	{
		this->symbols.push_back(Symbol(symbols[i]));
	}
}

Symbol Word::processSymbol()
{
	if (symbols.size() <= 0)
		throw std::runtime_error("Cannot consume an empty word!");

	return symbols.pop_front();
}

size_t Word::size() const
{
	return symbols.size();
}

bool Word::definedOver(const Alphabet& alphabet) const
{
	Sequence<Symbol>::const_iterator it;
	for (it = symbols.begin(); it != symbols.end(); ++it)
	{
		if (!alphabet.contains(*it))
			return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const Word& w)
{
	Sequence<Symbol>::const_iterator it;
	for (it = w.symbols.begin(); it != w.symbols.end(); ++it)
	{
		os << *it;
	}
	return os;
}
