#pragma once

#include "Symbol.hpp"
#include <string>
#include <queue>
#include <stack>

bool needsConcat(const Symbol& a, const Symbol& b);
std::string concatAll(const std::string& infix);
std::string toPostfix(const std::string& infix);
void handleLiteral(const Symbol& current, std::queue<Symbol>& output);
void handleOperator(const Symbol& current, std::queue<Symbol>& output, std::stack<Symbol>& operators);
void handleLeftParenthesis(const Symbol& current, std::stack<Symbol>& operators);
void handleRightParenthesis(const Symbol& current, std::stack<Symbol>& operators, std::queue<Symbol>& output);
