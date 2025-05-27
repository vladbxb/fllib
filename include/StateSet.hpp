#pragma once

#include <string>
#include <ostream>
#include <set>
#include <initializer_list>

template <typename S>
class StateSet
{
public:
	using iterator = typename std::set<S>::iterator;
	using const_iterator = typename std::set<S>::const_iterator;

	iterator begin() { return states.begin(); }
	iterator end() { return states.end(); }
	
	const_iterator begin() const { return states.begin(); }
	const_iterator end() const { return states.end(); }

	StateSet() = default;
	StateSet(std::initializer_list<S> list);
	// StateSet(std::initializer_list<const char*> list);
	StateSet(const S& state);
	StateSet(const std::set<S>& states) : states(states) {}

	void add(const S& state);
	void add(const StateSet& states);
	void remove(const S& state);
	bool contains(const S& state) const;

	size_t size() const;
	bool empty() const;

	void intersect(StateSet& other);
	void unite(StateSet& other);

	std::string formatStates() const;

	StateSet& operator=(const StateSet& other)
	{
		if (this != &other)
			this->states = other.states;
		return *this;
	}

	bool operator==(const StateSet& other) const
	{
		return states == other.states;
	}

	bool operator!=(const StateSet& other) const
	{
		return !(*this == other);
	}

	bool operator<(const StateSet& other) const
	{
		return states < other.states;
	}
private:
	std::set<S> states;
};

template <typename S>
StateSet<S>::StateSet(std::initializer_list<S> list) : states(list) {}

// template <typename S>
// StateSet<S>::StateSet(std::initializer_list<const char*> list)
// {
// 	std::initializer_list<const char*>::const_iterator it;
// 	for (it = list.begin(); it != list.end(); ++it)
// 	{
// 		states.insert(S(*it));
// 	}
// }

template <typename S>
StateSet<S>::StateSet(const S& state)
{
	states.insert(state);
}

template <typename S>
void StateSet<S>::add(const S& state)
{
	states.insert(state);
}

template <typename S>
void StateSet<S>::add(const StateSet<S>& states)
{
	StateSet<S>::const_iterator it;
	for (it = states.begin(); it != states.end(); ++it)
	{
		this->states.insert(*it);
	}
}

template <typename S>
void StateSet<S>::remove(const S& state)
{
	states.erase(state);
}

template <typename S>
bool StateSet<S>::contains(const S& state) const
{
	return states.find(state) != states.end();
}

template <typename S>
size_t StateSet<S>::size() const
{
	return states.size();
}

template <typename S>
bool StateSet<S>::empty() const
{
	return states.empty();
}

template <typename S>
void StateSet<S>::intersect(StateSet<S>& other)
{
	std::set<S> intersection;
	typename std::set<S>::iterator it;
	for (it = states.begin(); it != states.end(); ++it)
	{
		if (other.contains(*it))
			intersection.insert(*it);
	}
	states = intersection;
}

template <typename S>
void StateSet<S>::unite(StateSet<S>& other)
{
	this->add(other);
}

template <typename S>
std::string StateSet<S>::formatStates() const
{
	// Prettifies the state names inside accolades
	std::string statesString = "{ ";
	StateSet<S>::const_iterator it;
	for (it = states.begin(); it != states.end(); ++it)
	{
		StateSet<S>::const_iterator it2 = it;
		++it2;
		if (it2 == states.end())
			statesString.append(it->getName() + std::string(" "));
		else
			statesString.append(it->getName() + std::string(", "));
	}
	statesString.append(std::string("}"));
	return statesString;
}

template <typename S>
std::ostream& operator<<(std::ostream& os, const StateSet<S>& states)
{
	os << states.formatStates();
	return os;
}
