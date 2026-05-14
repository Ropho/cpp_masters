#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace compat
{
#if !defined(__cpp_lib_ranges_stride)
template <std::ranges::view View>
requires std::ranges::forward_range<View>
class StrideView
	: public std::ranges::view_interface<StrideView<View>>
{
private:

	using base_iterator = std::ranges::iterator_t<View>;
	using base_sentinel = std::ranges::sentinel_t<View>;
	using difference_type = std::ranges::range_difference_t<View>;

	class Iterator
	{
	public:

		using iterator_concept = std::forward_iterator_tag;
		using iterator_category = std::forward_iterator_tag;
		using value_type = std::ranges::range_value_t<View>;
		using difference_type = std::ranges::range_difference_t<View>;

		Iterator() = default;

		auto operator*() const -> decltype(auto)
		{
			return *m_current;
		}

		auto operator++() -> Iterator &
		{
			for (difference_type i = 0; i < m_step && m_current != m_end; ++i)
			{
				++m_current;
			}

			return *this;
		}

		auto operator++(int) -> Iterator
		{
			auto copy = *this;
			++(*this);
			return copy;
		}

		friend auto operator==(Iterator const & iterator,
		                       std::default_sentinel_t) -> bool
		{
			return iterator.m_current == iterator.m_end;
		}

		friend auto operator==(std::default_sentinel_t sentinel,
		                       Iterator const & iterator) -> bool
		{
			return iterator == sentinel;
		}

	private:

		friend class StrideView;

		Iterator(base_iterator current, base_sentinel end, difference_type step)
			: m_current(current)
			, m_end(end)
			, m_step(step)
		{
		}

		base_iterator m_current{};
		base_sentinel m_end{};
		difference_type m_step{1};
	};

public:

	StrideView(View base, difference_type step)
		: m_base(std::move(base))
		, m_step(step)
	{
		assert(m_step > 0);
	}

	auto begin() -> Iterator
	{
		return Iterator(std::ranges::begin(m_base), std::ranges::end(m_base), m_step);
	}

	auto end() -> std::default_sentinel_t
	{
		return std::default_sentinel;
	}

private:

	View m_base;
	difference_type m_step{1};
};

namespace views
{
class Stride
{
public:

	struct Closure
	{
		std::ptrdiff_t step{};

		template <std::ranges::viewable_range Range>
		friend auto operator|(Range && range, Closure closure)
		{
			return Stride{}(std::forward<Range>(range), closure.step);
		}
	};

	template <std::ranges::viewable_range Range>
	auto operator()(Range && range, std::ptrdiff_t step) const
	{
		assert(step > 0);

		return StrideView<std::views::all_t<Range>>(
			std::views::all(std::forward<Range>(range)), step);
	}

	auto operator()(std::ptrdiff_t step) const -> Closure
	{
		assert(step > 0);

		return Closure{step};
	}
};

inline constexpr Stride stride;
} // namespace views
#endif
} // namespace compat

////////////////////////////////////////////////////////////////////////////////

class Fibonacci
	: public std::ranges::view_interface<Fibonacci>
{
public:

	using value_type = unsigned long long;

private:

	class Iterator
	{
	public:

		using iterator_concept = std::forward_iterator_tag;
		using iterator_category = std::forward_iterator_tag;
		using value_type = Fibonacci::value_type;
		using difference_type = std::ptrdiff_t;

		Iterator() = default;

		auto operator*() const -> value_type
		{
			return m_current;
		}

		auto operator++() -> Iterator &
		{
			auto const next = m_current + m_next;
			m_current = m_next;
			m_next = next;
			++m_index;

			return *this;
		}

		auto operator++(int) -> Iterator
		{
			auto copy = *this;
			++(*this);
			return copy;
		}

		friend auto operator==(Iterator const & lhs, Iterator const & rhs) -> bool
		{
			return lhs.m_index == rhs.m_index;
		}

	private:

		friend class Fibonacci;

		Iterator(std::size_t index, value_type current, value_type next)
			: m_index(index)
			, m_current(current)
			, m_next(next)
		{
		}

		std::size_t m_index{};
		value_type m_current{};
		value_type m_next{1};
	};

public:

	explicit Fibonacci(std::size_t count)
		: m_count(count)
	{
	}

	auto begin() const -> Iterator
	{
		return Iterator(0, 0, 1);
	}

	auto end() const -> Iterator
	{
		return Iterator(m_count, 0, 1);
	}

private:

	std::size_t m_count{};
};

static_assert(std::ranges::view<Fibonacci>);
static_assert(std::ranges::forward_range<Fibonacci>);

////////////////////////////////////////////////////////////////////////////////

template <std::ranges::input_range Range>
auto to_vector(Range && range)
{
	using value_type = std::ranges::range_value_t<Range>;

	std::vector<value_type> result;
	std::ranges::copy(range, std::back_inserter(result));
	return result;
}

template <std::ranges::input_range Range, typename Output, typename Predicate, typename Operation>
auto transform_if(Range && range,
                  Output output,
                  Predicate predicate,
                  Operation operation) -> Output
{
	using value_type = std::ranges::range_value_t<Range>;

	std::vector<value_type> filtered;
	std::ranges::copy_if(range, std::back_inserter(filtered), predicate);

	return std::ranges::transform(filtered, output, operation).out;
}

template <std::ranges::sized_range Expected, std::ranges::sized_range Predicted>
requires std::ranges::input_range<Expected> && std::ranges::input_range<Predicted>
auto mean_absolute_error(Expected const & expected,
                         Predicted const & predicted) -> double
{
	assert(std::ranges::size(expected) == std::ranges::size(predicted));
	assert(!std::ranges::empty(expected));

	auto const total = std::transform_reduce(
		std::ranges::begin(expected),
		std::ranges::end(expected),
		std::ranges::begin(predicted),
		0.0,
		std::plus<>(),
		[](auto const expected_value, auto const predicted_value)
		{
			return std::abs(static_cast<double>(expected_value) -
			                static_cast<double>(predicted_value));
		});

	return total / static_cast<double>(std::ranges::size(expected));
}

