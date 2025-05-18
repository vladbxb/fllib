#pragma once

#include <string>

class States;

class State
{
public:
	State(const std::string& name);
	State(const char* name);
	State(const State& s);
	State(const States& s);
	std::string getName() const;

	bool operator==(const State& other) const { return name == other.name; }
	bool operator!=(const State& other) const { return name != other.name; }
	bool operator<(const State& other) const { return name < other.name; }

	friend std::ostream& operator<<(std::ostream& os, const State& state);
private:
	std::string name;
};
