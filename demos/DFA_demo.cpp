#include "DFA.hpp"
#include <iostream>

int main()
{
	DFA dfa({
			{"q0", '0', "q2"},
			{"q0", '1', "q1"},
			{"q1", '0', "q3"},
			{"q1", '1', "q0"},
			{"q2", '0', "q0"},
			{"q2", '1', "q3"},
			{"q3", '0', "q1"},
			{"q3", '1', "q2"}
		},
		"q0",
		{"q1"}
	);
	dfa.test("00111"); // Accepted
	dfa.test("110"); // Denied
	return 0;
}
