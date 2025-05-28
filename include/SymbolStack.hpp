#pragma once

#include "Symbol.hpp"
#include <stack>

class SymbolStack
{
public:
	SymbolStack(const Symbol& initial);
	void push(const Symbol& symbol);
	void pop();
	bool empty() const;
	size_t size() const { return symbolStack.size(); }
	Symbol top() const;
	friend bool operator<(const SymbolStack& s1, const SymbolStack& s2)
	{
		return s1.symbolStack < s2.symbolStack;
	}
	friend bool operator!=(const SymbolStack& s1, const SymbolStack& s2)
	{
		return s1.symbolStack != s2.symbolStack;
	}
private:
	std::stack<Symbol> symbolStack;
};
