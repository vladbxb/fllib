#pragma once

#include "State.hpp"
#include <set>
#include <initializer_list>

class States
{
public:
	using iterator = std::set<State>::iterator;
	using const_iterator = std::set<State>::const_iterator;

	iterator begin() { return states.begin(); }
	iterator end() { return states.end(); }

	const_iterator begin() const { return states.begin(); }
	const_iterator end() const { return states.end(); }

	States() = default;
	States(std::initializer_list<State> list);
	States(std::initializer_list<const char*> list);
	States(const State& state);
	States(const std::set<State>& states) : states(states) {}

	void add(const State& state);
	void add(const States& states);
	void remove(const State& state);
	bool contains(const State& state) const;

	size_t size() const;
	bool empty() const;

	void intersect(States& other);
	void unite(States& other);

	std::string toName() const;
	State singleState() const;

	States& operator=(const States& other)
	{
		if (this != &other)
			this->states = other.states;
		return *this;
	}

	bool operator<(const States& other) const
	{
		return states < other.states;
	}

	friend std::ostream& operator<<(std::ostream& os, const States& states);
private:
	std::set<State> states;
};
