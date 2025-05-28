#include "NFA.hpp"
#include <iostream>

int main()
{
	NFA nfa({
			{"q0", '0', {"q0", "q1"}},
			{"q0", '1', {"q0", "q2"}},
			{"q1", '1', {"q3"}},
			{"q2", '0', {"q4"}}
		},
		"q0",
		{"q3", "q4"}
	);

	nfa.test("0001"); // Accepted
	nfa.test("11101010"); // Accepted
	nfa.test("000011000111"); // Denied
	nfa.test("11100100"); // Denied
	return 0;
}
