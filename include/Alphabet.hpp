#pragma once

#include "Symbol.hpp"
#include <set>

class Alphabet
{
public:
	using iterator = std::set<Symbol>::iterator;
	using const_iterator = std::set<Symbol>::const_iterator;

	iterator begin() { return alphabet.begin(); }
	iterator end() { return alphabet.end(); }
	const_iterator begin() const { return alphabet.begin(); }
	const_iterator end() const { return alphabet.end(); }

	Alphabet() = default;
	Alphabet(const std::string& symbols);
	void add(const Symbol& symbol);
	bool contains(const Symbol& symbol) const;
	bool contains(char c) const;
private:
	std::set<Symbol> alphabet;
};
