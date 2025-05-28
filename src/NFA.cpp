#include "NFA.hpp"
#include "StateMapper.hpp"

#include <queue>
NFA::NFA(std::initializer_list<NFATransition> transitions, const State& initial, const States& accept) : NFA(std::set<NFATransition>(transitions), initial, accept) {}

NFA::NFA(const std::set<NFATransition>& transitions, const State& initial, const States& accept) : transitions(transitions), initial(initial), accept(accept)
{
	// Add initial and accepting states to the NFA's defined states.
	states.add(initial);
	States::const_iterator it;
	for (it = accept.begin(); it != accept.end(); ++it)
		states.add(*it);

	std::set<NFATransition>::const_iterator trit;
	for (trit = transitions.begin(); trit != transitions.end(); ++trit)
	{
		// State from = trit->first.first;
		// Symbol symbol = trit->first.second;
		// States to = trit->second;
		states.add(trit->from);
		// Add every resulting state to the NFA's states.
		states.add(trit->to);
		alphabet.add(trit->symbol);
	}
}

NFA::NFA(const NFATransitions& transitions, const State& initial, const States& accept) : transitions(transitions), initial(initial), accept(accept)
{
	// Add initial and accepting states to the NFA's defined states.
	states.add(initial);
	States::const_iterator it;
	for (it = accept.begin(); it != accept.end(); ++it)
		states.add(*it);

	NFATransitions::const_iterator trit;
	for (trit = transitions.begin(); trit != transitions.end(); ++trit)
	{
		State from = trit->first.first;
		Symbol symbol = trit->first.second;
		States to = trit->second;
		states.add(from);
		// Add every resulting state to the NFA's states.
		states.add(to);
		alphabet.add(symbol);
	}
}

void NFA::tryLambda(const State& state, States& closure) const
{
	// Tries to reach all unvisited states which can be reached by lambda.
	if (closure.contains(state))
		return;

	closure.add(state);

	// Handles the case where the transition doesn't exist.
	if (!transitions.has(state, Symbol::lambda()))
		return;

	States nextStates = transitions.get(state, Symbol::lambda());
	States::const_iterator it;
	for (it = nextStates.begin(); it != nextStates.end(); ++it)
	{
		tryLambda(*it, closure);
	}
}


States NFA::calculateLambdaClosure(const States& states) const
{
	// Calculates the lambda closure of multiple states, and returns the union.
	States result;
	States::const_iterator it;
	for (it = states.begin(); it != states.end(); ++it)
	{
		tryLambda(*it, result);
	}	
	return result;
}

States NFA::calculateLambdaClosure(const State& state) const
{
	// Calculates the lambda closure of a single State.
	States result;
	tryLambda(state, result);
	return result;
}

std::set<NFATransition> NFA::transitionsWithoutLambda() const
{
	// Returns lambda eliminated transitions.
	std::set<NFATransition> newTransitions;
	States::const_iterator sit;

	for (sit = states.begin(); sit != states.end(); ++sit)
	{
		Alphabet::const_iterator ait;
		for (ait = alphabet.begin(); ait != alphabet.end(); ++ait)
		{
			States currentElimination = eliminateLambda(*sit, *ait);
			if (!currentElimination.empty())
			newTransitions.insert(NFATransition(*sit, *ait, currentElimination));
		}
	}
	
	return newTransitions;
}

States NFA::eliminateLambda(State state, Symbol symbol) const
{
	States firstClosure = calculateLambdaClosure(state);
	States symbolResults;
	States::const_iterator it;
	for (it = firstClosure.begin(); it != firstClosure.end(); ++it)
	{
		if (!transitions.has(*it, symbol))
			continue;
		States foundStates = transitions.get(*it, symbol);
		symbolResults.add(foundStates);
	}
	States secondClosure = calculateLambdaClosure(symbolResults);
	return secondClosure;
}

