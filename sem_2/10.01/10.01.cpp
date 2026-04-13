#include <cassert>
#include <cstddef>
#include <deque>
#include <iostream>
#include <memory>
#include <vector>

namespace
{
constexpr int k_vector_push_count = 4'096;
constexpr int k_deque_push_count = 8'192;
}

static std::vector<double> collect_vector_capacity_growth_ratios()
{
	std::vector<int> values;
	std::size_t previous_capacity = 0;
	std::vector<double> growth_ratios;
	growth_ratios.reserve(32);

	for (int i = 0; i < k_vector_push_count; ++i)
	{
		values.push_back(i);

		std::size_t const current_capacity = values.capacity();

		assert(current_capacity >= values.size());

		if (current_capacity != previous_capacity)
		{
			if (previous_capacity > 0)
			{
				growth_ratios.push_back(static_cast<double>(current_capacity)
					/ static_cast<double>(previous_capacity));
			}

			previous_capacity = current_capacity;
		}
	}

	return growth_ratios;
}

static void test_vector_capacity_monotonic_and_growth_factor()
{
	std::vector<double> const growth_ratios = collect_vector_capacity_growth_ratios();

	assert(!growth_ratios.empty());

	for (double const ratio : growth_ratios)
	{
		assert(ratio >= 1.4);
		assert(ratio <= 2.1);
	}

	double const median = growth_ratios[growth_ratios.size() / 2];

	for (double const ratio : growth_ratios)
	{
		double const lo = median * 0.75;
		double const hi = median * 1.25;
		assert(ratio >= lo);
		assert(ratio <= hi);
	}
}

static std::size_t deque_max_run_adjacent_stride()
{
	std::deque<int> values;

	for (int i = 0; i < k_deque_push_count; ++i)
	{
		values.push_back(i);
	}

	std::size_t max_run_same_stride = 0;
	std::size_t current_run = 0;

	for (std::size_t index = 1; index < values.size(); ++index)
	{
		std::byte * const previous = reinterpret_cast<std::byte *>(std::addressof(values[index - 1]));
		std::byte * const current = reinterpret_cast<std::byte *>(std::addressof(values[index]));
		std::ptrdiff_t const delta = current - previous;

		if (delta == static_cast<std::ptrdiff_t>(sizeof(int)))
		{
			++current_run;
		}
		else
		{
			if (current_run > max_run_same_stride)
			{
				max_run_same_stride = current_run;
			}

			current_run = 0;
		}
	}

	if (current_run > max_run_same_stride)
	{
		max_run_same_stride = current_run;
	}

	return max_run_same_stride;
}

static void test_deque_adjacent_address_gaps()
{
	std::size_t const max_run_same_stride = deque_max_run_adjacent_stride();

	assert(max_run_same_stride > 0);

	std::size_t const chunk_elements_lower_bound = max_run_same_stride + 1;
	assert(chunk_elements_lower_bound >= 2);
	assert(chunk_elements_lower_bound <= static_cast<std::size_t>(k_deque_push_count));
}

static void test_vector_element_address_changes_after_reallocation()
{
	std::vector<int> values;

	values.push_back(0);

	int * const first_address = std::addressof(values.front());

	while (std::addressof(values.front()) == first_address)
	{
		values.push_back(static_cast<int>(values.size()));
	}

	assert(values.size() > 1u);
}

static void print_empirical_summary()
{
	std::vector<double> const ratios = collect_vector_capacity_growth_ratios();

	double const median = ratios.empty() ? 0.0 : ratios[ratios.size() / 2];

	std::cout << "10.01 vector: median capacity growth factor (step ratio) ~ " << median << '\n';

	std::size_t const run = deque_max_run_adjacent_stride();
	std::size_t const chunk_lb = run + 1;

	std::cout << "10.01 deque: longest run of adjacent int addresses with stride sizeof(int): " << run
		  << "; lower bound on contiguous block size (elements) >= " << chunk_lb << '\n';
}

int main()
{
	test_vector_capacity_monotonic_and_growth_factor();
	test_vector_element_address_changes_after_reallocation();
	test_deque_adjacent_address_gaps();

	print_empirical_summary();

	return 0;
}
