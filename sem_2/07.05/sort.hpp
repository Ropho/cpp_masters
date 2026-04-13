////////////////////////////////////////////////////////////////////////////////

#ifndef SORT_HPP
#define SORT_HPP

#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

template < typename T >
std::size_t choose_pivot_index(const std::vector < T > & vec, std::size_t left, std::size_t right)
{
	auto first   = left;
	auto middle  = std::midpoint(left, right - 1);
	auto last    = right - 1;

	auto a = vec[first];
	auto b = vec[middle];
	auto c = vec[last];

	if ((a <= b && b <= c) || (c <= b && b <= a))
	{
		return middle;
	}
	if ((b <= a && a <= c) || (c <= a && a <= b))
	{
		return first;
	}
	return last;
}

////////////////////////////////////////////////////////////////////////////////

template < typename T >
std::size_t partition(std::vector < T > & vec, std::size_t left, std::size_t right)
{
	auto pivot_index = choose_pivot_index(vec, left, right);
	auto pivot = vec[pivot_index];

	std::swap(vec[pivot_index], vec[left]);

	auto i = left + 1;
	auto j = right - 1;

	while (i <= j)
	{
		while (i < right && vec[i] < pivot)
		{
			++i;
		}
		while (j > left && vec[j] > pivot)
		{
			--j;
		}
		if (i >= j)
		{
			break;
		}
		std::swap(vec[i], vec[j]);
		++i;
		--j;
	}

	std::swap(vec[left], vec[j]);
	return j;
}

////////////////////////////////////////////////////////////////////////////////

template < typename T >
void insertion_sort(std::vector < T > & vec, std::size_t left, std::size_t right)
{
	for (auto i = left + 1; i < right; ++i)
	{
		auto key = vec[i];
		auto j   = i;

		while (j > left && vec[j - 1] > key)
		{
			vec[j] = vec[j - 1];
			--j;
		}
		vec[j] = key;
	}
}

////////////////////////////////////////////////////////////////////////////////

template < typename T >
void quicksort(std::vector < T > & vec, std::size_t left, std::size_t right, std::size_t threshold)
{
	if (right - left <= threshold)
	{
		if (right - left > 1)
		{
			insertion_sort(vec, left, right);
		}
		return;
	}

	auto pivot_index = partition(vec, left, right);
	quicksort(vec, left, pivot_index, threshold);
	quicksort(vec, pivot_index + 1, right, threshold);
}

////////////////////////////////////////////////////////////////////////////////

template < typename T >
void sort(std::vector < T > & vec, std::size_t threshold);

////////////////////////////////////////////////////////////////////////////////

template < typename T >
void sort(std::vector < T > & vec)
{
	sort(vec, std::size_t(0));
}

////////////////////////////////////////////////////////////////////////////////

template < typename T >
void sort(std::vector < T > & vec, std::size_t threshold)
{
	quicksort(vec, 0, vec.size(), threshold);
}

////////////////////////////////////////////////////////////////////////////////

#endif
