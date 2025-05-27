#include "OutputMap.hpp"

#include <exception>

OutputMap::OutputMap(const std::set<std::pair<State, Symbol>>& outputMap)
{
	std::set<std::pair<State, Symbol>>::const_iterator it;
	for (it = outputMap.begin(); it != outputMap.end(); ++it)
	{
		this->outputMap.emplace(it->first, it->second);
	}
}

Symbol OutputMap::getSymbol(const State& state) const
{
	OutputMap::const_iterator it = outputMap.find(state);
	if (it == outputMap.end())
		throw std::runtime_error("Requested state is not in output map!");
	return outputMap.at(state);
}
