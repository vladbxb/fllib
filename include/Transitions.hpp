#pragma once

#include "StateSet.hpp"
#include "State.hpp"
#include "States.hpp"
#include "Symbol.hpp"
#include "PDAPair.hpp"
#include "PDAPairs.hpp"
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

	bool operator==(const Transition& other) const
	{
		return from == other.from && symbol == other.symbol && to == other.to;
	}
	
	bool operator!=(const Transition& other) const
	{
		return !(*this == other);
	}

	friend std::ostream& operator<<(std::ostream& os, const Transition<F, S, T>& transition)
	{
		if (transition.symbol == Symbol::lambda())
			os << transition.from << " --" << std::string("λ") << "--> " << transition.to;
		else
			os << transition.from << " --" << transition.symbol << "--> " << transition.to;
		return os;
	}
};

using DFATransition = Transition<State, Symbol, State>;
using NFATransition = Transition<State, Symbol, States>;
using SSCTransition = Transition<States, Symbol, States>;
using OutputTransition = Transition<State, Symbol, Symbol>;
using PDATransition = Transition<PDAPair, Symbol, PDAPairs>;

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
	Transitions(const Transitions<F, S, T>& other);
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
	void add(const std::set<Transition<F, S, T>>& transitions);
	void add(const Transitions<F, S, T>& other);
	void add(std::initializer_list<Transition<F, S, T>> transitions);
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
	this->add(transitions);
}

template <typename F, typename S, typename T>
Transitions<F, S, T>::Transitions(const Transitions<F, S, T>& other)
{
	this->add(other);
}

template <typename F, typename S, typename T>
Transitions<F, S, T>::Transitions(std::initializer_list<Transition<F, S, T>> transitions)
{
	// Builds the transition table based on an initializer list.
	this->add(transitions);
}

template <typename F, typename S, typename T>
void Transitions<F, S, T>::add(const F& from, const S& symbol, const T& to)
{
	// Adds a transition based on the 3 needed objects.
	// Returns abruptly if the key already exists (there should be one to one correspondence)
	if (this->has(from, symbol))
		return;
	Key k = std::make_pair(from, symbol);
	transitions.emplace(k, to);
}

template <typename F, typename S, typename T>
void Transitions<F, S, T>::add(const Transition<F, S, T>& transition)
{
	// Adds a transition based on a Transition object.
	// Returns abruptly if the key already exists (there should be one to one correspondence)
	if (this->has(transition.from, transition.symbol))
		return;
	Key k = std::make_pair(transition.from, transition.symbol);
	transitions.emplace(k, transition.to);
}

template <typename F, typename S, typename T>
void Transitions<F, S, T>::add(const std::set<Transition<F, S, T>>& transitions)
{
	typename std::set<Transition<F, S, T>>::const_iterator it;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		this->add(*it);
	}
}

template <typename F, typename S, typename T>
void Transitions<F, S, T>::add(const Transitions<F, S, T>& other)
{
	Transitions<F, S, T>::const_iterator it;
	for (it = other.begin(); it != other.end(); ++it)
	{
		F from = it->first.first;
		S symbol = it->first.second;
		T to = it->second;
		this->add(from, symbol, to);
	}
}

template <typename F, typename S, typename T>
void Transitions<F, S, T>::add(std::initializer_list<Transition<F, S, T>> transitions)
{
	typename std::initializer_list<Transition<F, S, T>>::const_iterator it;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		this->add(it->from, it->symbol, it->to);
	}
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



template <typename F, typename S>
class Transitions<F, S, StateSet<F>>
{
public:
	using Key = std::pair<F, S>;
	using iterator = typename std::map<Key, StateSet<F>>::iterator;
	using const_iterator = typename std::map<Key, StateSet<F>>::const_iterator;

	iterator begin() { return transitions.begin(); }
	iterator end() { return transitions.end(); }

	const_iterator begin() const { return transitions.begin(); }
	const_iterator end() const { return transitions.end(); }

	Transitions(const std::set<Transition<F, S, StateSet<F>>>& transitions);
	Transitions(const Transitions<F, S, StateSet<F>>& other);
	Transitions(std::initializer_list<Transition<F, S, StateSet<F>>> transitions);

	template <typename F2, typename S2, typename T2>
	Transitions(const Transitions<F2, S2, T2>& other)
	{
		// Constructor for trying implicit conversion of the typenames of another Transitions object to its own typenames.
		typename Transitions<F2, S2, T2>::const_iterator it;
		for (it = other.begin(); it != other.end(); ++it)
		{
			// Cast each type for easy conversion
			add(static_cast<F>(it->first.first), static_cast<S>(it->first.second), static_cast<StateSet<F>>(it->second));
		}
	}

