#include "EFA.hpp"

#include "StateMapper.hpp"
#include <iostream>
#include <queue>

EFA::EFA(const Alphabet& alphabet, const States& states, const State& initial, const States& accept, const NFATransitions& transitions) : alphabet(alphabet), states(states), initial(initial), accept(accept), transitions(transitions) {}

MutableRegExString EFA::toMutRegex()
{
	// Returns the final regular expression achieved by removal of states
	addExplicitUnion();
	addNewInitialState();
	addNewAcceptState();
	eliminateNonTerminalStates();

    addExplicitUnion(initial, initial);
    addExplicitUnion(initial, *accept.begin());

	// Return first (and only) transition consisting of an unsimplified regular expression.
	return transitions.begin()->first.second;
}

void EFA::addExplicitUnion()
{
	// Applies the union algorithm on all possible pairs of states
	States::const_iterator it1;
	States::const_iterator it2;
	for (it1 = states.begin(); it1 != states.end(); ++it1)
	{
		for (it2 = states.begin(); it2 != states.end(); ++it2)
		{
			addExplicitUnion(*it1, *it2);
		}
	}
}

void EFA::addExplicitUnion(const State& s1, const State& s2)
{
	// Replaces multiple transitions with different symbols from s1 to s2 with a single one
	// that consists of the union of all of them, if such case exists
	EFATransitions::const_iterator it;
	std::map<std::pair<State, MutableRegExString>, States> foundTransitions;
	size_t found = 0;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		if (it->first.first == s1 && it->second.contains(s2))
		{
			// Increase found counter only if the insertion happened & do the insertion
			// (returns pair of which its second argument indicates insertion success)
			if (foundTransitions.emplace(it->first, it->second).second)
				++found;
		}
	}
	if (found <= 1)
		return;

	MutableRegExString symbolUnion;
	std::map<std::pair<State, MutableRegExString>, States> newTransitions;
	std::map<std::pair<State, MutableRegExString>, States>::const_iterator trit;
	for (trit = foundTransitions.begin(); trit != foundTransitions.end(); ++trit)
	{
		transitions.erase(trit->first);
		States result = trit->second;
		result.remove(s2);
		if (!result.empty())
			newTransitions[trit->first] = result;
		symbolUnion = symbolUnion._union(trit->first.second);
	}
	std::pair<State, MutableRegExString> newKey = std::make_pair(s1, symbolUnion);
	newTransitions[newKey] = s2;
	for (trit = newTransitions.begin(); trit != newTransitions.end(); ++trit)
		transitions.add(EFATransition(trit->first.first, trit->first.second, trit->second));
}	

void EFA::addNewInitialState()
{
	// Adds a new initial state to the old one, if needed.
	if (accept.contains(initial) || transitionsExistTo(initial))
	{
		State newInitialState = retrieveNewState();
		transitions.add(newInitialState, Symbol::lambda(), initial);
		states.add(newInitialState);
		initial = newInitialState;
	}
}

State EFA::retrieveNewState() const
{
	// Hacky way of getting a new unique state.
	// Ideally, State should've housed an ID instead of a string. But it's too late now.
	StateMapper reserver;
	State test = reserver.reserveState();
	while (states.contains(test))
	{
		test = reserver.reserveState();
	}
	return test;
}

void EFA::addNewAcceptState()
{
	bool multipleAcceptStates = accept.size() > 1;
	bool transitionsLeaveAccept = false;
	States::const_iterator it;
	for (it = accept.begin(); it != accept.end(); ++it)
	{
		if (transitionsLeaveFrom(*it))
		{
			transitionsLeaveAccept = true;
			break;
		}
	}
	if (multipleAcceptStates || transitionsLeaveAccept)
	{
		State newAcceptState = retrieveNewState();
		for (it = accept.begin(); it != accept.end(); ++it)
		{
			transitions.add(*it, Symbol::lambda(), newAcceptState);
		}
		states.add(newAcceptState);
		accept = newAcceptState;
	}
}

void EFA::eliminateNonTerminalStates()
{
	std::queue<State> toBeDeleted;
	States::const_iterator it;
	for (it = states.begin(); it != states.end(); ++it)
	{
		if (*it == initial || accept.contains(*it))
			continue;

		tryEliminating(*it);
		toBeDeleted.push(*it);
	}
	while (!toBeDeleted.empty())
	{
		states.remove(toBeDeleted.front());
		toBeDeleted.pop();
	}
}

