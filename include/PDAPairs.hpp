#pragma once

#include "PDAPair.hpp"
#include "StateSet.hpp"

class PDAPairs : public StateSet<PDAPair>
{
	using StateSet<PDAPair>::StateSet;
};
