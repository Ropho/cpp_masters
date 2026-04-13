#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>
#include <random>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

enum class FreeBlockSearch : std::uint8_t
{
	first_fit,
	best_fit,
};

class Allocator
{
public:

	explicit Allocator(std::size_t pool_size, FreeBlockSearch policy);

	Allocator(Allocator const &) = delete;

	Allocator & operator=(Allocator const &) = delete;

	Allocator(Allocator &&) = delete;

	Allocator & operator=(Allocator &&) = delete;

	~Allocator();

	void * allocate(std::size_t size);

	void deallocate(void * pointer);

private:

	struct alignas(std::max_align_t) Node
	{
		std::size_t size{};
		Node * next{};
	};

	using Header = Node;

	std::byte * get_byte(void * pointer) const
	{
		return static_cast<std::byte *>(pointer);
	}

	Node * get_node(void * pointer) const
	{
		return static_cast<Node *>(pointer);
	}

	Header * get_header(void * pointer) const
	{
		return static_cast<Header *>(pointer);
	}

	std::pair<Node *, Node *> find_first(std::size_t required) const;

	std::pair<Node *, Node *> find_best(std::size_t required) const;

	std::pair<Node *, Node *> find_for_policy(std::size_t required) const;

	void merge(Node * previous, Node * node);

	FreeBlockSearch m_policy{};

	std::size_t m_size{};

	void * m_begin{};

	Node * m_head{};

	static constexpr std::align_val_t s_alignment{ alignof(std::max_align_t) };
};

Allocator::Allocator(std::size_t pool_size, FreeBlockSearch policy)
	: m_policy(policy)
	, m_size(pool_size)
{
	assert(m_size >= sizeof(Node) + 1);

	m_begin = ::operator new(m_size, s_alignment);
	m_head = get_node(m_begin);
	m_head->size = m_size - sizeof(Header);
	m_head->next = nullptr;
}

Allocator::~Allocator()
{
	::operator delete(m_begin, m_size, s_alignment);
}

std::pair<Allocator::Node *, Allocator::Node *> Allocator::find_first(std::size_t required) const
{
	Node * current = m_head;
	Node * previous = nullptr;

	while (current != nullptr && required > current->size)
	{
		previous = current;
		current = current->next;
	}

	return { current, previous };
}

std::pair<Allocator::Node *, Allocator::Node *> Allocator::find_best(std::size_t required) const
{
	Node * best = nullptr;
	Node * best_previous = nullptr;
	std::size_t best_surplus = (std::numeric_limits<std::size_t>::max)();

	Node * current = m_head;
	Node * previous = nullptr;

	while (current != nullptr)
	{
		if (current->size >= required)
		{
			std::size_t const surplus = current->size - required;

			if (surplus < best_surplus)
			{
				best_surplus = surplus;
				best = current;
				best_previous = previous;

				if (surplus == 0)
				{
					break;
				}
			}
		}

		previous = current;
		current = current->next;
	}

	return { best, best_previous };
}

std::pair<Allocator::Node *, Allocator::Node *> Allocator::find_for_policy(std::size_t required) const
{
	switch (m_policy)
	{
	case FreeBlockSearch::first_fit:
		return find_first(required);

	case FreeBlockSearch::best_fit:
		return find_best(required);
	}

	assert(false);
	return { nullptr, nullptr };
}

void * Allocator::allocate(std::size_t size)
{
	if (size == 0)
	{
		size = 1;
	}

	std::byte * const pool = get_byte(m_begin);
	void * end_void = static_cast<void *>(pool + sizeof(Header) + size);
	std::size_t space = 2 * alignof(Header);
	void * next_void = end_void;

	if (void * const aligned_void = std::align(alignof(Header), sizeof(Header), next_void, space))
	{
		std::byte * const aligned = static_cast<std::byte *>(aligned_void);
		std::byte * const end = static_cast<std::byte *>(end_void);
		std::size_t padding = static_cast<std::size_t>(aligned - end);

		auto const found = find_for_policy(size + padding);
		Node * const current = found.first;
		Node * const previous = found.second;

		if (current == nullptr)
		{
			return nullptr;
		}

		if (current->size >= size + padding + sizeof(Node) + 1)
		{
			std::size_t const step = sizeof(Header) + size + padding;
			Node * const rest = get_node(get_byte(current) + step);

			rest->size = current->size - step;
			rest->next = current->next;
			current->next = rest;
		}
		else
		{
			padding = current->size - size;
		}

		if (previous == nullptr)
		{
			m_head = current->next;
		}
		else
		{
			previous->next = current->next;
		}

		Header * const header = get_header(current);
		header->size = size + padding;

		return get_byte(current) + sizeof(Header);
	}

	return nullptr;
}

