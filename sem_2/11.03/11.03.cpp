#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iterator>
#include <random>
#include <string>
#include <vector>

template <typename RandomIt, typename Compare>
void median_of_three_move_to_first(RandomIt const first, RandomIt const last, Compare comp)
{
	auto const tail = std::prev(last);
	auto mid = first;
	std::advance(mid, std::distance(first, last) / 2);

	if (comp(*mid, *first))
	{
		std::iter_swap(first, mid);
	}

	if (comp(*tail, *first))
	{
		std::iter_swap(first, tail);
	}

	if (comp(*tail, *mid))
	{
		std::iter_swap(mid, tail);
	}

	std::iter_swap(first, mid);
}

template <typename RandomIt, typename Compare>
RandomIt hoare_partition(RandomIt const first, RandomIt const last, Compare comp)
{
	median_of_three_move_to_first(first, last, comp);

	auto const pivot = *first;

	auto left = first;
	auto right = std::prev(last);

	for (;;)
	{
		while (comp(*left, pivot))
		{
			++left;
		}

		while (comp(pivot, *right))
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

template <typename RandomIt, typename Compare>
void quicksort(RandomIt const first, RandomIt const last, Compare comp)
{
	if (std::distance(first, last) <= 1)
	{
		return;
	}

	RandomIt const boundary = hoare_partition(first, last, comp);

	quicksort(first, std::next(boundary), comp);
	quicksort(std::next(boundary), last, comp);
}

template <typename RandomIt>
void quicksort(RandomIt const first, RandomIt const last)
{
	using value_type = typename std::iterator_traits<RandomIt>::value_type;

	quicksort(first, last, std::less<value_type>{});
}

static bool less_absolute(int const lhs, int const rhs)
{
	return std::abs(lhs) < std::abs(rhs);
}

static void test_integers_default_less()
{
	std::vector<int> data = {5, 2, 8, 1, 9, 3, 7, 4, 6};

	quicksort(data.begin(), data.end());

	assert(std::is_sorted(data.begin(), data.end()));
}

static void test_strings_std_less_object()
{
	std::vector<std::string> data = {"gamma", "alpha", "beta"};

	quicksort(data.begin(), data.end(), std::less<std::string>{});

	assert(std::is_sorted(data.begin(), data.end(), std::less<std::string>{}));
}

static void test_lambda_descending()
{
	std::vector<int> data = {1, 2, 3, 4, 5};

	auto const descending = [](int const lhs, int const rhs) { return lhs > rhs; };

	quicksort(data.begin(), data.end(), descending);

	assert(std::is_sorted(data.begin(), data.end(), descending));
}

static void test_free_function_comparator()
{
	std::vector<int> data = {-3, 10, -1, 7, 0, 2};

	quicksort(data.begin(), data.end(), less_absolute);

	assert(std::is_sorted(data.begin(), data.end(), less_absolute));
	assert(data == std::vector<int>({0, -1, 2, -3, 7, 10}));
}

static void test_shuffled_duplicates()
{
	std::vector<int> data(64, 7);
	std::mt19937 generator{9'871'234u};
	std::shuffle(data.begin(), data.end(), generator);

	quicksort(data.begin(), data.end());

	assert(std::is_sorted(data.begin(), data.end()));
}

int main()
{
	test_integers_default_less();
	test_strings_std_less_object();
	test_lambda_descending();
	test_free_function_comparator();
	test_shuffled_duplicates();

	return 0;
}
