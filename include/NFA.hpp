#pragma once

#include "States.hpp"
#include "Alphabet.hpp"
#include "Transitions.hpp"
#include "Automaton.hpp"

#include "DFA.hpp"

class RegEx;

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

	RegEx convertToRegEx() const;
	NFA convertToRegularNFA() const;
	DFA convertToDFA() const;

	AcceptCode accepts(Word word) const override;

	friend std::ostream& operator<<(std::ostream& os, const NFA& a)
	{
		os << "Defined states: " << a.states << '\n';
		os << "Initial state: " << a.initial << '\n';
		os << "Accept states: " << a.accept << '\n';
		os << "Transition table:\n";
		os << a.transitions;
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

#include "EFA.hpp"
#include "RegEx.hpp"

inline RegEx NFA::convertToRegEx() const
{
	EFA e(alphabet, states, initial, accept, transitions);
	RegEx converted(e.toMutRegex());
	converted.simplify();
	return converted;
}

