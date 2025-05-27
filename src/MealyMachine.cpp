#include "MealyMachine.hpp"

MealyMachine::MealyMachine(std::initializer_list<DFATransition> inputTransitions, std::initializer_list<OutputTransition> outputTransitions, const State& initial) : MealyMachine(std::set<DFATransition>(inputTransitions), std::set<OutputTransition>(outputTransitions), initial) {}

MealyMachine::MealyMachine(const std::set<DFATransition>& inputTransitions, const std::set<OutputTransition>& outputTransitions, const State& initial) : inputTransitions(inputTransitions), outputTransitions(outputTransitions), initial(initial)
{
	states.add(initial);
	std::set<DFATransition>::const_iterator itrit;
	std::set<OutputTransition>::const_iterator otrit;
	for (itrit = inputTransitions.begin(); itrit != inputTransitions.end(); ++itrit)
	{
		bool correspondingOutput = false;
		for (otrit = outputTransitions.begin(); otrit != outputTransitions.end(); ++otrit)
		{
			if (itrit->from == otrit->from && itrit->symbol == otrit->symbol)
			{
				correspondingOutput = true;
				break;
			}
		}
		if (!correspondingOutput)
			throw std::runtime_error("An output transition for every input transition does not exist!");

		states.add(itrit->from);
		states.add(itrit->to);
		inputAlphabet.add(itrit->symbol);
	}

	for (otrit = outputTransitions.begin(); otrit != outputTransitions.end(); ++otrit)
	{
		outputAlphabet.add(otrit->symbol);
	}
}

SymbolOutput MealyMachine::run(Word word) const
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
		if (!inputTransitions.has(currentState, currentSymbol))
			throw std::runtime_error("Transition for current symbol does not exist! Abort!");
		State nextState = inputTransitions.get(currentState, currentSymbol);
		Symbol nextOutput = outputTransitions.get(currentState, currentSymbol);
		output.push_back(nextOutput);
		currentState = nextState;
	}
	return output;
}
