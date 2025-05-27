#pragma once

#include "Symbol.hpp"
#include <string>
#include <deque>
#include <exception>

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
	template <typename U>
	Sequence(const U& collection);

	bool contains(const T& element) const;
	size_t size() const { return sequence.size(); }
protected:
	std::deque<T> sequence;
};

template <typename T>
template <typename U>
Sequence<T>::Sequence(const U& collection)
{
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
	template <typename U>
	MutableSequence(const U& collection) : Sequence<T>(collection) {}
	void push_back(const T& element);
	T pop_front();
};

template <typename T>
void MutableSequence<T>::push_back(const T& element)
{
	this->sequence.push_back(element);
}

template <typename T>
T MutableSequence<T>::pop_front()
{
	if (this->sequence.empty())
		throw std::runtime_error("Cannot pop_front() an empty sequence!");
	T first = this->sequence.front();
	this->sequence.pop_front();
	return first;
}
