#include "MooreMachine.hpp"
#include <iostream>

int main()
{
	MooreMachine a({
						{"q0", '0', "q1"},
						{"q0", '1', "q2"},
						{"q1", '0', "q1"},
						{"q1", '1', "q3"},
						{"q2", '0', "q2"},
						{"q2", '1', "q3"},
						{"q3", '0', "q3"},
						{"q3", '1', "q2"}
					},
					{
						{"q0", 'a'},
						{"q1", 'c'},
						{"q2", 'a'},
						{"q3", 'b'}
					},
					"q0"
				);

	a.test("001101");

	return 0;
}
