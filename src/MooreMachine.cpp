#include "MooreMachine.hpp"

MooreMachine::MooreMachine(std::initializer_list<DFATransition> transitions, std::initializer_list<std::pair<State, Symbol>> outputMap, const State& initial) : MooreMachine(std::set<DFATransition>(transitions), std::set<std::pair<State,Symbol>>(outputMap), initial) {}

MooreMachine::MooreMachine(const std::set<DFATransition>& transitions, const std::set<std::pair<State, Symbol>>& outputMap, const State& initial) : transitions(transitions), outputMap(outputMap), initial(initial)
{
	states.add(initial);
	std::set<DFATransition>::const_iterator trit;
	for (trit = transitions.begin(); trit != transitions.end(); ++trit)
	{
		states.add(trit->from);
		states.add(trit->to);
		inputAlphabet.add(trit->symbol);
	}

	std::set<std::pair<State, Symbol>>::const_iterator oit;
	for (oit = outputMap.begin(); oit != outputMap.end(); ++oit)
	{
		if (!states.contains(oit->first))
			throw std::runtime_error("Mapped state to output not in input states!");
		outputAlphabet.add(oit->second);
	}
}

SymbolOutput MooreMachine::run(Word word) const
{
	if (!word.definedOver(inputAlphabet))
	{
		throw std::runtime_error("Input word is not defined over the input alphabet!\n");
	}
	State currentState = initial;
	SymbolOutput output;
	while (word.size() > 0)
	{
		Symbol currentSymbol = word.processSymbol();
		if (!transitions.has(currentState, currentSymbol))
			throw std::runtime_error("Transition for current symbol does not exist! Abort!");
		State nextState = transitions.get(currentState, currentSymbol);
		output.push_back(outputMap.getSymbol(nextState));
		currentState = nextState;
	}
	return output;
}
