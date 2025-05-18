#include "Alphabet.hpp"
#include <iostream>

using namespace std;

int main()
{
	Alphabet a("abcdef");

	a.add(Symbol('g'));

	if (a.contains(Symbol('a')) && a.contains('b') && a.contains('g') && !a.contains('z'))
		cout << "Ambele overloaduri pentru contains functioneaza!\n";
	else
		cout << "Ambele overloaduri pentru contains NU functioneaza!\n";

	cout << "Test finalizat!\n";
	return 0;
}
