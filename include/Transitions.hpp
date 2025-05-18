#pragma once

#include "State.hpp"
#include "States.hpp"
#include "Symbol.hpp"
#include <utility>
#include <map>
#include <optional>

template <typename F, typename S, typename T>
struct Transition
{
	F from;
	S symbol;
	T to;

	Transition(const F& from, const S& symbol, const T& to) : from(from), symbol(symbol), to(to) {}

	template <typename F2, typename S2, typename T2>
	Transition(const Transition<F2, S2, T2>& other) : from(F(other.from)), symbol(S(other.symbol)), to(T(other.to)) {}


	bool operator<(const Transition& other) const
	{
		if (from != other.from)
			return from < other.from;
		if (symbol != other.symbol)
			return symbol < other.symbol;
		return to < other.to;
	}

	friend std::ostream& operator<<(std::ostream& os, const Transition<F, S, T>& transition)
	{
		os << transition.from << " --" << transition.symbol << "--> " << transition.to;
		return os;
	}
};

using DFATransition = Transition<State, Symbol, State>;
using NFATransition = Transition<State, Symbol, States>;
using SSCTransition = Transition<States, Symbol, States>;

template <typename F, typename S, typename T>
class Transitions
{
public:
	using Key = std::pair<F, S>;
	using iterator = typename std::map<Key, T>::iterator;
	using const_iterator = typename std::map<Key, T>::const_iterator;

	iterator begin() { return transitions.begin(); }
	iterator end() { return transitions.end(); }

	const_iterator begin() const { return transitions.begin(); }
	const_iterator end() const { return transitions.end(); }

	Transitions(const std::set<Transition<F, S, T>>& transitions);
	Transitions(std::initializer_list<Transition<F, S, T>> transitions);

	template <typename F2, typename S2, typename T2>
	Transitions(const Transitions<F2, S2, T2>& other)
	{
		// Constructor for trying implicit conversion of the typenames of another Transitions object to its own typenames.
		typename Transitions<F2, S2, T2>::const_iterator it;
		for (it = other.begin(); it != other.end(); ++it)
		{
			// Cast each type for easy conversion
			add(static_cast<F>(it->first.first), static_cast<S>(it->first.second), static_cast<T>(it->second));
		}
	}

	void add(const F& from, const S& symbol, const T& to);
	void add(const Transition<F, S, T>& transition);
	bool has(const F& from, const S& symbol) const;
	T get(const F& from, const S& symbol) const;

	friend std::ostream& operator<<(std::ostream& os, const Transitions<F, S, T>& transitions)
	{
		Transitions<F, S, T>::const_iterator it;
		for (it = transitions.begin(); it != transitions.end(); ++it)
		{
			F from = it->first.first;
			S symbol = it->first.second;
			T to = it->second;
			Transition<F, S, T> currentTransition(from, symbol, to);
			os << currentTransition << '\n';
		}
		return os;
	}
			
private:
	std::map<Key, T> transitions;
};

template <typename F, typename S, typename T>
Transitions<F, S, T>::Transitions(const std::set<Transition<F, S, T>>& transitions)
{
	// Builds the transition table based on a set of Transition objects.
	typename std::set<Transition<F, S, T>>::const_iterator it;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		this->add(*it);
	}
}

template <typename F, typename S, typename T>
Transitions<F, S, T>::Transitions(std::initializer_list<Transition<F, S, T>> transitions)
{
	// Builds the transition table based on an initializer list.
	typename std::initializer_list<Transition<F, S, T>>::const_iterator it;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		this->add(*it);
	}
}

template <typename F, typename S, typename T>
void Transitions<F, S, T>::add(const F& from, const S& symbol, const T& to)
{
	// Adds a transition based on the 3 needed objects.
	Key k = std::make_pair(from, symbol);
	transitions.emplace(k, to);
}

template <typename F, typename S, typename T>
void Transitions<F, S, T>::add(const Transition<F, S, T>& transition)
{
	// Adds a transition based on a Transition object.
	Key k = std::make_pair(transition.from, transition.symbol);
	transitions.emplace(k, transition.to);
}

template <typename F, typename S, typename T>
bool Transitions<F, S, T>::has(const F& from, const S& symbol) const
{
	// Checks if the result of a key exists in the map.
	Key k = std::make_pair(from, symbol);
	auto it = transitions.find(k);
	return it != transitions.end();
}

template <typename F, typename S, typename T>
T Transitions<F, S, T>::get(const F& from, const S& symbol) const
{
	// Gets the result of the key pair in the map.
	Key k = std::make_pair(from, symbol);
	return transitions.at(k);
}

// Aliases for certain automata / constructions

using DFATransitions = Transitions<State, Symbol, State>;
using NFATransitions = Transitions<State, Symbol, States>;
using SSCTransitions = Transitions<States, Symbol, States>;
