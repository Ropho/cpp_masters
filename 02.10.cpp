////////////////////////////////////////////////////////////////////////////////////

// support : www.cs.usfca.edu/~galles/visualization/ComparisonSort.html

////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////
std::size_t choose_pivot_index(const std::vector < int > & vector, std::size_t left, std::size_t right)
{
	auto first = left;
	auto middle = std::midpoint(left, right - 1);
	auto last = right - 1;

	auto a = vector[first];
	auto b = vector[middle];
	auto c = vector[last];

	if ((a <= b && b <= c) || (c <= b && b <= a)) {
		return middle;
	}
	else if ((b <= a && a <= c) || (c <= a && a <= b)) {
		return first;
	}
	else {
		return last;
	}
}

////////////////////////////////////////////////////////////////////////////////////
std::size_t partition(std::vector < int > & vector, std::size_t left, std::size_t right)
{
	auto pivot_index = choose_pivot_index(vector, left, right);
	auto pivot = vector[pivot_index];

	std::swap(vector[pivot_index], vector[left]);

	// Метод Хоара: i и j движутся навстречу друг другу
	auto i = left + 1;
	auto j = right - 1;

	while (i <= j) {
		while (i < right && vector[i] < pivot) {
			++i;
		}

		while (j > left && vector[j] > pivot) {
			--j;
		}

		if (i >= j) {
			break;
		}

		std::swap(vector[i], vector[j]);
		++i;
		--j;
	}

	std::swap(vector[left], vector[j]);
	return j;
}

////////////////////////////////////////////////////////////////////////////////////

void quicksort(std::vector < int > & vector, std::size_t left, std::size_t right)
{
	if (right - left > 1)
	{
		auto pivot_index = partition(vector, left, right);

		quicksort(vector, left, pivot_index);
		quicksort(vector, pivot_index + 1, right);
	}
}

////////////////////////////////////////////////////////////////////////////////////
void sort(std::vector < int > & vector)
{
	quicksort(vector, 0, std::size(vector));
}

////////////////////////////////////////////////////////////////////////////////////

int main()
{
	auto size = 1'000uz;

//  ---------------------------------------

	std::vector < int > vector(size, 0);

//  ---------------------------------------

	for (auto i = 0uz; i < size; ++i)
	{
		vector[i] = size - i;
	}

//  ---------------------------------------

	sort(vector);

//  ---------------------------------------

	assert(std::ranges::is_sorted(vector));
}

////////////////////////////////////////////////////////////////////////////////////