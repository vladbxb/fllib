#pragma once

#include "State.hpp"
#include "States.hpp"
#include "Alphabet.hpp"
#include "Word.hpp"
#include "Symbol.hpp"
#include "Transitions.hpp"
#include "SymbolStack.hpp"
#include "Automaton.hpp"
#include "PushdownID.hpp"
#include "PushdownIDs.hpp"
#include <initializer_list>
#include <set>


class PDA : public Automaton
{
public:
	PDA(std::initializer_list<PDATransition> transitions, const State& initialState, const Symbol& initialSymbol, const States& accept);
	PDA(const std::set<PDATransition>& transitions, const State& initialState, const Symbol& initialSymbol, const States& accept);
	AcceptCode accepts(Word word) const override;
private:
	States states;
	Alphabet inputAlphabet;
	Alphabet pushdownAlphabet;
	SymbolStack symbolStack;
	PushdownID description;
	States accept;
	PDATransitions transitions;

	PushdownIDs calculateLambdaClosure(const PushdownIDs& pushdownIDs) const;
	void tryLambda(const PushdownID& pushdownID, PushdownIDs& closure) const;
	PushdownID getTransitionedPID(const PushdownID& pushdownID, const PDAPair& pair) const;
	PushdownIDs getTransitionedPIDs(const PushdownID& pushdownID, const PDAPairs& pairs) const;
};
