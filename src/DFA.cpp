#include "DFA.hpp"

DFA::DFA(std::initializer_list<DFATransition> transitions, const State& initial, const States& accept) : DFA(std::set<DFATransition>(transitions), initial, accept) {}

DFA::DFA(const std::set<DFATransition>& transitions, const State& initial, const States& accept) : transitions(transitions), initial(initial), accept(accept)
{
	states.add(initial);
	States::const_iterator it;
	for (it = accept.begin(); it != accept.end(); ++it)
		states.add(*it);

	std::set<DFATransition>::const_iterator trit;
	for (trit = transitions.begin(); trit != transitions.end(); ++trit)
	{
		states.add(trit->from);
		states.add(trit->to);
		alphabet.add(trit->symbol);
	}
}

AcceptCode DFA::accepts(Word word) const
{
	if (!word.definedOver(alphabet))
		return AcceptCode::Abort;
	State currentState = initial;
	while (std::optional<Symbol> currentSymbol = word.processSymbol())
	{
		std::optional<State> nextState = transitions.get(currentState, *currentSymbol);
		if (!nextState)
			return AcceptCode::Abort;
		currentState = *nextState;
	}

	if (accept.contains(currentState))
		return AcceptCode::Accepted;
	return AcceptCode::Denied;
}
