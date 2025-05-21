#include "StateMapper.hpp"

#include <sstream>
#include <utility>

void StateMapper::add(const States& states)
{
	if (has(states))
		return;
	std::string ID = getStringForID(currentID);
	State newState(ID);
	std::pair<States, State> testPair = std::make_pair(states, newState);
	IDs.insert(testPair);
	definedResults.add(newState);
	++currentID;
}

State StateMapper::reserveState()
{
	std::string ID = getStringForID(currentID);
	State newState(ID);
	definedResults.add(newState);
	++currentID;
	return newState;
}

bool StateMapper::has(const States& states) const
{
	return IDs.find(states) != IDs.end();
}

State StateMapper::get(const States& states) const
{
	return IDs.at(states);
}

std::string StateMapper::getStringForID(size_t ID) const
{
	std::string newID = "q";
	std::ostringstream oss;
	oss << ID;
	std::string IDasStr = oss.str();
	newID.append(IDasStr);
	return newID;
}
