#include "RegExString.hpp"

std::string RegExString::toString() const
{
	std::string result;
	Sequence<Symbol>::const_iterator it;
	for (it = string.begin(); it != string.end(); ++it)
	{
		result.push_back(it->getSymbol());
	}
	return result;
}

MutableRegExString MutableRegExString::concat(const MutableRegExString& r) const
{
	return MutableRegExString::concat(*this, r);
}

MutableRegExString MutableRegExString::star() const
{
	return MutableRegExString::star(*this);
}

MutableRegExString MutableRegExString::_union(const MutableRegExString& r) const
{
	return MutableRegExString::_union(*this, r);
}

MutableRegExString MutableRegExString::concat(const MutableRegExString& r1, const MutableRegExString& r2)
{
	if (r1 == MutableRegExString(Symbol::_void()) || r2 == MutableRegExString(Symbol::_void()))
		return Symbol::_void();
	if (r1 == MutableRegExString(Symbol::lambda()))
		return r2;
	if (r2 == MutableRegExString(Symbol::lambda()))
		return r1;

	// Base case
	MutableRegExString result = r1;
	MutableRegExString::const_iterator it;
	for (it = r2.begin(); it != r2.end(); ++it)
	{
		result.string.push_back(*it);
	}
	return result;
}

MutableRegExString MutableRegExString::star(const MutableRegExString& r)
{
	if (r == Symbol::_void() || r == Symbol::lambda())
		return Symbol::lambda();
	if (r.back() == Symbol::star())
		return r;

	// Base case
	MutableRegExString result = Symbol::leftParenthesis();
	result = MutableRegExString::concat(result, r);
	result = MutableRegExString::concat(result, MutableRegExString(Symbol::rightParenthesis()));
	result = MutableRegExString::concat(result, MutableRegExString(Symbol::star()));
	return result;
}

MutableRegExString MutableRegExString::_union(const MutableRegExString& r1, const MutableRegExString& r2)
{
	if (r1.empty() || r1 == Symbol::_void())
		return r2;
	if (r2.empty() || r2 == Symbol::_void())
		return r1;

	if (r1 == r2)
		return r1;

	// Base case
	MutableRegExString result = r1;
	result = MutableRegExString::concat(result, Symbol::_union());
	result = MutableRegExString::concat(result, r2);
	return result;
}

std::string MutableRegExString::toString() const
{
	std::string result;
	MutableSequence<Symbol>::const_iterator it;
	for (it = string.begin(); it != string.end(); ++it)
	{
		result.push_back(it->getSymbol());
	}
	return result;
}

std::ostream& operator<<(std::ostream& os, const MutableRegExString& r)
{
	MutableRegExString::const_iterator it;
	for (it = r.begin(); it != r.end(); ++it)
	{
		os << *it;
	}
	return os;
}

