#pragma once

#include "Automaton.hpp"
#include "Transitions.hpp"
#include "State.hpp"
#include "States.hpp"
#include "Alphabet.hpp"
#include "Word.hpp"
#include "OutputSequence.hpp"
#include <initializer_list>
#include <set>

class MealyMachine : public OutputAutomaton
{
public:
	MealyMachine(std::initializer_list<DFATransition> inputTransitions, std::initializer_list<OutputTransition> outputTransitions, const State& initial);
	MealyMachine(const std::set<DFATransition>& inputTransitions, const std::set<OutputTransition>& outputTransitions, const State& initial);

	SymbolOutput run(Word word) const override;

	friend std::ostream& operator<<(std::ostream& os, const MealyMachine mealy)
	{
		os << "Defined states: " << mealy.states << '\n';
		os << "Initial state: " << mealy.initial << '\n';
		os << "Input transition table:\n";
		os << mealy.inputTransitions;
		os << "Output transition table:\n";
		os << mealy.outputTransitions;
		return os;
	}
private:
	States states;
	Alphabet inputAlphabet;
	DFATransitions inputTransitions;
	State initial;
	Alphabet outputAlphabet;
	OutputTransitions outputTransitions;
};