// void EFA::tryEliminating(const State& q)
// {
// 	addExplicitUnion(q, q);
// 	EFATransitions in;
// 	EFATransitions out;
// 	MutableRegExString loopLabel;
// 	EFATransitions::const_iterator it;
// 	for (it = transitions.begin(); it != transitions.end(); ++it)
// 	{
// 		const State& from = it->first.first;
// 		const MutableRegExString& label = it->first.second;
// 		const States& to = it->second;
// 		if (from == q && to.size() == 1 && to.contains(q)) {
// 			loopLabel = loopLabel.empty()
// 					  ? label
// 					  : loopLabel._union(label);
// 		}
// 		else
// 		if (from == q)
// 			out.add(q, label, to);
// 		else
// 		if (to.contains(q))
// 			in.add(from, label, q);
// 	}
// 
// 	if (loopLabel.empty())
// 		loopLabel = Symbol::lambda();
// 
// 	// Register the direct transitions from p to s.
// 	EFATransitions newTransitions;
// 	EFATransitions::const_iterator it1;
// 	EFATransitions::const_iterator it2;
// 	States::const_iterator sit;
// 	for (it1 = in.begin(); it1 != in.end(); ++it1)
// 	{
// 		State p = it1->first.first;
// 		for (it2 = out.begin(); it2 != out.end(); ++it2)
// 		{
// 			States s = it2->second;
// 			for (sit = s.begin(); sit != s.end(); ++sit)
// 			{
// 				MutableRegExString inLabel = it1->first.second;
// 				MutableRegExString outLabel = it2->first.second;
// 				MutableRegExString oldDirect;
// 				for (it = transitions.begin(); it != transitions.end(); ++it)
// 				{
// 					if (it->first.first == p && it->second.contains(*sit))
// 					{
// 						oldDirect = it->first.second;
// 						break;
// 					}
// 				}
// 				if (oldDirect.empty())
// 					oldDirect = Symbol::_void();
// 				MutableRegExString newDirect = oldDirect._union(inLabel.concat(loopLabel.star()).concat(outLabel));
// 				newTransitions.add(p, newDirect, *sit);
// 			}
// 		}
// 	}
// 
// 	// for (it1 = in.begin(); it1 != in.end(); ++it1)
// 	// {
// 	// 	transitions.remove(it1->first.first, it1->first.second, q);
// 	// }
// 	// for (it2 = out.begin(); it2 != out.end(); ++it2)
// 	// {
// 	// 	for (sit = it2->second.begin(); sit != it2->second.end(); ++sit)
// 	// 		transitions.remove(q, it2->first.second, *sit);
// 	// }
// 	// if (!loopLabel.empty())
// 	// 	transitions.remove(q, loopLabel, q);
// 	
// 	std::vector<std::pair<State,MutableRegExString>> keys;
//     for (auto jt = transitions.begin(); jt != transitions.end(); ++jt) {
//         auto const& key   = jt->first;
//         auto const& dests = jt->second;
//         if (key.first == q || dests.contains(q)) {
//             keys.push_back(key);
//         }
//     }
//     for (auto const& k : keys) {
//         transitions.erase(k);
//     }
// 
// 	// Add the direct transitions after removing q.
// 	for (it = newTransitions.begin(); it != newTransitions.end(); ++it)
// 	{
// 		transitions.add(it->first.first, it->first.second, it->second);
// 	}
// }

// void EFA::tryEliminating(const State& q) {
//   // 1) Gather Rpq, Rqq, Rqs, and Rold for every (p,s)
//   struct Edge { State from, to; MutableRegExString label; };
//   std::vector<Edge>  incoming, outgoing;
//   MutableRegExString loopLabel = Symbol::_void();
// 
//   // First, collapse any existing q→q loops into loopLabel
//   for (auto const& [key, dests] : transitions) {
//     if (key.first==q && dests.contains(q))
//       loopLabel = loopLabel.empty()
//                 ? key.second
//                 : loopLabel._union(key.second);
//   }
//   if (loopLabel.empty())
//     loopLabel = Symbol::lambda();
// 
//   // Now collect *all* p→q edges and *all* q→s edges (split multis)
//   for (auto const& [key, dests] : transitions) {
//     State p = key.first;
//     auto  lbl = key.second;
//     for (State t : dests) {
//       if (p==q && t==q) continue;    // skip self‐loop (we already collapsed)
//       if (t==q) incoming.push_back({p, q, lbl});
//       if (p==q) outgoing.push_back({q, t, lbl});
//     }
//   }
// 
//   // 2) Build the new transition set:
//   EFATransitions rebuilt;
//   //   a) carry over every edge that doesn't touch q
//   for (auto const& [key, dests] : transitions) {
//     State p = key.first;
//     for (State t : dests) {
//       if (p!=q && t!=q)
//         rebuilt.add(p, key.second, t);
//     }
//   }
//   //   b) for each incoming×outgoing, compute Rnew(p→s)
//   for (auto const& inE : incoming) {
//     for (auto const& outE : outgoing) {
//       State p = inE.from;
//       State s = outE.to;
// 
//       // Rold = whatever remains in rebuilt for p→s
//       // MutableRegExString Rold = rebuilt.has(p,s)
//       //                         ? rebuilt.get(p,s)
//       //                         : Symbol::_void();
// 	  MutableRegExString Rold = Symbol::_void();
// 		for (auto jt = rebuilt.begin(); jt != rebuilt.end(); ++jt) {
// 			if (jt->first.first == p && jt->second.contains(s)) {
// 				Rold = jt->first.second;
// 				break;
// 			}
// 		}
// 
// 
//       // reroute = inE.label · (loopLabel)* · outE.label
//       auto reroute = inE.label.concat(loopLabel.star())
//                              .concat(outE.label);
// 
//       auto Rnew    = Rold._union(reroute);
//       rebuilt.add(p, Rnew, s);
//     }
//   }
// 
//   // 3) Swap it in
//   transitions = std::move(rebuilt);
// }

