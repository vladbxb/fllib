#include "Word.hpp"
#include "Alphabet.hpp"
#include <iostream>

using namespace std;

int main()
{
	Word w("apanapameperepe");
	Symbol p1 = w.processSymbol();
	Symbol p2 = w.processSymbol();
	if (p1.getSymbol() == 'a' && p2.getSymbol() == 'p')
		cout << "processSymbol method is working correctly!\n";
	size_t s = w.size();
	if (s == 13)
		cout << "size method is working fine!\n";
	Alphabet a1("apanapameperepe");
	Alphabet a2("ab");

	if (w.definedOver(a1) && !w.definedOver(a2))
		cout << "definedOver method is working correctly!\n";

	cout << "Tests finalized!\n";
	return 0;
}

