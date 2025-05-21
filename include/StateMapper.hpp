#pragma once

#include "States.hpp"
#include "State.hpp"
#include <map>

class StateMapper
{
public:
	StateMapper() : currentID(0) {}
	void add(const States& states);
	State reserveState();
	bool has(const States& states) const;
	State get(const States& states) const;	
private:
	size_t currentID;
	States definedResults;
	std::map<States, State> IDs;

	std::string getStringForID(size_t ID) const;
};
