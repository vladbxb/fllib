#pragma once

#include "Symbol.hpp"
#include "RegExString.hpp"
#include <utility>
#include <stack>
#include <string>

struct Node
{
	Node(const Symbol& symbol) : symbol(symbol), left(nullptr), right(nullptr) {}

	Symbol symbol;
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;
};

class RegExSyntaxTree
{
public:
	RegExSyntaxTree(const RegExString& postfix);
	std::string stringifyRoot();
private:
	std::unique_ptr<Node> root;
	void build(const RegExString& postfix);
	std::unique_ptr<Node> simplify(std::unique_ptr<Node> node);
	bool structurallyEqual(const std::unique_ptr<Node>& left, const std::unique_ptr<Node>& right);
	std::string stringify(const std::unique_ptr<Node>& node);
};
