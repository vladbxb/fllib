#include "RegEx.hpp"

#include "expression.hpp"
#include "RegExSyntaxTree.hpp"
#include "NFA.hpp"
#include <exception>
#include <cassert>
#include <iostream>
#include <utility>
#include <set>

// NFA RegEx::toNFA() const
// {
// 	StateMapper namer;
// 
// 	RegExString convertedToString(convertToPostfix().toString());
// 
// 	if (convertedToString.size() == 0)
// 	{
// 		return std::make_unique<NFA>({}, "q0", {});
// 	}
// 
// 	std::stack<NFAFragment> fragmentStack;
// 	RegExString::const_iterator it;
// 	for (it = convertedToString.begin(); it != convertedToString.end(); ++it)
// 	{
// 		if (it->isLiteral())
// 			fragmentStack.push(buildFragment(*it, namer));
// 		else
// 		if (it->isUnaryOperator())
// 		{
// 			NFAFragment f = fragmentStack.top();
// 			fragmentStack.pop();
// 			fragmentStack.push(buildFragment(*it, f, namer));
// 		}
// 		else
// 		if (it->isOperator())
// 		{
// 			NFAFragment f2 = fragmentStack.top();
// 			fragmentStack.pop();
// 			NFAFragment f1 = fragmentStack.top();
// 			fragmentStack.pop();
// 			fragmentStack.push(buildFragment(*it, f1, f2, namer));
// 		}
// 	}
// 
// 	NFAFragment result = fragmentStack.top();
// 	fragmentStack.pop();
// 
// 	return NFA(result.transitions, result.initial, result.accept);
// }	

void RegEx::simplify()
{
	switch (notation)
	{
		case RegExNotation::Infix:
		{
			RegExSyntaxTree tree(toPostfix(expression.toString()));
			std::string result = tree.stringifyRoot();
			expression = RegExString(result);
			break;
		}
		case RegExNotation::Postfix:
		{
			RegExSyntaxTree tree(expression.toString());
			std::string result = tree.stringifyRoot();
			expression = RegExString(result);
			break;
		}
	}
}

RegEx RegEx::convertToPostfix() const
{
	// // Converting a regular expression to postfix notation using the Shunting-Yard algorithm
	if (notation == RegExNotation::Postfix)
		return *this;

	// RegExString concatForm(addExplicitConcat());


	// std::queue<Symbol> output;
	// std::stack<Symbol> operators;

	// RegExString::const_iterator it;
	// for (it = concatForm.begin(); it != concatForm.end(); ++it)
	// {
	// 	if (it->isLiteral())
	// 		handleLiteral(*it, output);
	// 	if (it->isOperator())
	// 		handleOperator(*it, output, operators);
	// 	if (it->isLeftParenthesis())
	// 		handleLeftParenthesis(*it, operators);
	// 	if (it->isRightParenthesis())
	// 		handleRightParenthesis(*it, operators, output);
	// }


	// while (!operators.empty())
	// {
	// 	if (operators.top().isLeftParenthesis())
	// 		throw std::runtime_error("Mismatched parentheses in regular expression!");
	// 	output.push(operators.top());
	// 	operators.pop();
	// }

	// std::string outputResult;
	// while (!output.empty())
	// {
	// 	outputResult.push_back(output.front().getSymbol());
	// 	output.pop();
	// }

	// return RegEx(outputResult, RegExNotation::Postfix);
	return RegEx(toPostfix(expression.toString()), RegExNotation::Postfix);
}

NFAFragment RegEx::buildFragment(const Symbol& symbol, StateMapper& namer)
{
	assert(symbol.isLiteral());
	State s1(namer.reserveState());
	State s2(namer.reserveState());
	NFATransition transition(s1, symbol, {s2});
	NFAFragment fragment = { {transition}, s1, {s2} };
	return fragment;
}

NFAFragment RegEx::buildFragment(const Symbol& symbol, const NFAFragment& f, StateMapper& namer)
{
	assert(symbol.isUnaryOperator());
	if (symbol.isStar())
	{
		State newInitial(namer.reserveState());
		State newAccept(namer.reserveState());
		NFATransitions newTransitions(f.transitions);
		NFATransition lambdaToOldInitial(newInitial, Symbol::lambda(), {f.initial});
		newTransitions.add(lambdaToOldInitial);
		States newFinal;
		newFinal.add(newAccept);

		NFATransition skip(newInitial, Symbol::lambda(), {newAccept});
		newTransitions.add(skip);

		States::const_iterator sit;
		for (sit = f.accept.begin(); sit != f.accept.end(); ++sit)
		{
			NFATransition lambdaToFinal(*sit, Symbol::lambda(), {newAccept});
			newTransitions.add(lambdaToFinal);
		}

		NFATransition lambdaFinalToOldInitial(newAccept, Symbol::lambda(), {f.initial});
		newTransitions.add(lambdaFinalToOldInitial);

		return { newTransitions, newInitial, newFinal };
	}
	// This should be impossible.
	return { {}, "", {} };
}

