#pragma once

#include "Symbol.hpp"
#include "Alphabet.hpp"
#include "Sequence.hpp"

#include <utility>
#include <iostream>

class Word
{
public:
	Word(const std::string& symbols) : symbols(symbols) {}
	Word(const char* symbols) : symbols(std::string(symbols)) {}
	Symbol processSymbol();
	size_t size() const;
	bool definedOver(const Alphabet& alphabet) const;

	friend std::ostream& operator<<(std::ostream& os, const Word& w);
private:
	MutableSequence<Symbol> symbols;
};	