void Allocator::deallocate(void * pointer)
{
	if (pointer == nullptr)
	{
		return;
	}

	Node * const node = get_node(get_byte(pointer) - sizeof(Header));

	Node * previous = nullptr;
	Node * current = m_head;

	while (current != nullptr && get_byte(node) >= get_byte(current))
	{
		previous = current;
		current = current->next;
	}

	node->next = current;

	if (previous == nullptr)
	{
		m_head = node;
	}
	else
	{
		previous->next = node;
	}

	merge(previous, node);
}

void Allocator::merge(Node * previous, Node * node)
{
	if (node->next != nullptr &&
		get_byte(node) + sizeof(Header) + node->size == get_byte(node->next))
	{
		node->size += sizeof(Header) + node->next->size;
		node->next = node->next->next;
	}

	if (previous != nullptr &&
		get_byte(previous) + sizeof(Header) + previous->size == get_byte(node))
	{
		previous->size += sizeof(Header) + node->size;
		previous->next = node->next;
	}
}

static void run_allocation_pattern(Allocator & allocator, std::vector<void *> & slots, std::mt19937 & engine)
{
	std::uniform_int_distribution<std::size_t> distribution(32, 2048);

	for (std::size_t i = 0; i < slots.size(); ++i)
	{
		slots[i] = allocator.allocate(distribution(engine));
	}

	for (std::size_t i = 0; i < slots.size(); i += 8)
	{
		if (slots[i] != nullptr)
		{
			allocator.deallocate(slots[i]);
			slots[i] = nullptr;
		}
	}

	for (std::size_t i = 0; i < slots.size(); i += 8)
	{
		slots[i] = allocator.allocate(distribution(engine));
	}

	for (std::size_t i = 0; i < slots.size(); ++i)
	{
		if (slots[i] != nullptr)
		{
			allocator.deallocate(slots[i]);
			slots[i] = nullptr;
		}
	}
}

static void BM_AllocatorFirstFit(benchmark::State & state)
{
	constexpr std::size_t pool_bytes = 8 * 1024 * 1024;
	constexpr std::size_t slot_count = 512;

	std::vector<void *> slots(slot_count);
	std::mt19937 engine{ 4'251'707u };

	for (auto _ : state)
	{
		Allocator allocator(pool_bytes, FreeBlockSearch::first_fit);
		run_allocation_pattern(allocator, slots, engine);
		benchmark::DoNotOptimize(slots);
	}
}

static void BM_AllocatorBestFit(benchmark::State & state)
{
	constexpr std::size_t pool_bytes = 8 * 1024 * 1024;
	constexpr std::size_t slot_count = 512;

	std::vector<void *> slots(slot_count);
	std::mt19937 engine{ 4'251'707u };

	for (auto _ : state)
	{
		Allocator allocator(pool_bytes, FreeBlockSearch::best_fit);
		run_allocation_pattern(allocator, slots, engine);
		benchmark::DoNotOptimize(slots);
	}
}

BENCHMARK(BM_AllocatorFirstFit);

BENCHMARK(BM_AllocatorBestFit);

int main()
{
	Allocator allocator(1u << 10, FreeBlockSearch::first_fit);

	assert(allocator.allocate(16) != nullptr);
	void * const x = allocator.allocate(16);
	void * const y = allocator.allocate(16);
	assert(allocator.allocate(16) != nullptr);

	allocator.deallocate(y);
	allocator.deallocate(x);

	void * const z = allocator.allocate(32);
	assert(z == x);
	(void)z;

	benchmark::RunSpecifiedBenchmarks();

	return 0;
}
