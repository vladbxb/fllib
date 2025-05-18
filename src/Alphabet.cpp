#include "Alphabet.hpp"

Alphabet::Alphabet(const std::string& symbols)
{
	std::string::const_iterator it;
	for (it = symbols.begin(); it != symbols.end(); ++it)
		alphabet.insert(*it);
}

void Alphabet::add(const Symbol& symbol)
{
	if (symbol == Symbol::lambda())
		return;
	alphabet.insert(symbol);
}

bool Alphabet::contains(const Symbol& symbol) const
{
	return alphabet.find(symbol) != alphabet.end();
}

bool Alphabet::contains(char c) const
{
	return alphabet.find(Symbol(c)) != alphabet.end();
}
