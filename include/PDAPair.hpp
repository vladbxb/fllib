#pragma once

#include <string>

struct PDAPair
{
	PDAPair(const State& state, const Symbol& symbol) : state(state) { stackOp.push_back(symbol); }
	PDAPair(const State& state, const std::string& symbols) : state(state)
	{
		if (symbols.length() <= 0 || symbols.length() > 2)
			throw std::runtime_error("Invalid stack operation! Exceeded symbol sequence length in stack operation expressions!");

		std::string::const_iterator it;
		for (it = symbols.begin(); it != symbols.end(); ++it)
		{
			stackOp.push_back(Symbol(*it));
		}
	}
	State state;
	std::vector<Symbol> stackOp;
};

inline bool operator<(const PDAPair& p1, const PDAPair& p2)
{
	if (p1.state < p2.state) return true;
	if (p2.state < p1.state) return false;
	return p1.stackOp < p2.stackOp;
}

inline bool operator!=(const PDAPair& p1, const PDAPair& p2)
{
	if (p1.state != p2.state) return true;
	if (p1.state == p2.state) return false;
	return p1.stackOp != p2.stackOp;
}
