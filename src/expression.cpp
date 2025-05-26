#include "expression.hpp"

#include "Sequence.hpp"
#include <exception>

bool needsConcat(const Symbol& a, const Symbol&b)
{
	bool a_req = a.isLiteral() || a.isStar() || a.isRightParenthesis();
	bool b_req = b.isLiteral() || b.isLeftParenthesis();
	return a_req && b_req;
}

std::string concatAll(const std::string& infix)
{
	// Takes an infix expression and explicitly adds concat symbols by the rules of implicit catenation. Causes undefined behavior if taken any other type of expression.
	if (infix.size() == 0)
		return "";
	else if (infix.size() == 1)
	{
		std::string result;
		result.push_back(*infix.begin());
		return result;
	}

	std::string result;
	std::string::const_iterator it1 = infix.begin();
	std::string::const_iterator it2 = infix.begin() + 1;
	while (it2 != infix.end())
	{
		result.push_back(*it1);
		if (needsConcat(*it1, *it2))
		{
			result.push_back(Symbol::CONCAT);
		}
		++it1;
		++it2;
	}
	result.push_back(*it1);

	return result;
}

std::string toPostfix(const std::string& infix)
{
	// Takes an infix expression and converts it to postfix using the Shunting-Yard algorithm. Causes undefined behavior if it takes any other type of expression.
	Sequence<Symbol> concatForm = concatAll(infix);	

	std::queue<Symbol> output;
	std::stack<Symbol> operators;

	Sequence<Symbol>::const_iterator it;
	for (it = concatForm.begin(); it != concatForm.end(); ++it)
	{
		if (it->isLiteral())
			handleLiteral(*it, output);
		if (it->isOperator())
			handleOperator(*it, output, operators);
		if (it->isLeftParenthesis())
			handleLeftParenthesis(*it, operators);
		if (it->isRightParenthesis())
			handleRightParenthesis(*it, operators, output);
	}

	while (!operators.empty())
	{
		if (operators.top().isLeftParenthesis())
			throw std::runtime_error("Mismatched parentheses in regular expression!");
		output.push(operators.top());
		operators.pop();
	}

	std::string outputResult;
	while (!output.empty())
	{
		outputResult.push_back(output.front().getSymbol());
		output.pop();
	}

	return outputResult;
}

void handleLiteral(const Symbol& current, std::queue<Symbol>& output)
{
	output.push(current);
}

void handleOperator(const Symbol& current, std::queue<Symbol>& output, std::stack<Symbol>& operators)
{
	while ( !operators.empty() && (operators.top() != Symbol::leftParenthesis()) &&
			(operators.top().getOpPrecedence() >= current.getOpPrecedence()) )
	{
		output.push(operators.top());
		operators.pop();
	}
	operators.push(current);
}

void handleLeftParenthesis(const Symbol& current, std::stack<Symbol>& operators)
{
	operators.push(current);
}

void handleRightParenthesis(const Symbol& current, std::stack<Symbol>& operators, std::queue<Symbol>& output)
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
