#include "StateMapper.hpp"
#include "States.hpp"
#include "Transitions.hpp"

#include <iostream>

int main()
{
	StateMapper sm;
	States s1 = {"q0", "q1", "q2"};
	sm.add(s1);
	States s2 = {"q2", "q3", "q4", "q5", "q6"};
	sm.add(s2);

	if (sm.has(s1))
		std::cout << "s1:\n" << sm.get(s1) << '\n';
	if (sm.has(s2))
		std::cout << "s2:\n" << sm.get(s2) << '\n';

	DFATransition dt(sm.get(s1), 'a', sm.get(s2));
	std::cout << dt << '\n';

	return 0;
}
