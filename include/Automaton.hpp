#pragma once

#include "Word.hpp"
#include "OutputSequence.hpp"
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
	AcceptCode accepts(const char* s) const { return accepts(Word(s)); }

	void test(const Word& word) const { std::cout << "Word " << word << " is: " << getMessage(accepts(word)) << '\n'; }
	void test(const std::string& s) const { std::cout << "Word " << s << " is: " << getMessage(accepts(s)) << '\n'; }
	void test(const char* s) const { std::cout << "Word " << std::string(s) << " is: " << getMessage(accepts(s)) << '\n'; }

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

class OutputAutomaton
{
public:
	virtual SymbolOutput run(Word word) const = 0;
	SymbolOutput run(const std::string& s) const { return run(Word(s)); }
	SymbolOutput run(const char* s) const { return run(Word(s)); }

	void test(const Word& word) const { std::cout << "Output for word: " << word << " is: " << run(word) << '\n'; }
	void test(const std::string& s) const { std::cout << "Output for word: " << s << " is: " << run(s) << '\n'; }
	void test(const char* s) const { std::cout << "Output for word: " << s << " is: " << run(s) << '\n'; }
};
