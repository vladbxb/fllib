#include "Transitions.hpp"
#include "State.hpp"
#include "Symbol.hpp"
#include <iostream>

int main()
{
	DFATransitions transitions = {
		{ "q0", '0', "q1" },
		{ "q0", '1', "q2" }
	};

	NFATransitions ntransitions = {
		{ "q0", 'a', {"q1", "q0"} },
		{ "q1", 'a', {"q1"} },
		{ "q1", 'b', {"q2"} }
	};

	std::cout << "DFA transitions:\n";
	std::cout << transitions;
	std::cout << "\nNFA transitions\n";
	std::cout << ntransitions;
	std::cout << '\n';

	std::set<NFATransitions> t;
	NFATransition t1 = {"q0", Symbol::lambda(), {"q1"} };

	return 0;
}
