#include "NFA.hpp"
#include <iostream>

using namespace std;
int main()
{
	NFA a({
			{ "q0", 'a', {"q0", "q2"} },
			{ "q0", 'b', {"q1"} },
			{ "q1", 'a', {"q4"} },
			{ "q1", Symbol::lambda(), {"q0"} },
			{ "q2", 'b', {"q3"} },
			{ "q3", 'c', {"q1", "q3"} },
		},
		"q0",
		{"q4"}
	   );

	std::cout << "Lambda NFA test:\n\n";

	// Accepted
	a.test("aaabca");
	a.test("babcabcca");
	a.test("bbba");
	a.test("aaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbba");

	// Denied
	a.test("aaabc");
	a.test("bbbbbaaabbb");
	a.test("abcb");
	a.test("aab");

	// Abort
	a.test("bc");
	a.test("ccaaaa");
	a.test("abccccccccccbc");
	a.test("bbc");

	std::cout << a;

	NFA r = a.convertToRegularNFA();

	std::cout << "Regular NFA tests:\n\n";

	// Accepted
	r.test("aaabca");
	r.test("babcabcca");
	r.test("bbba");
	r.test("aaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbba");

	// Denied
	r.test("aaabc");
	r.test("bbbbbaaabbb");
	r.test("abcb");
	r.test("aab");

	// Abort
	r.test("bc");
	r.test("ccaaaa");
	r.test("abccccccccccbc");
	r.test("bbc");

	std::cout << r;

	DFA d = a.convertToDFA();

	std::cout << "DFA tests:\n\n";

	// Accepted
	d.test("aaabca");
	d.test("babcabcca");
	d.test("bbba");
	d.test("aaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbba");

	// Denied
	d.test("aaabc");
	d.test("bbbbbaaabbb");
	d.test("abcb");
	d.test("aab");

	// Abort
	d.test("bc");
	d.test("ccaaaa");
	d.test("abccccccccccbc");
	d.test("bbc");

	std::cout << d;

	std::cout << "Converted to RegEx: " << a.convertToRegEx() << '\n';

	return 0;
}
