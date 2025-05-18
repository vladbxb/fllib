#include "Sequence.hpp"
#include <iostream>

using namespace std;

int main()
{
	Sequence<int> s;
	s.push_back(3);
	int t = -5;
	s.push_back(t);
	s.push_back(int(8.3));
	
	cout << "Printing sequence: ";
	Sequence<int>::iterator it;
	for (it = s.begin(); it != s.end(); ++it)
	{
		cout << *it << ' ';
	}
	cout << '\n';
		
	Sequence<int>::const_iterator cit;
	for (cit = s.begin(); cit != s.end(); ++cit)
	{
		cout << *cit << ' ';
	}
	cout << '\n';

	if (s.contains(-5) && s.contains(8) && s.contains(3) && !s.contains(10))
		cout << "Contains method works as expected!\n";
	else
		cout << "Contains method DOESNT work as expected!\n";

	int p = s.pop_front();
	cout << "First added element is: " << p << '\n';
	if (!s.contains(p))
		cout << "Pop front works as expected!\n";
	else
		cout << "Pop front DOESN'T work as expected!\n";

	if (s.size() == 2)
		cout << "Size method works as expected!\n";
	else
		cout << "Size method DOESN'T work as expected!\n";

	return 0;
}
