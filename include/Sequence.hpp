#pragma once

#include "Symbol.hpp"
#include <string>
#include <deque>
#include <exception>
#include <cassert>

template <typename T>
class Sequence
{
public:
	using iterator = typename std::deque<T>::iterator;
	using const_iterator = typename std::deque<T>::const_iterator;

	iterator begin() { return sequence.begin(); }
	iterator end() { return sequence.end(); }

	const_iterator begin() const { return sequence.begin(); }
	const_iterator end() const { return sequence.end(); }

	Sequence() = default;
	Sequence(const T& element);
	Sequence(const Sequence<T>& other);
	template <typename U>
	Sequence(const U& collection);

	bool contains(const T& element) const;
	size_t size() const { return sequence.size(); }
	bool empty() const { return sequence.empty(); }

	friend bool operator<(const Sequence<T>& s1, const Sequence<T>& s2)
	{
		Sequence<T>::const_iterator it1 = s1.begin();
		Sequence<T>::const_iterator it2 = s2.begin();
		while (it1 != s1.end() && it2 != s2.end())
		{
			if (*it1 < *it2)
				return true;
			if (*it1 > *it2)
				return false;
			++it1;
			++it2;
		}

		return it1 == s1.end() && it2 != s2.end();
	}
protected:
	std::deque<T> sequence;
};

template <typename T>
Sequence<T>::Sequence(const T& element)
{
	this->sequence.push_back(element);
}

template <typename T>
Sequence<T>::Sequence(const Sequence<T>& other)
{
	Sequence<T>::const_iterator it;
	for (it = other.begin(); it != other.end(); ++it)
	{
		this->sequence.push_back(*it);
	}
}

template <typename T>
template <typename U>
Sequence<T>::Sequence(const U& collection)
{
	// As long as a const_iterable is passed and there exists an explicit conversion from
	// what type the collection stores to T, the Sequence can be built
	typename U::const_iterator it;
	for (it = collection.begin(); it != collection.end(); ++it)
	{
		sequence.push_back(*it);
	}
}

template <typename T>
bool Sequence<T>::contains(const T& element) const
{
	typename std::deque<T>::const_iterator it;
	for (it = sequence.begin(); it != sequence.end(); ++it)
		if (element == *it)
			return true;
	return false;
}



template <typename T>
class MutableSequence : public Sequence<T>
{
public:
	MutableSequence() = default;
	MutableSequence(const T& element) : Sequence<T>(element) {}
	MutableSequence(const MutableSequence<T>& other) : Sequence<T>(other.sequence) {}
	template <typename U>
	MutableSequence(const U& collection) : Sequence<T>(collection) {}
	void push_front(const T& element);
	void push_back(const T& element);
	void pop_front();
	void pop_back();
	T front() const;
	T back() const;
	T popFrontWithResult();

	friend bool operator==(const MutableSequence& s1, const MutableSequence& s2) { return s1.sequence == s2.sequence; }
	friend bool operator<(const MutableSequence& s1, const MutableSequence& s2) { return s1.sequence < s2.sequence; }
};

template <typename T>
void MutableSequence<T>::push_front(const T& element)
{
	this->sequence.push_front(element);
}

template <typename T>
void MutableSequence<T>::push_back(const T& element)
{
	this->sequence.push_back(element);
}

template <typename T>
void MutableSequence<T>::pop_front()
{
	assert(!this->sequence.empty());
	this->sequence.pop_front();
}

template <typename T>
void MutableSequence<T>::pop_back()
{
	assert(!this->sequence.empty());
	this->sequence.pop_back();
}

template <typename T>
T MutableSequence<T>::front() const
{
	return this->sequence.front();
}

template <typename T>
T MutableSequence<T>::back() const
{
	return this->sequence.back();
}

template <typename T>
T MutableSequence<T>::popFrontWithResult()
{
	assert(!this->sequence.empty());
	T first = this->sequence.front();
	this->sequence.pop_front();
	return first;
}