void EFA::tryEliminating(const State& q)
{
	EFATransitions incoming, outgoing;
	MutableRegExString loopLabel = Symbol::_void();
	EFATransitions::const_iterator trit;
	for (trit = transitions.begin(); trit != transitions.end(); ++trit)
	{
		// Accumulate all q -> q loops into a single label.
		if (trit->first.first == q && trit->second.contains(q))
			loopLabel = loopLabel.empty() ? trit->first.second : loopLabel._union(trit->first.second);
	}
	if (loopLabel.empty())
		loopLabel = Symbol::lambda();

	// Collect incoming and outcoming transitions in relation to q.
	for (trit = transitions.begin(); trit != transitions.end(); ++trit)
	{
		State p = trit->first.first;
		MutableRegExString label = trit->first.second;
		States to = trit->second;
		States::const_iterator sit;
		for (sit = to.begin(); sit != to.end(); ++sit)
		{
			State s = *sit;
			// All of the q loops have already been processed, so skip.
			if (p == q && q == s)
				continue;
			if (q == s)
				incoming.add(p, label, q);
			if (p == q)
				outgoing.add(q, label, s);
		}
	}

	EFATransitions newTransitions;
	// Insert transitions which do not touch q.
	for (trit = transitions.begin(); trit != transitions.end(); ++trit)
	{
		States::const_iterator sit;
		for (sit = trit->second.begin(); sit != trit->second.end(); ++sit)
		{
			if (trit->first.first != q && *sit != q)
				newTransitions.add(trit->first.first, trit->first.second, *sit);
		}
	}

	EFATransitions::const_iterator it1;
	EFATransitions::const_iterator it2;
	for (it1 = incoming.begin(); it1 != incoming.end(); ++it1)
	{
		for (it2 = outgoing.begin(); it2 != outgoing.end(); ++it2)
		{
			State p = it1->first.first;
			States::const_iterator sit;
			for (sit = it2->second.begin(); sit != it2->second.end(); ++sit)
			{
				MutableRegExString oldDirect = Symbol::_void();
				// Find the old transition p -> s.
				// It has already been inserted (it is not related to q).
				for (trit = newTransitions.begin(); trit != newTransitions.end(); ++trit)
				{
					if (trit->first.first == p && trit->second.contains(*sit))
					{
						oldDirect = trit->first.second;
						break;
					}
				}
				MutableRegExString partialNewDirect = it1->first.second.concat(loopLabel.star()).concat(it2->first.second);
				MutableRegExString newDirect = oldDirect._union(partialNewDirect);
				newTransitions.add(p, newDirect, *sit);
			}
		}
	}

	transitions = std::move(newTransitions);
}

bool EFA::transitionsExistTo(const State& s) const
{
	// Check if transitions exist to a certain state. Might move this to Transitions.hpp later.
	EFATransitions::const_iterator it;
	bool transitionsToState = false;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		if (it->second.contains(s))
		{
			transitionsToState = true;
			break;
		}
	}
	return transitionsToState;
}

bool EFA::transitionsLeaveFrom(const State& s) const
{
	// Check if transitions leave a certain state. Might move this to Transitions.hpp later.
	EFATransitions::const_iterator it;
	bool transitionsLeaveState = false;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		if (it->first.first == s)
		{
			transitionsLeaveState = true;
			break;
		}
	}
	return transitionsLeaveState;
}
