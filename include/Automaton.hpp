#pragma once

#include "Word.hpp"
#include <iostream>

enum class AcceptCode
{
	Accepted,
	Denied,
	Abort
};

class Automaton
{
public:
	virtual AcceptCode accepts(Word word) const = 0;
	AcceptCode accepts(const std::string& s) const { return accepts(Word(s)); }
	AcceptCode accepts(const char* s, const size_t arrsize) const { return accepts(Word(s, arrsize)); }

	void test(const Word& word) const { std::cout << "Word " << word << " is: " << getMessage(accepts(word)) << '\n'; }
	void test(const std::string& s) const { std::cout << "Word " << s << " is: " << getMessage(accepts(s)) << '\n'; }
	void test(const char* s, const size_t arrsize) const { std::cout << "Word " << std::string(s, arrsize) << " is: " << getMessage(accepts(s, arrsize)) << '\n'; }

	std::string getMessage(AcceptCode code) const
	{
		switch (code)
		{
			case AcceptCode::Accepted:
				return "Accepted!";
			case AcceptCode::Denied:
				return "Denied.";
			case AcceptCode::Abort:
				return "Abort!";
		}
		return "Error!";
	}
};
