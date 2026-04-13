#include <algorithm>
#include <cassert>
#include <iterator>
#include <random>
#include <string>
#include <vector>

template<typename RandomIt>
void median_of_three_move_to_first(RandomIt const first, RandomIt const last)
{
	auto const tail = std::prev(last);
	auto mid = first;
	std::advance(mid, std::distance(first, last) / 2);

	if (*mid < *first)
	{
		std::iter_swap(first, mid);
	}

	if (*tail < *first)
	{
		std::iter_swap(first, tail);
	}

	if (*tail < *mid)
	{
		std::iter_swap(mid, tail);
	}

	std::iter_swap(first, mid);
}

template<typename RandomIt>
RandomIt hoare_partition(RandomIt const first, RandomIt const last)
{
	median_of_three_move_to_first(first, last);

	auto const pivot = *first;

	auto left = first;
	auto right = std::prev(last);

	for (;;)
	{
		while (*left < pivot)
		{
			++left;
		}

		while (*right > pivot)
		{
			--right;
		}

		if (left >= right)
		{
			return right;
		}

		std::iter_swap(left, right);
		++left;
		--right;
	}
}

template<typename RandomIt>
void quicksort(RandomIt const first, RandomIt const last)
{
	if (std::distance(first, last) <= 1)
	{
		return;
	}

	RandomIt const boundary = hoare_partition(first, last);

	quicksort(first, std::next(boundary));
	quicksort(std::next(boundary), last);
}

static void test_integers()
{
	std::vector<int> data = { 5, 2, 8, 1, 9, 3, 7, 4, 6 };

	quicksort(data.begin(), data.end());

	assert(std::is_sorted(data.begin(), data.end()));
}

static void test_strings()
{
	std::vector<std::string> data = { "gamma", "alpha", "beta" };

	quicksort(data.begin(), data.end());

	assert(data.at(0) == "alpha");
	assert(data.at(1) == "beta");
	assert(data.at(2) == "gamma");
}

static void test_shuffled_duplicates()
{
	std::vector<int> data(64, 7);
	std::mt19937 generator{ 9'871'234u };
	std::shuffle(data.begin(), data.end(), generator);

	quicksort(data.begin(), data.end());

	assert(std::is_sorted(data.begin(), data.end()));
}

int main()
{
	test_integers();
	test_strings();
	test_shuffled_duplicates();
}
