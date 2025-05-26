#pragma once

#include "Symbol.hpp"
#include "StateMapper.hpp"
#include "RegExString.hpp"
#include <queue>
#include <stack>

struct NFAFragment;
class NFA;

enum class RegExNotation
{
	Infix,
	Postfix
};

class RegEx
{
public:
	RegEx(const std::string& expression, RegExNotation notation) : expression(expression), notation(notation) {}
	RegEx(const char* expression, RegExNotation notation) : expression(expression), notation(notation) {}
	RegEx(const MutableRegExString& mutRegex) : expression(mutRegex.toString()) {}
	void simplify();
	RegEx convertToPostfix() const;
	std::string toString() const { return expression.toString(); }
	NFA toNFA() const;

	static NFAFragment buildFragment(const Symbol& symbol, StateMapper& namer);
	static NFAFragment buildFragment(const Symbol& symbol, const NFAFragment& f, StateMapper& namer);
	static NFAFragment buildFragment(const Symbol& symbol, const NFAFragment& f1, const NFAFragment& f2, StateMapper& namer);
	friend std::ostream& operator<<(std::ostream& os, const RegEx& r) { os << r.toString(); return os; }
private:
	RegExString expression;
	RegExNotation notation;

	// Converting helpers
	std::string addExplicitConcat() const;
	void handleLiteral(const Symbol& current, std::queue<Symbol>& output) const;
	void handleOperator(const Symbol& current, std::queue<Symbol>& output, std::stack<Symbol>& operators) const;
	void handleLeftParenthesis(const Symbol& current, std::stack<Symbol>& operators) const;
	void handleRightParenthesis(const Symbol& current, std::stack<Symbol>& operators, std::queue<Symbol>& output) const;
};

#include "NFA.hpp"

inline NFA RegEx::toNFA() const
{
	StateMapper namer;
	RegExString convertedToString(convertToPostfix().toString());

	if (convertedToString.size() == 0)
	{
		return NFA({}, "q0", {});
	}

	std::stack<NFAFragment> fragmentStack;
	RegExString::const_iterator it;
	for (it = convertedToString.begin(); it != convertedToString.end(); ++it)
	{
		if (it->isLiteral())
			fragmentStack.push(buildFragment(*it, namer));
		else
		if (it->isUnaryOperator())
		{
			NFAFragment f = fragmentStack.top();
			fragmentStack.pop();
			fragmentStack.push(buildFragment(*it, f, namer));
		}
		else
		if (it->isOperator())
		{
			NFAFragment f2 = fragmentStack.top();
			fragmentStack.pop();
			NFAFragment f1 = fragmentStack.top();
			fragmentStack.pop();
			fragmentStack.push(buildFragment(*it, f1, f2, namer));
		}
	}

	NFAFragment result = fragmentStack.top();
	fragmentStack.pop();

	return NFA(result.transitions, result.initial, result.accept);
}	

