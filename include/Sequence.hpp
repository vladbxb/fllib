#pragma once

#include <deque>
#include <optional>

template <typename T> class Sequence
{
public:
	using iterator = typename std::deque<T>::iterator;
	using const_iterator = typename std::deque<T>::const_iterator;

	iterator begin() { return sequence.begin(); }
	iterator end() { return sequence.end(); }

	const_iterator begin() const { return sequence.begin(); }
	const_iterator end() const { return sequence.end(); }

	void push_back(const T& element);
	bool contains(const T& element) const;
	std::optional<T> pop_front();
	size_t size() const;
private:
	std::deque<T> sequence;
};

template <typename T>
void Sequence<T>::push_back(const T& element)
{
	sequence.push_back(element);
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
std::optional<T> Sequence<T>::pop_front()
{
	if (!sequence.empty())
	{
		T first = sequence.front();
		sequence.pop_front();
		return first;
	}
	return std::nullopt;
}

template <typename T>
size_t Sequence<T>::size() const
{
	return sequence.size();
}
