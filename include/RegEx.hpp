#pragma once

#include "Symbol.hpp"
#include "Sequence.hpp"
#include "StateMapper.hpp"
#include "NFA.hpp"
#include <queue>
#include <stack>

enum class RegExNotation
{
	Infix,
	Postfix
};

class RegExString
{
public:
	using iterator = Sequence<Symbol>::iterator;
	using const_iterator = Sequence<Symbol>::const_iterator;

	iterator begin() { return string.begin(); }
	iterator end() { return string.end(); }

	const_iterator begin() const { return string.begin(); }
	const_iterator end() const { return string.end(); }

	RegExString(const std::string& string) : string(string) {}
	// Convert const char* to const_iterable
	RegExString(const char* string) : string(std::string(string)) {}

	size_t size() const { return string.size(); }

	std::string toString() const;
private:
	Sequence<Symbol> string;
};

class RegEx
{
public:
	RegEx(const std::string& expression, RegExNotation notation) : expression(expression), notation(notation) {}
	RegEx(const char* expression, RegExNotation notation) : expression(expression), notation(notation) {}
	RegEx convertToPostfix() const;
	std::string toString() const { return expression.toString(); }
	NFA toNFA() const;

	static NFAFragment buildFragment(const Symbol& symbol, StateMapper& namer);
	static NFAFragment buildFragment(const Symbol& symbol, const NFAFragment& f, StateMapper& namer);
	static NFAFragment buildFragment(const Symbol& symbol, const NFAFragment& f1, const NFAFragment& f2, StateMapper& namer);
private:
	RegExString expression;
	RegExNotation notation;

	// Converting helpers
	std::string addExplicitConcat() const;
	bool needsConcat(const Symbol& a, const Symbol& b) const;
	void handleLiteral(const Symbol& current, std::queue<Symbol>& output) const;
	void handleOperator(const Symbol& current, std::queue<Symbol>& output, std::stack<Symbol>& operators) const;
	void handleLeftParenthesis(const Symbol& current, std::stack<Symbol>& operators) const;
	void handleRightParenthesis(const Symbol& current, std::stack<Symbol>& operators, std::queue<Symbol>& output) const;
};
