#pragma once

#include "Alphabet.hpp"
#include "Transitions.hpp"
#include "States.hpp"
#include "RegExString.hpp"

class EFA
{
public:
	EFA(const Alphabet& alphabet, const States& states, const State& initial, const States& accept, const NFATransitions& transitions);
	
	MutableRegExString toMutRegex();
private:
	Alphabet alphabet;
	States states;
	State initial;
	States accept;
	EFATransitions transitions;

	void addExplicitUnion();
	void addExplicitUnion(const State& s1, const State& s2);
	void addNewInitialState();
	State retrieveNewState() const;
	void addNewAcceptState();

	void eliminateNonTerminalStates();
	void tryEliminating(const State& s);

	bool transitionsExistTo(const State& s) const;
	bool transitionsLeaveFrom(const State& s) const;
};