States NFA::calculateNewAcceptStates(const States& definedStates) const
{
	// Returns states which, when calculating their closures, contain at least an accept state.
	States newAccept;
	States::const_iterator sit;
	for (sit = definedStates.begin(); sit != definedStates.end(); ++sit)
	{
		States currentClosure = calculateLambdaClosure(*sit);
		States::const_iterator accit;
		for (accit = this->accept.begin(); accit != this->accept.end(); ++accit)
		{
			if (currentClosure.contains(*accit))
			{
				newAccept.add(*sit);
				break;
			}
		}
	}
	return newAccept;
}

std::set<States> NFA::calculateNewAcceptStates(const std::set<States>& definedStates) const
{
	std::set<States> newAcceptStates;

	std::set<States>::const_iterator dit;
	for (dit = definedStates.begin(); dit != definedStates.end(); ++dit)
	{
		States::const_iterator sit;
		for (sit = dit->begin(); sit != dit->end(); ++sit)
		{
			if (accept.contains(*sit))	
			{
				newAcceptStates.insert(*dit);
				break;
			}
		}
	}

	return newAcceptStates;
}

NFA NFA::convertToRegularNFA() const
{
	States newAccept = calculateNewAcceptStates(states);

	std::set<NFATransition> newTransitions = transitionsWithoutLambda();

	return NFA(newTransitions, initial, newAccept);
}

DFA NFA::convertToDFA() const
{
	NFA r = convertToRegularNFA();

	std::set<States> newStates;
	States newInitialState = calculateLambdaClosure(r.initial);
	
	StateMapper IDs;
	IDs.add(newInitialState);

	std::set<DFATransition> newTransitions;

	std::queue<States> unprocessedStates;
	unprocessedStates.push(newInitialState);

	while (!unprocessedStates.empty())
	{
		States currentStates = unprocessedStates.front();
		unprocessedStates.pop();
		if (newStates.find(currentStates) != newStates.end())
			continue;
		newStates.insert(currentStates);
		IDs.add(currentStates);

		Alphabet::const_iterator ait;
		for (ait = r.alphabet.begin(); ait != r.alphabet.end(); ++ait)
		{
			States SSCresult;

			States::const_iterator sit;
			for (sit = currentStates.begin(); sit != currentStates.end(); ++sit)
			{
				if (!r.transitions.has(*sit, *ait))
					continue;

				States currentResult = r.transitions.get(*sit, *ait);
				SSCresult.add(currentResult);
			}

			if (newStates.find(SSCresult) == newStates.end() && !IDs.has(SSCresult))
			{
				IDs.add(SSCresult);
				unprocessedStates.push(SSCresult);
			}

			newTransitions.insert(DFATransition(IDs.get(currentStates), *ait, IDs.get(SSCresult)));
		}
	}

	std::set<States> newAcceptStates = calculateNewAcceptStates(newStates);
	States acceptIDs;
	std::set<States>::const_iterator accit;
	for (accit = newAcceptStates.begin(); accit != newAcceptStates.end(); ++accit)
	{
		State currentAcceptState = IDs.get(*accit);
		acceptIDs.add(currentAcceptState);
	}


	return DFA(newTransitions, IDs.get(newInitialState), acceptIDs);
}



AcceptCode NFA::accepts(Word word) const
{
	if (!word.definedOver(alphabet))
	{
		return AcceptCode::Abort;
	}

	States currentStates = calculateLambdaClosure({initial});
	while (word.size() > 0)
	{
		Symbol currentSymbol = word.processSymbol();
		bool transitionExists = false;
		States nextStates;
		States::const_iterator it;
		for (it = currentStates.begin(); it != currentStates.end(); ++it)
		{
			if (!transitions.has(*it, currentSymbol))
				continue;
			States traversed = transitions.get(*it, currentSymbol);
			nextStates.unite(traversed);
			transitionExists = true;
		}

		if (!transitionExists)
		{
			return AcceptCode::Abort;
		}

		currentStates = calculateLambdaClosure(nextStates);
	}
	
	States::const_iterator it;	
	for (it = currentStates.begin(); it != currentStates.end(); ++it)
	{
		if (!accept.contains(*it))
			continue;
		return AcceptCode::Accepted;
	}
	return AcceptCode::Denied;
}
