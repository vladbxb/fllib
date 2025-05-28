#include "SymbolStack.hpp"

SymbolStack::SymbolStack(const Symbol& initial)
{
	symbolStack.push(initial);
}

void SymbolStack::push(const Symbol& symbol)
{
	symbolStack.push(symbol);
}

void SymbolStack::pop()
{
	if (!symbolStack.empty())
		symbolStack.pop();
}

bool SymbolStack::empty() const
{
	return symbolStack.empty();
}

Symbol SymbolStack::top() const
{
	if (symbolStack.empty())
		return Symbol::lambda();
	return symbolStack.top();
}
