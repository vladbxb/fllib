#include <States.hpp>
#include <iostream>

using namespace std;

int main()
{
	States s1;
	States s2 = { State("q0"), State("q1"), State("q2") };

	s1.add(State("q0"));
	s1.add(State("q1"));

	s2.remove(State("q2"));

	if (!s1.contains(State("q2")) && s1.contains(State("q0")) && s1.contains(State("q1")) && s2.contains(State("q0")) && s2.contains(State("q1")))
		cout << "Metodele add, remove si contains functioneaza corect!\n";
	else
		cout << "Metodele add, remove si contains NU functioneaza corect!\n";

	if (s1.size() == s2.size())
		cout << "Metoda size functioneaza corect!\n";
	else
		cout << "Metoda size NU functioneaza corect!\n";

	s1.remove(State("q0"));
	s1.remove(State("q1"));

	if (s1.empty())
		cout << "Metoda empty functioneaza corect!\n";
	else
		cout << "Metoda empty NU functioneaza corect!\n";

	s2.add(State("q0"));
	s2.add(State("q1"));
	s2.add(State("q3"));
	s1.add(State("q4"));

	s1.intersect(s2);

	set<State>::const_iterator it;
	for (it = s1.begin(); it != s1.end(); ++it)
	{
		cout << it->getName() << '\n';
	}

	States s4 = {"q0", "q1", "q2", "q3"};
	States s5 = {"q4", "q5", "q6", "q7"};

	s5.unite(s4);

	cout << s5 << '\n';

	cout << "All tests finalized!\n";
	return 0;
}
