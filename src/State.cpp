#include "State.hpp"

#include "States.hpp"

#include <ostream>

State::State(const std::string& name) : name(name) {}
State::State(const char* name) : name(name) {}
State::State(const State& s) : name(s.name) {}
State::State(const States& s) : name(s.toName()) {}

std::string State::getName() const
{
	return this->name;
}

std::ostream& operator<<(std::ostream& os, const State& state)
{
	os << state.name;
	return os;
}
