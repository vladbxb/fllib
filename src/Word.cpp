#include "Word.hpp"

#include <exception>

Symbol Word::processSymbol()
{
	if (symbols.size() <= 0)
		throw std::runtime_error("Cannot consume an empty word!");

	return symbols.popFrontWithResult();
}

size_t Word::size() const
{
	return symbols.size();
}

bool Word::definedOver(const Alphabet& alphabet) const
{
	MutableSequence<Symbol>::const_iterator it;
	for (it = symbols.begin(); it != symbols.end(); ++it)
	{
		if (!alphabet.contains(*it))
			return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const Word& w)
{
	MutableSequence<Symbol>::const_iterator it;
	for (it = w.symbols.begin(); it != w.symbols.end(); ++it)
	{
		os << *it;
	}
	return os;
}
