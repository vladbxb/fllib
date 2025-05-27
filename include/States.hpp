#pragma once

#include "StateSet.hpp"
#include "State.hpp"
#include <initializer_list>

class States : public StateSet<State>
{
public:
	using StateSet<State>::StateSet;

	States(std::initializer_list<const char*> list);
};
