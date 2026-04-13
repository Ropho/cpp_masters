#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <list>
#include <memory_resource>
#include <vector>

#include <benchmark/benchmark.h>

static void demo_new_delete_resource()
{
	std::pmr::memory_resource * const resource = std::pmr::new_delete_resource();

	void * const block = resource->allocate(sizeof(int), alignof(int));

	*static_cast<int *>(block) = 1;

	resource->deallocate(block, sizeof(int), alignof(int));
}

static void demo_monotonic_vector_in_buffer()
{
	std::array<std::byte, 1u << 10> buffer{};

	std::pmr::monotonic_buffer_resource resource(buffer.data(), buffer.size());

	std::pmr::polymorphic_allocator<int> allocator(&resource);

	std::pmr::vector<int> vector(allocator);

	vector = { 1, 2, 3, 4, 5 };

	std::size_t index = 0;

	for (std::size_t i = 0; i < vector.size(); ++i, index += sizeof(int))
	{
		assert(static_cast<int>(std::to_integer<std::uint8_t>(buffer[index])) == vector[i]);
	}

	vector.push_back(1);

	index = 0;

	for (std::size_t i = 0; i < vector.size(); ++i, index += sizeof(int))
	{
		assert(static_cast<int>(std::to_integer<std::uint8_t>(buffer[index])) == vector[i]);
	}
}

static void BM_ListStd(benchmark::State & state)
{
	for (auto _ : state)
	{
		std::list<int> list;

		for (std::size_t i = 0; i < (1u << 10); ++i)
		{
			list.push_back(1);
		}

		benchmark::DoNotOptimize(list);
	}
}

static void BM_ListPmrDefault(benchmark::State & state)
{
	for (auto _ : state)
	{
		std::pmr::list<int> list;

		for (std::size_t i = 0; i < (1u << 10); ++i)
		{
			list.push_back(1);
		}

		benchmark::DoNotOptimize(list);
	}
}

static void BM_ListPmrMonotonicDefaultUpstream(benchmark::State & state)
{
	for (auto _ : state)
	{
		std::pmr::monotonic_buffer_resource resource;

		std::pmr::polymorphic_allocator<int> allocator(&resource);

		std::pmr::list<int> list(allocator);

		for (std::size_t i = 0; i < (1u << 10); ++i)
		{
			list.push_back(1);
		}

		benchmark::DoNotOptimize(list);
	}
}

static void BM_ListPmrMonotonicStackBuffer(benchmark::State & state)
{
	for (auto _ : state)
	{
		std::array<std::byte, 32u << 10> array{};

		std::pmr::monotonic_buffer_resource resource(array.data(), array.size());

		std::pmr::polymorphic_allocator<int> allocator(&resource);

		std::pmr::list<int> list(allocator);

		for (std::size_t i = 0; i < (1u << 10); ++i)
		{
			list.push_back(1);
		}

		benchmark::DoNotOptimize(list);
	}
}

BENCHMARK(BM_ListStd);

BENCHMARK(BM_ListPmrDefault);

BENCHMARK(BM_ListPmrMonotonicDefaultUpstream);

BENCHMARK(BM_ListPmrMonotonicStackBuffer);

int main(int argc, char * argv[])
{
	demo_new_delete_resource();

	demo_monotonic_vector_in_buffer();

	::benchmark::Initialize(&argc, argv);

	if (::benchmark::ReportUnrecognizedArguments(argc, argv))
	{
		return 1;
	}

	::benchmark::RunSpecifiedBenchmarks();

	return 0;
}
