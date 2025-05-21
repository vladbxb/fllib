#pragma once


#include <iostream>

class Symbol
{
public:
	static constexpr char LAMBDA = '\0';
	static constexpr char STAR = '*';
	static constexpr char CONCAT = '.';
	static constexpr char UNION = '+';
	static constexpr char LEFT_PARENTHESIS = '(';
	static constexpr char RIGHT_PARENTHESIS = ')';

	Symbol(char symbol);
	char getSymbol() const;

	static Symbol lambda() { return Symbol(LAMBDA); }
	static Symbol star() { return Symbol(STAR); }
	static Symbol concat() { return Symbol(CONCAT); }
	static Symbol _union() { return Symbol(UNION); }
	static Symbol leftParenthesis() { return Symbol(LEFT_PARENTHESIS); }
	static Symbol rightParenthesis() { return Symbol(RIGHT_PARENTHESIS); }
	bool isLambda() const { return symbol == LAMBDA; }

	bool isOperator() const;
	bool isUnaryOperator() const;
	bool isStar() const { return symbol == STAR; }
	bool isConcat() const { return symbol == CONCAT; }
	bool isUnion() const { return symbol == UNION; }
	bool isLiteral() const;
	bool isLeftParenthesis() const { return symbol == LEFT_PARENTHESIS; }
	bool isRightParenthesis() const { return symbol == RIGHT_PARENTHESIS; }

	size_t getOpPrecedence() const;

	bool operator<(const Symbol& other) const { return symbol < other.symbol; }
	bool operator==(const Symbol& other) const { return symbol == other.symbol; }
	bool operator!=(const Symbol& other) const { return symbol != other.symbol; }

	friend std::ostream& operator<<(std::ostream& os, const Symbol& s);
private:
	char symbol;
};
