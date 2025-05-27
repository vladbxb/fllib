#pragma once

#include "Automaton.hpp"
#include "Word.hpp"
#include "Transitions.hpp"
#include "State.hpp"
#include "Symbol.hpp"
#include "States.hpp"
#include "OutputMap.hpp"
#include <ostream>

class MooreMachine : public OutputAutomaton
{
public:
	MooreMachine(std::initializer_list<DFATransition> transitions, std::initializer_list<std::pair<State, Symbol>> outputMap, const State& initial);
	MooreMachine(const std::set<DFATransition>& transitions, const std::set<std::pair<State, Symbol>>& outputMap, const State& initial);

	SymbolOutput run(Word word) const override;

	friend std::ostream& operator<<(std::ostream& os, const MooreMachine& moore)
	{
		os << "Defined states: " << moore.states << '\n';
		os << "Initial state: " << moore.initial << '\n';
		os << "Input transition table:\n";
		os << moore.transitions;
		os << "Output map:\n";
		os << moore.outputMap;
		return os;
	}
private:
	States states;
	Alphabet inputAlphabet;
	Alphabet outputAlphabet;
	DFATransitions transitions;
	OutputMap outputMap;
	State initial;
};