NFAFragment RegEx::buildFragment(const Symbol& symbol, const NFAFragment& f1, const NFAFragment& f2, StateMapper& namer)
{
	assert(symbol.isOperator());
	if (symbol.isUnion())
	{
		State newInitial(namer.reserveState());
		State newAccept(namer.reserveState());
		NFATransitions newTransitions(f1.transitions);
		newTransitions.add(f2.transitions);
		NFATransition lambdaToInitials(newInitial, Symbol::lambda(), {f1.initial, f2.initial});
		newTransitions.add(lambdaToInitials);

		States::const_iterator it;
		for (it = f1.accept.begin(); it != f1.accept.end(); ++it)
		{
			NFATransition lambdaToNewAccept(*it, Symbol::lambda(), {newAccept});
			newTransitions.add(lambdaToNewAccept);
		}
		for (it = f2.accept.begin(); it != f2.accept.end(); ++it)
		{
			NFATransition lambdaToNewAccept(*it, Symbol::lambda(), {newAccept});
			newTransitions.add(lambdaToNewAccept);
		}	

		return { newTransitions, newInitial, {newAccept} };
	}
	if (symbol.isConcat())
	{
		State newInitial(namer.reserveState());
		State newAccept(namer.reserveState());
		NFATransitions newTransitions(f1.transitions);
		newTransitions.add(f2.transitions);
		NFATransition lambdaToOldInitial(newInitial, Symbol::lambda(), {f1.initial});
		newTransitions.add(lambdaToOldInitial);
		States::const_iterator it;
		for (it = f1.accept.begin(); it != f1.accept.end(); ++it)
		{
			NFATransition middleLambda(*it, Symbol::lambda(), {f2.initial});
			newTransitions.add(middleLambda);
		}
		for (it = f2.accept.begin(); it != f2.accept.end(); ++it)
		{
			NFATransition endLambda(*it, Symbol::lambda(), {newAccept});
			newTransitions.add(endLambda);
		}

		return { newTransitions, newInitial, {newAccept} };
	}
	// This should be impossible.
	return { {}, "", {} };
}

std::string RegEx::addExplicitConcat() const
{
	// if (notation != RegExNotation::Infix)
	// 	return expression.toString();

	// if (expression.size() == 0)
	// 	return "";
	// else if (expression.size() == 1)
	// {
	// 	std::string result;
	// 	Sequence<Symbol>::const_iterator it = expression.begin();
	// 	result.push_back(it->getSymbol());
	// 	return result;
	// }

	// std::string result;
	// RegExString::const_iterator it1 = expression.begin();
	// RegExString::const_iterator it2 = expression.begin() + 1;
	// while (it2 != expression.end())
	// {
	// 	result.push_back(it1->getSymbol());
	// 	if (needsConcat(*it1, *it2))
	// 	{
	// 		result.push_back(Symbol::CONCAT);
	// 	}
	// 	++it1;
	// 	++it2;
	// }
	// result.push_back(it1->getSymbol());

	// return result;
	std::string infix;
	Sequence<Symbol>::const_iterator it;
	for (it = expression.begin(); it != expression.end(); ++it)
	{
		infix.push_back(it->getSymbol());
	}
	return concatAll(infix);
}

// bool RegEx::needsConcat(const Symbol& a, const Symbol& b) const
// {
// 	bool a_req = a.isLiteral() || a.isStar() || a.isRightParenthesis();
// 	bool b_req = b.isLiteral() || b.isLeftParenthesis();
// 	return a_req && b_req;
// }

void RegEx::handleLiteral(const Symbol& current, std::queue<Symbol>& output) const
{
	output.push(current);
}

void RegEx::handleOperator(const Symbol& current, std::queue<Symbol>& output, std::stack<Symbol>& operators) const
{
	while ( !operators.empty() && (operators.top() != Symbol::leftParenthesis()) &&
			(operators.top().getOpPrecedence() >= current.getOpPrecedence()) )
	{
		output.push(operators.top());
		operators.pop();
	}
	operators.push(current);
}

void RegEx::handleLeftParenthesis(const Symbol& current, std::stack<Symbol>& operators) const
{
	operators.push(current);
}

void RegEx::handleRightParenthesis(const Symbol& current, std::stack<Symbol>& operators, std::queue<Symbol>& output) const
{
	while (!operators.empty() && !operators.top().isLeftParenthesis())
	{
		output.push(operators.top());
		operators.pop();
	}

	if (operators.empty())
		throw std::runtime_error("Mismatched parentheses in regular expression!");

	operators.pop();
}
