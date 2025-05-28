#pragma once

#include "State.hpp"
#include "SymbolStack.hpp"

struct PushdownID
{
	PushdownID(const State& state, const SymbolStack& symbolStack) : state(state), symbolStack(symbolStack) {}
	State state;
	SymbolStack symbolStack;
	
};

inline bool operator<(const PushdownID& p1, const PushdownID& p2)
{
	if (p1.state < p2.state)
		return true;
	if (p2.state < p1.state)
		return false;
	return p1.symbolStack < p2.symbolStack;
}
inline bool operator!=(const PushdownID& p1, const PushdownID& p2)
{
	if (p1.state != p2.state)
		return true;
	if (p1.state == p2.state)
		return false;
	return p1.symbolStack != p2.symbolStack;
}
