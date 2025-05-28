#include "RegEx.hpp"
#include <iostream>

int main()
{
	RegEx example("ab*ab*a", RegExNotation::Infix);

	NFA nfa = example.toNFA();
	nfa.test("aaba"); // Accepted
	nfa.test("abbbbabbba"); // Accepted
	nfa.test("abaab"); // Denied
	nfa.test("bbabaab"); // Denied
	
	return 0;
}
