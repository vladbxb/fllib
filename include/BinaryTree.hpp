#pragma once

template <typename T>
struct Node
{
	Node() : left(nullptr), right(nullptr) {}
	Node(const T& value) : left(nullptr), right(nullptr), value(value) {}

	Node* left;
	Node* right;
	T value;
}

template <typename T>
class BinaryTree
{
public:

