#pragma once

#include "States.hpp"
#include "Alphabet.hpp"
#include "Transitions.hpp"
#include "Automaton.hpp"

#include "DFA.hpp"

#include <iostream>

struct NFAFragment
{
	NFATransitions transitions;
	State initial;
	States accept;
};


class NFA : public Automaton
{
public:
	NFA(std::initializer_list<NFATransition> transitions, const State& initial, const States& accept);
	NFA(const std::set<NFATransition>& transitions, const State& initial, const States& accept);
	NFA(const NFATransitions& transitions, const State& initial, const States& accept);
	NFA(const NFAFragment& fragment) : NFA(fragment.transitions, fragment.initial, fragment.accept) {}

	void tryLambda(const State& state, States& closure) const;
	States calculateLambdaClosure(const State& state) const;
	States calculateLambdaClosure(const States& states) const;

	NFA convertToRegularNFA() const;
	DFA convertToDFA() const;

	AcceptCode accepts(Word word) const override;

	friend std::ostream& operator<<(std::ostream& os, const NFA& nfa)
	{
		os << "Defined states: " << nfa.states << '\n';
		os << "Initial state: " << nfa.initial << '\n';
		os << "Accept states: " << nfa.accept << '\n';
		os << "Transition table:\n";
		os << nfa.transitions;
		return os;
	}
private:
	Alphabet alphabet;
	States states;
	State initial;
	States accept;
	NFATransitions transitions;

	// Helper methods for Regular NFA and DFA conversion.
	std::set<NFATransition> transitionsWithoutLambda() const;
	States eliminateLambda(State state, Symbol symbol) const;
	States calculateNewAcceptStates(const States& definedStates) const;
	std::set<States> calculateNewAcceptStates(const std::set<States>& definedStates) const;
};
