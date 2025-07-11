#include "MealyMachine.hpp"
#include <iostream>

int main()
{
	MealyMachine a({
					 {"q0", '0', "q1"},
					 {"q0", '1', "q2"},
					 {"q1", '0', "q1"},
					 {"q1", '1', "q3"},
					 {"q2", '0', "q3"},
					 {"q2", '1', "q2"},
					 {"q3", '0', "q3"},
					 {"q3", '1', "q3"}
				   },
				   {
				   	 {"q0", '0', 'a'},
					 {"q0", '1', 'a'},
					 {"q1", '0', 'b'},
					 {"q1", '1', 'c'},
					 {"q2", '0', 'c'},
					 {"q2", '1', 'a'},
					 {"q3", '0', 'c'},
					 {"q3", '1', 'b'}
				   },
				   "q0"
				);

	a.test("0111");

	return 0;
}
