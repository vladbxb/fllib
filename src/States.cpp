#include "States.hpp"

States::States(std::initializer_list<const char*> list)
{
	std::initializer_list<const char*>::const_iterator it;
	for (it = list.begin(); it != list.end(); ++it)
	{
		add(State(*it));
	}
}
