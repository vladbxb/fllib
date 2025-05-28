#pragma once

#include "PushdownID.hpp"
#include "StateSet.hpp"

class PushdownIDs : public StateSet<PushdownID>
{
public:
	using StateSet<PushdownID>::StateSet;
};
