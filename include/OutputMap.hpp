#pragma once

#include <set>
#include <map>
#include "State.hpp"
#include "Symbol.hpp"

class OutputMap
{
public:
	using iterator = std::map<State, Symbol>::iterator;
	using const_iterator = std::map<State, Symbol>::const_iterator;

	iterator begin() { return outputMap.begin(); }
	iterator end() { return outputMap.end(); }
	const_iterator begin() const { return outputMap.begin(); }
	const_iterator end() const { return outputMap.end(); }

	OutputMap() = default;
	OutputMap(const std::set<std::pair<State, Symbol>>& outputMap);
	Symbol getSymbol(const State& state) const;

	friend std::ostream& operator<<(std::ostream& os, const OutputMap& outputMap)
	{
		std::map<State, Symbol>::const_iterator mit;
		for (mit = outputMap.begin(); mit != outputMap.end(); ++mit)
		{
			os << mit->first << '/' << mit->second << '\n';
		}
		return os;
	}
private:
	std::map<State, Symbol> outputMap;
};