	void add(const F& from, const S& symbol, const StateSet<F>& to);
	void add(const Transition<F, S, StateSet<F>>& transition);
	void add(const std::set<Transition<F, S, StateSet<F>>>& transitions);
	void add(const Transitions<F, S, StateSet<F>>& other);
	void add(std::initializer_list<Transition<F, S, StateSet<F>>> transitions);
	bool has(const F& from, const S& symbol) const;
	StateSet<F> get(const F& from, const S& symbol) const;

	friend std::ostream& operator<<(std::ostream& os, const Transitions<F, S, StateSet<F>>& transitions)
	{
		Transitions<F, S, StateSet<F>>::const_iterator it;
		for (it = transitions.begin(); it != transitions.end(); ++it)
		{
			F from = it->first.first;
			S symbol = it->first.second;
			StateSet<F> to = it->second;
			Transition<F, S, StateSet<F>> currentTransition(from, symbol, to);
			os << currentTransition << '\n';
		}
		return os;
	}
			
private:
	std::map<Key, StateSet<F>> transitions;
};

template <typename F, typename S>
Transitions<F, S, StateSet<F>>::Transitions(const std::set<Transition<F, S, StateSet<F>>>& transitions)
{
	// Builds the transition table based on a set of Transition objects.
	this->add(transitions);
}

template <typename F, typename S>
Transitions<F, S, StateSet<F>>::Transitions(const Transitions<F, S, StateSet<F>>& other)
{
	this->add(other);
}

template <typename F, typename S>
Transitions<F, S, StateSet<F>>::Transitions(std::initializer_list<Transition<F, S, StateSet<F>>> transitions)
{
	// Builds the transition table based on an initializer list.
	this->add(transitions);
}

template <typename F, typename S>
void Transitions<F, S, StateSet<F>>::add(const F& from, const S& symbol, const StateSet<F>& to)
{
	// Adds a transition based on the 3 needed objects.
	Key k = std::make_pair(from, symbol);
	transitions[k].add(to);
}

template <typename F, typename S>
void Transitions<F, S, StateSet<F>>::add(const Transition<F, S, StateSet<F>>& transition)
{
	// Adds a transition based on a Transition object.
	Key k = std::make_pair(transition.from, transition.symbol);
	transitions[k].add(transition.to);
}

template <typename F, typename S>
void Transitions<F, S, StateSet<F>>::add(const std::set<Transition<F, S, StateSet<F>>>& transitions)
{
	typename std::set<Transition<F, S, StateSet<F>>>::const_iterator it;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		this->add(*it);
	}
}

template <typename F, typename S>
void Transitions<F, S, StateSet<F>>::add(const Transitions<F, S, StateSet<F>>& other)
{
	Transitions<F, S, StateSet<F>>::const_iterator it;
	for (it = other.begin(); it != other.end(); ++it)
	{
		F from = it->first.first;
		S symbol = it->first.second;
		StateSet<F> to = it->second;
		this->add(from, symbol, to);
	}
}

template <typename F, typename S>
void Transitions<F, S, StateSet<F>>::add(std::initializer_list<Transition<F, S, StateSet<F>>> transitions)
{
	typename std::initializer_list<Transition<F, S, StateSet<F>>>::const_iterator it;
	for (it = transitions.begin(); it != transitions.end(); ++it)
	{
		this->add(it->from, it->symbol, it->to);
	}
}

template <typename F, typename S>
bool Transitions<F, S, StateSet<F>>::has(const F& from, const S& symbol) const
{
	// Checks if the result of a key exists in the map.
	Key k = std::make_pair(from, symbol);
	auto it = transitions.find(k);
	return it != transitions.end();
}

template <typename F, typename S>
StateSet<F> Transitions<F, S, StateSet<F>>::get(const F& from, const S& symbol) const
{
	// Gets the result of the key pair in the map.
	Key k = std::make_pair(from, symbol);
	return transitions.at(k);
}

// Aliases for certain automata / constructions

using DFATransitions = Transitions<State, Symbol, State>;
using NFATransitions = Transitions<State, Symbol, States>;
using OutputTransitions = Transitions<State, Symbol, Symbol>;
using PDATransitions = Transitions<PDAPair, Symbol, PDAPairs>;