template <std::ranges::sized_range Expected, std::ranges::sized_range Predicted>
requires std::ranges::input_range<Expected> && std::ranges::input_range<Predicted>
auto mean_squared_error(Expected const & expected,
                        Predicted const & predicted) -> double
{
	assert(std::ranges::size(expected) == std::ranges::size(predicted));
	assert(!std::ranges::empty(expected));

	auto const total = std::transform_reduce(
		std::ranges::begin(expected),
		std::ranges::end(expected),
		std::ranges::begin(predicted),
		0.0,
		std::plus<>(),
		[](auto const expected_value, auto const predicted_value)
		{
			auto const difference = static_cast<double>(expected_value) -
			                        static_cast<double>(predicted_value);

			return difference * difference;
		});

	return total / static_cast<double>(std::ranges::size(expected));
}

auto fibonacci_number(std::size_t n) -> Fibonacci::value_type
{
	Fibonacci fibonacci(n + 1);
	auto iterator = std::ranges::next(fibonacci.begin(), static_cast<std::ptrdiff_t>(n));

	return *iterator;
}

////////////////////////////////////////////////////////////////////////////////

auto almost_equal(double lhs, double rhs) -> bool
{
	return std::abs(lhs - rhs) < 1e-9;
}

void test_ranges_algorithms()
{
	std::vector<int> data = {1, 2, 1, 3};
	std::ranges::replace(data, 1, 9);
	assert(data == std::vector<int>({9, 2, 9, 3}));

	std::vector<int> filled(4);
	std::ranges::fill(filled, 7);
	assert(filled == std::vector<int>({7, 7, 7, 7}));

	std::vector<int> duplicates = {1, 1, 2, 2, 2, 3};
	auto tail = std::ranges::unique(duplicates);
	duplicates.erase(tail.begin(), tail.end());
	assert(duplicates == std::vector<int>({1, 2, 3}));

	std::vector<int> rotated = {1, 2, 3, 4, 5};
	std::ranges::rotate(rotated, std::ranges::next(rotated.begin(), 2));
	assert(rotated == std::vector<int>({3, 4, 5, 1, 2}));

	std::vector<int> population = {1, 2, 3, 4, 5, 6};
	std::vector<int> sample;
	std::mt19937 generator{42};
	std::ranges::sample(population, std::back_inserter(sample), 3, generator);
	assert(sample.size() == 3);

	for (auto const value : sample)
	{
		assert(std::ranges::find(population, value) != population.end());
	}

	std::ranges::sort(sample);
	auto duplicate_sample_tail = std::ranges::unique(sample);
	assert(duplicate_sample_tail.begin() == sample.end());
}

void test_transform_if()
{
	std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
	std::vector<int> transformed;

	transform_if(
		numbers,
		std::back_inserter(transformed),
		[](int value) { return value % 2 == 0; },
		[](int value) { return value * value; });

	assert(transformed == std::vector<int>({4, 16, 36}));
}

void test_errors()
{
	std::vector<double> expected = {1.0, 2.0, 3.0};
	std::vector<double> predicted = {1.5, 1.5, 2.0};

	assert(almost_equal(mean_absolute_error(expected, predicted), 2.0 / 3.0));
	assert(almost_equal(mean_squared_error(expected, predicted), 0.5));
}

void test_views()
{
	std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

	auto even = numbers | std::views::filter([](int value) { return value % 2 == 0; });
	assert(to_vector(even) == std::vector<int>({2, 4, 6}));

	auto dropped = numbers | std::views::drop(2);
	assert(to_vector(dropped) == std::vector<int>({3, 4, 5, 6}));

	std::vector<std::vector<int>> groups = {{1, 2}, {3}, {4, 5}};
	auto joined = groups | std::views::join;
	assert(to_vector(joined) == std::vector<int>({1, 2, 3, 4, 5}));

	std::vector<char> letters = {'a', 'b', 'c'};
	std::vector<std::string> zipped;

	for (auto [number, letter] : std::views::zip(numbers, letters))
	{
		zipped.push_back(std::string(1, letter) + std::to_string(number));
	}

	assert(zipped == std::vector<std::string>({"a1", "b2", "c3"}));

#if defined(__cpp_lib_ranges_stride)
	auto strided = numbers | std::views::stride(2);
#else
	auto strided = numbers | compat::views::stride(2);
#endif
	assert(to_vector(strided) == std::vector<int>({1, 3, 5}));
}

void test_fibonacci_view()
{
	Fibonacci fibonacci(10);
	auto values = to_vector(fibonacci);

	assert(values == std::vector<Fibonacci::value_type>(
		{0, 1, 1, 2, 3, 5, 8, 13, 21, 34}));
	assert(fibonacci_number(0) == 0);
	assert(fibonacci_number(1) == 1);
	assert(fibonacci_number(10) == 55);

	auto tail = Fibonacci(8) | std::views::drop(5);
	assert(to_vector(tail) == std::vector<Fibonacci::value_type>({5, 8, 13}));
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
	std::size_t n = 10;

	if (argc >= 2)
	{
		n = static_cast<std::size_t>(std::stoull(argv[1]));
	}

	test_ranges_algorithms();
	test_transform_if();
	test_errors();
	test_views();
	test_fibonacci_view();

	std::cout << "F(" << n << ") = " << fibonacci_number(n) << '\n';
	std::cout << "All tests passed" << '\n';

	return 0;
}
