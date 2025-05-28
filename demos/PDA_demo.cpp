#include "PDA.hpp"
#include <iostream>

int main()
{
	PDA pda({
				{ {"q0", 'Z'}, 'a', { {"q0", "AZ"} } },
				{ {"q0" ,'A'}, 'a', { {"q0", "AA"} } },
				{ {"q0", 'A'}, 'b', { {"q1", Symbol::lambda()} } },
				{ {"q1", 'A'}, 'b', { {"q1", Symbol::lambda()} } },
				{ {"q1", 'Z'}, Symbol::lambda(), { {"q1", Symbol::lambda()} } }
			},
			"q0",
			'Z',
			{}
		);

	pda.test("aabb");
	pda.test("aaaabbbb");
	pda.test("aabaaabbbbb");

	return 0;
}
