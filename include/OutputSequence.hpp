#pragma once

#include "Symbol.hpp"
#include "Sequence.hpp"
#include <ostream>

template <typename T>
class OutputSequence
{
public:
	OutputSequence() = default;
	void push_back(const T& element) { sequence.push_back(element); }
	friend std::ostream& operator<<(std::ostream& os, const OutputSequence& o)
	{
		typename MutableSequence<T>::const_iterator it;
		for (it = o.sequence.begin(); it != o.sequence.end(); ++it)
		{
			os << *it;
		}
		return os;
	}
private:
	MutableSequence<T> sequence;
};

using SymbolOutput = OutputSequence<Symbol>;
