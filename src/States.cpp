#include "States.hpp"

States::States(std::initializer_list<State> list) : states(list) {}
States::States(std::initializer_list<const char*> list)
{
	std::initializer_list<const char*>::const_iterator it;
	for (it = list.begin(); it != list.end(); ++it)
	{
		states.insert(State(*it));
	}
}
States::States(const State& state)
{
	states.insert(state);
}

void States::add(const State& state)
{
	states.insert(state);
}

void States::add(const States& states)
{
	States::const_iterator it;
	for (it = states.begin(); it != states.end(); ++it)
	{
		this->states.insert(*it);
	}
}

void States::remove(const State& state)
{
	states.erase(state);
}

bool States::contains(const State& state) const
{
	return states.find(state) != states.end();
}

size_t States::size() const
{
	return states.size();
}

bool States::empty() const
{
	return states.empty();
}

void States::intersect(States& other)
{
	std::set<State> intersection;
	std::set<State>::iterator it;
	for (it = states.begin(); it != states.end(); ++it)
	{
		if (other.contains(*it))
			intersection.insert(*it);
	}
	states = intersection;	
}

void States::unite(States& other)
{
	this->add(other);
}

// std::string States::toName() const
// {
// 	// Concatenates the names of all the States in the set.
// 	std::string compositeStateName;
// 	States::const_iterator it;
// 	for (it = states.begin(); it != states.end(); ++it)
// 	{
// 		compositeStateName.append(it->getName());
// 	}
// 	return compositeStateName;	
// }

std::string States::formatStates() const
{
	// Prettifies the state names inside accolades
	std::string statesString = "{ ";
	States::const_iterator it;
	for (it = states.begin(); it != states.end(); ++it)
	{
		States::const_iterator it2 = it;
		++it2;
		if (it2 == states.end())
			statesString.append(std::string("\"") + it->getName() + std::string("\" "));
		else
			statesString.append(std::string("\"") + it->getName() + std::string("\", "));
	}
	statesString.append(std::string("}"));
	return statesString;
}

// State States::singleState() const
// {
// 	return State(this->toName());
// }

std::ostream& operator<<(std::ostream& os, const States& states)
{
	os << states.formatStates();
	return os;
}
