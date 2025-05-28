#include "PDA.hpp"

#include "PDAPair.hpp"

PDA::PDA(std::initializer_list<PDATransition> transitions, const State& initialState, const Symbol& initialSymbol, const States& accept) : PDA(std::set<PDATransition>(transitions), initialState, initialSymbol, accept) {}

PDA::PDA(const std::set<PDATransition>& transitions, const State& initialState, const Symbol& initialSymbol, const States& accept) : transitions(transitions), symbolStack(initialSymbol), description(initialState, symbolStack), accept(accept)
{
	states.add(initialState);
	States::const_iterator sit;
	for (sit = accept.begin(); sit != accept.end(); ++sit)
	{
		states.add(*sit);
	}
	std::set<PDATransition>::const_iterator trit;
	for (trit = transitions.begin(); trit != transitions.end(); ++trit)
	{
		PDAPair from = trit->from;
		states.add(from.state);
		inputAlphabet.add(trit->symbol);
		PDAPairs to = trit->to;
		PDAPairs::const_iterator it;
		for (it = to.begin(); it != to.end(); ++it)
		{
			states.add(it->state);
			// Exception handling and conditions.
			if (from.stackOp.size() <= 0 || from.stackOp.size() > 2)
				throw std::runtime_error("Invalid stack operation! Exceeded symbol sequence length in stack operation expressions!");
			if (it->stackOp.size() == 1 && it->stackOp.at(0) != Symbol::lambda())
				throw std::runtime_error("Invalid stack operation! Single symbol cannot turn to non-lambda symbol!");
			if (it->stackOp.size() == 2 && (it->stackOp.at(1) != from.stackOp.at(0)))
				throw std::runtime_error("Invalid stack operation! Push expression does not match top of the stack expression!");
			std::vector<Symbol>::const_iterator strit;
			for (strit = it->stackOp.begin(); strit != it->stackOp.end(); ++strit)
			{
				pushdownAlphabet.add(*strit);
			}
		}
	}
}

AcceptCode PDA::accepts(Word word) const
{
	if (!word.definedOver(inputAlphabet))
	{
		return AcceptCode::Abort;
	}

	PushdownIDs currentIDs = calculateLambdaClosure({description});
	while (word.size() > 0)
	{
		Symbol currentSymbol = word.processSymbol();
		bool transitionExists = false;
		PushdownIDs nextIDs;
		PushdownIDs::const_iterator it;
		for (it = currentIDs.begin(); it != currentIDs.end(); ++it)
		{
			PDAPair testPair(it->state, it->symbolStack.top());
			if (!transitions.has(testPair, currentSymbol))
				continue;
			PDAPairs nextPairs = transitions.get(testPair, currentSymbol);
			PushdownIDs traversed = getTransitionedPIDs(*it, nextPairs);
			nextIDs.unite(traversed);
			transitionExists = true;
		}


		if (!transitionExists)
		{
			return AcceptCode::Abort;
		}

		currentIDs = calculateLambdaClosure(nextIDs);
	}

	PushdownIDs::const_iterator it;
	for (it = currentIDs.begin(); it != currentIDs.end(); ++it)
	{
		if (accept.contains(it->state) || it->symbolStack.empty())
			return AcceptCode::Accepted;
	}
	return AcceptCode::Denied;
}

PushdownIDs PDA::calculateLambdaClosure(const PushdownIDs& pushdownIDs) const
{
	PushdownIDs result;
	PushdownIDs::const_iterator it;
	for (it = pushdownIDs.begin(); it != pushdownIDs.end(); ++it)
	{
		tryLambda(*it, result);
	}
	return result;
}

void PDA::tryLambda(const PushdownID& pushdownID, PushdownIDs& closure) const
{
	// Tries to reach all unvisited states which can be reached by lambda transitions that also have a valid stack operation.
	if (closure.contains(pushdownID))
		return;

	closure.add(pushdownID);

	// Handles the case where no lambda transition exists.
	PDAPair testPair(pushdownID.state, pushdownID.symbolStack.top());
	if (!transitions.has(testPair, Symbol::lambda()))
		return;

	PDAPairs resultPairs = transitions.get(testPair, Symbol::lambda());
	PushdownIDs nextIDs = getTransitionedPIDs(pushdownID, resultPairs);

	PushdownIDs::const_iterator pit;
	for (pit = nextIDs.begin(); pit != nextIDs.end(); ++pit)
	{
		tryLambda(*pit, closure);
	}
}

PushdownID PDA::getTransitionedPID(const PushdownID& pushdownID, const PDAPair& pair) const
{
	PushdownID resultID = pushdownID;
	resultID.state = pair.state;
	if (pair.stackOp.at(0) == Symbol::lambda())
	{
		resultID.symbolStack.pop();
	}
	else if (pair.stackOp.size() == 2 && pair.stackOp.at(1) == pushdownID.symbolStack.top())
	{
		resultID.symbolStack.push(pair.stackOp.at(0));
	}
	return resultID;
}

PushdownIDs PDA::getTransitionedPIDs(const PushdownID& pushdownID, const PDAPairs& pairs) const
{
	PushdownIDs nextIDs;
	PDAPairs::const_iterator it;
	for (it = pairs.begin(); it != pairs.end(); ++it)
	{
		PushdownID nextID = getTransitionedPID(pushdownID, *it);
		nextIDs.add(nextID);
	}
	return nextIDs;
}
