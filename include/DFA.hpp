#pragma once

#include "States.hpp"
#include "Alphabet.hpp"
#include "State.hpp"
#include "Transitions.hpp"
#include "Automaton.hpp"

#include <iostream>

class DFA : public Automaton
{
public:
	DFA(std::initializer_list<DFATransition> transitions, const State& initial, const States& accept);
	DFA(const std::set<DFATransition>& transitions, const State& initial, const States& accept);

	AcceptCode accepts(Word word) const override;

	friend std::ostream& operator<<(std::ostream& os, const DFA& dfa)
	{
		os << "Defined states: " << dfa.states << '\n';
		os << "Initial state: " << dfa.initial << '\n';
		os << "Accept states: " << dfa.accept << '\n';
		os << "Transition table:\n";
		os << dfa.transitions;
		return os;
	}
private:
	Alphabet alphabet;
	States states;
	State initial;
	States accept;
	DFATransitions transitions;
};
