#include "Symbol.hpp"
#include <iostream>

using namespace std;

int main()
{
	Symbol s('c');
	if (s.getSymbol() == 'c')
		cout << "getSymbol method works fine!\n";
	cout << "Test finished!\n";
	return 0;
}
