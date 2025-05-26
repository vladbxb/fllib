#include "NFA.hpp"
#include <iostream>

int main()
{
	// works
	// NFA a({
	// 		{"q0", 'a', {"q1"}},
	// 		{"q0", 'b', {"q3"}},
	// 	 	{"q1", 'a', {"q1", "q2"}},
	// 		{"q1", 'b', {"q1"}},
	// 		{"q3", 'a', {"q2", "q4"}},
	// 		{"q3", 'b', {"q2", "q3"}}
	// 	  },
	// 	  "q0",
	// 	  {"q4"}
	// 	 );

	NFA a({
			{"q0", 'b', {"q1"}},
			{"q1", 'a', {"q1", "q2", "q3"}},
			{"q1", 'b', {"q1", "q2"}},
			{"q2", 'b', {"q0"}}
		  },
		  "q0",
		  {"q3"}
		 );

	// idk if this works
	// NFA a({
	// 		{"q0", 'a', {"q1", "q2"}},
	// 		{"q0", 'b', {"q1"}},
	// 		{"q2", 'a', {"q3"}},
	// 		{"q2", 'b', {"q3"}},
	// 		{"q3", 'a', {"q2", "q0"}},
	// 		{"q3", 'b', {"q0"}}
	// 	  },
	// 	  "q0",
	// 	  {"q1"}
	// 	 );

	RegEx conv = a.convertToRegEx();
	std::cout << "Converted to RegEx: " << conv << '\n';
	return 0;
}
