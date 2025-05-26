#include "RegExSyntaxTree.hpp"

RegExSyntaxTree::RegExSyntaxTree(const RegExString& postfix)
{
	// Build the tree from postfix notation.
	build(postfix);
	// Simplify by starting at the root node.
	root = simplify(std::move(root));
}

std::string RegExSyntaxTree::stringifyRoot()
{
	return stringify(root);
}

void RegExSyntaxTree::build(const RegExString& postfix)
{
	std::stack<std::unique_ptr<Node>> nodeStack;
	RegExString::const_iterator it;
	for (it = postfix.begin(); it != postfix.end(); ++it)
	{
		if (it->isLiteral())
			nodeStack.push(std::make_unique<Node>(*it));
		else
		if (it->isUnaryOperator())
		{
			std::unique_ptr<Node> child = std::move(nodeStack.top());
			nodeStack.pop();
			std::unique_ptr<Node> op = std::make_unique<Node>(*it);
			op->left = std::move(child);
			nodeStack.push(std::move(op));
		}
		else
		if (it->isOperator())
		{
			std::unique_ptr<Node> right = std::move(nodeStack.top());
			nodeStack.pop();
			std::unique_ptr<Node> left = std::move(nodeStack.top());
			nodeStack.pop();
			std::unique_ptr<Node> op = std::make_unique<Node>(*it);
			op->right = std::move(right);
			op->left = std::move(left);
			nodeStack.push(std::move(op));
		}
	}

	std::unique_ptr<Node> result = std::move(nodeStack.top());
	nodeStack.pop();
	root = std::move(result);
}

std::unique_ptr<Node> RegExSyntaxTree::simplify(std::unique_ptr<Node> node)
{
	if (node == nullptr)
		return nullptr;
	node->left = simplify(std::move(node->left));
	node->right = simplify(std::move(node->right));
	if (node->symbol.isUnion())
	{
		// If subtrees are structurally equal, remove one of them (in case of union).
		if (structurallyEqual(node->left, node->right))
			return std::move(node->left);
		// Attempt to make subtrees have commutative notation by lexicographic order.
		if (stringify(node->left) > stringify(node->right))
			std::swap(node->left, node->right);
	}

	return node;
}

bool RegExSyntaxTree::structurallyEqual(const std::unique_ptr<Node>& left, const std::unique_ptr<Node>& right)
{
	if (left == nullptr && right == nullptr)
		return true;
	if (left == nullptr || right == nullptr)
		return false;
	if (left->symbol != right->symbol)
		return false;
	// If the leaf is a literal, by the above checks we ensure that the other symbol is also a literal
	if (left->symbol.isLiteral())
		return true;
	
	return structurallyEqual(left->left, right->left) && structurallyEqual(left->right, right->right);
}

std::string RegExSyntaxTree::stringify(const std::unique_ptr<Node>& node)
{
	if (node == nullptr)
		return "";
	if (node->symbol.isLiteral())
	{
		std::string result;
		result.push_back(node->symbol.getSymbol());
		return result;
	}
	if (node->symbol.isStar())
	{
		std::string result;
		result.push_back(Symbol::leftParenthesis().getSymbol());
		result.append(stringify(node->left));
		result.push_back(Symbol::rightParenthesis().getSymbol());
		result.push_back(Symbol::star().getSymbol());
		return result;
	}
	if (node->symbol.isConcat())
	{
		std::string result;
		//result.push_back(Symbol::leftParenthesis().getSymbol());
		result.append(stringify(node->left));
		//result.push_back(Symbol::concat().getSymbol());
		result.append(stringify(node->right));
		//result.push_back(Symbol::rightParenthesis().getSymbol());
		return result;
	}
	if (node->symbol.isUnion())
	{
		std::string result;
		result.push_back(Symbol::leftParenthesis().getSymbol());
		result.append(stringify(node->left));
		result.push_back(Symbol::_union().getSymbol());
		result.append(stringify(node->right));
		result.push_back(Symbol::rightParenthesis().getSymbol());
		return result;
	}
	return "";
}
