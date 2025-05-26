#pragma once

#include "Symbol.hpp"
#include "Sequence.hpp"

class RegExString
{
public:
	using iterator = Sequence<Symbol>::iterator;
	using const_iterator = Sequence<Symbol>::const_iterator;

	iterator begin() { return string.begin(); }
	iterator end() { return string.end(); }

	const_iterator begin() const { return string.begin(); }
	const_iterator end() const { return string.end(); }

	RegExString(const std::string& string) : string(string) {}
	// Convert const char* to const_iterable
	RegExString(const char* string) : string(std::string(string)) {}

	size_t size() const { return string.size(); }
	bool empty() const { return string.empty(); }

	std::string toString() const;
private:
	Sequence<Symbol> string;
};

class MutableRegExString
{
public:
	using iterator = MutableSequence<Symbol>::iterator;
	using const_iterator = MutableSequence<Symbol>::const_iterator;

	iterator begin() { return string.begin(); }
	iterator end() { return string.end(); }

	const_iterator begin() const { return string.begin(); }
	const_iterator end() const { return string.end(); }

	MutableRegExString() = default;
	MutableRegExString(const std::string& string) : string(string) {}
	MutableRegExString(const char* string) : string(std::string(string)) {}
	MutableRegExString(const MutableRegExString& mut) : string(mut.string) {}
	MutableRegExString(const Symbol& symbol) : string(symbol) {}

	MutableRegExString concat(const MutableRegExString& r) const;
	MutableRegExString star() const;
	MutableRegExString _union(const MutableRegExString& r) const;
	static MutableRegExString concat(const MutableRegExString& r1, const MutableRegExString& r2);
	static MutableRegExString star(const MutableRegExString& r);
	static MutableRegExString _union(const MutableRegExString& r1, const MutableRegExString& r2);

	bool size() const { return string.size(); }
	bool empty() const { return string.empty(); }
	Symbol back() const { return string.back(); }

	std::string toString() const;

	friend bool operator==(const MutableRegExString& r1, const MutableRegExString& r2) { return r1.string == r2.string; }
	friend bool operator<(const MutableRegExString& r1, const MutableRegExString& r2) { return r1.string < r2.string; }
	friend std::ostream& operator<<(std::ostream& os, const MutableRegExString& r);

private:
	MutableSequence<Symbol> string;
};

