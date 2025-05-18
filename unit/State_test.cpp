#include "State.hpp"
#include <iostream>

using namespace std;

int main()
{
	State s("q0");
	State t("q0");
	State u("q1");
	State v = "q2";

	cout << "State s: " << s.getName() << '\n';
	cout << "State t: " << t.getName() << '\n';
	cout << "State u: " << u.getName() << '\n';

	if (s == t && s != u && t != u)
	   	cout << "Equality operator works as expected!\n";	
	else
		cout << "Equality operator DOESN'T work as expected!\n";

	cout << u << v << '\n';

	cout << "Test finished!\n";
	return 0;
}
