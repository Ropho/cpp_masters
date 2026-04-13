////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <vector>

#include <benchmark/benchmark.h>

#include "sort.hpp"

////////////////////////////////////////////////////////////////////////////////

static void BM_Sort_ReverseDouble10000(benchmark::State & state)
{
	const auto threshold = static_cast < std::size_t > (state.range(0));
	constexpr std::size_t n = 10'000;

	std::vector < double > vec(n);

	for (std::size_t i = 0; i < n; ++i)
	{
		vec[i] = static_cast < double > (n - i);
	}

	for (auto _ : state)
	{
		state.PauseTiming();
		for (std::size_t i = 0; i < n; ++i)
		{
			vec[i] = static_cast < double > (n - i);
		}
		state.ResumeTiming();

		sort(vec, threshold);
	}
}

////////////////////////////////////////////////////////////////////////////////

BENCHMARK(BM_Sort_ReverseDouble10000)
	->Arg(0)
	->Arg(2)
	->Arg(5)
	->Arg(10)
	->Arg(15)
	->Arg(20)
	->Arg(30)
	->Arg(50)
	->Arg(100)
	->Unit(benchmark::kMicrosecond);

////////////////////////////////////////////////////////////////////////////////

BENCHMARK_MAIN();

////////////////////////////////////////////////////////////////////////////////
