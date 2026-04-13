#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <new>
#include <utility>
#include <vector>

class Allocator
{
public:

	virtual ~Allocator() = default;

	virtual void * allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) = 0;

	virtual void deallocate(void * pointer) = 0;

	Allocator(Allocator const &) = delete;

	Allocator & operator=(Allocator const &) = delete;

protected:

	Allocator() = default;

	template<typename T>
	T * get(void * pointer) const
	{
		return static_cast<T *>(pointer);
	}

	static constexpr std::align_val_t s_alignment{ alignof(std::max_align_t) };
};

class LinearBumpAllocator final : public Allocator
{
public:

	explicit LinearBumpAllocator(std::size_t pool_size)
		: m_size(pool_size)
	{
		m_begin = ::operator new(m_size, s_alignment);
	}

	~LinearBumpAllocator() override
	{
		::operator delete(m_begin, m_size, s_alignment);
	}

	void * allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) override
	{
		void * cursor = get<std::byte>(m_begin) + m_offset;
		std::size_t free_space = m_size - m_offset;

		if (void * const aligned = std::align(alignment, size, cursor, free_space))
		{
			m_offset = m_size - free_space + size;
			return aligned;
		}

		return nullptr;
	}

	void deallocate(void * /*pointer*/) override
	{
	}

private:

	std::size_t m_size{};
	std::size_t m_offset{};
	void * m_begin{};
};

class StackAllocator final : public Allocator
{
public:

	struct alignas(std::max_align_t) Header
	{
		std::size_t span_to_prior_offset{};
	};

	explicit StackAllocator(std::size_t pool_size)
		: m_size(pool_size)
	{
		m_begin = ::operator new(m_size, s_alignment);
	}

	~StackAllocator() override
	{
		::operator delete(m_begin, m_size, s_alignment);
	}

	void * allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) override
	{
		void * cursor = get<std::byte>(m_begin) + m_offset + sizeof(Header);
		std::size_t free_space = m_size - m_offset - sizeof(Header);

		if (void * const aligned = std::align(alignment, size, cursor, free_space))
		{
			Header * const header = get<Header>(get<std::byte>(aligned) - sizeof(Header));
			header->span_to_prior_offset = static_cast<std::size_t>(
				std::distance(get<std::byte>(m_begin) + m_offset, get<std::byte>(aligned)));
			m_offset = static_cast<std::size_t>(get<std::byte>(aligned) - get<std::byte>(m_begin)) + size;
			return aligned;
		}

		return nullptr;
	}

	void deallocate(void * pointer) override
	{
		Header * const header = get<Header>(get<std::byte>(pointer) - sizeof(Header));
		m_offset = static_cast<std::size_t>(get<std::byte>(pointer) - get<std::byte>(m_begin))
			- header->span_to_prior_offset;
	}

private:

	std::size_t m_size{};
	std::size_t m_offset{};
	void * m_begin{};
};

class PoolAllocator final : public Allocator
{
public:

	struct Node
	{
		Node * next{};
	};

	explicit PoolAllocator(std::size_t slab_size, std::size_t block_step)
		: m_size(slab_size)
		, m_step(block_step)
	{
		assert(m_size % m_step == 0);
		assert(m_step >= sizeof(Node));

		make_list();
		m_begin = m_head;
	}

	~PoolAllocator() override
	{
		for (void * const slab : m_lists)
		{
			::operator delete(slab, m_size, s_alignment);
		}
	}

	void * allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) override
	{
		(void)alignment;
		if (size > m_step)
		{
			return nullptr;
		}

		if (!m_head)
		{
			if (m_offset == m_lists.size())
			{
				make_list();
			}
			else
			{
				m_head = get<Node>(m_lists[m_offset - 1]);
			}
		}

		Node * const node = m_head;

		if (node->next == nullptr)
		{
			std::byte * const next_block = get<std::byte>(node) + m_step;

			if (next_block != get<std::byte>(m_lists[m_offset - 1]) + m_size)
			{
				m_head = get<Node>(next_block);
				m_head->next = nullptr;
			}
			else
			{
				m_head = node->next;
			}
		}
		else
		{
			m_head = node->next;
		}

		return node;
	}

	void deallocate(void * pointer) override
	{
		Node * const node = get<Node>(pointer);
		node->next = m_head;
		m_head = node;
	}

private:

	void make_list()
	{
		m_head = get<Node>(::operator new(m_size, s_alignment));
		m_head->next = nullptr;
		++m_offset;
		m_lists.push_back(m_head);
	}

	std::size_t m_size{};
	std::size_t m_step{};
	std::size_t m_offset{};
	void * m_begin{};
	Node * m_head{};
	std::vector<void *> m_lists{};
};

class FreeListAllocator final : public Allocator
{
public:

	struct Node
	{
		std::size_t size{};
		Node * next{};
	};

	using Header = Node;

	explicit FreeListAllocator(std::size_t pool_size)
		: m_size(pool_size)
	{
		assert(m_size >= sizeof(Node) + 1);

		m_begin = ::operator new(m_size, s_alignment);
		m_head = get<Node>(m_begin);
		m_head->size = m_size - sizeof(Header);
		m_head->next = nullptr;
	}

	~FreeListAllocator() override
	{
		::operator delete(m_begin, m_size, s_alignment);
	}

	void * allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) override
	{
		(void)alignment;

		if (size == 0)
		{
			size = 1;
		}

		std::byte * const pool = get<std::byte>(m_begin);
		void * end_void = static_cast<void *>(pool + sizeof(Header) + size);
		std::size_t space = 2 * alignof(Header);
		void * next_void = end_void;

		if (void * const aligned_void = std::align(alignof(Header), sizeof(Header), next_void, space))
		{
			std::byte * const aligned = get<std::byte>(aligned_void);
			std::byte * const end = get<std::byte>(end_void);
			std::size_t padding = static_cast<std::size_t>(aligned - end);

			auto const found = find_first(size + padding);
			Node * const current = found.first;
			Node * const previous = found.second;

			if (current == nullptr)
			{
				return nullptr;
			}

			if (current->size >= size + padding + sizeof(Node) + 1)
			{
				std::size_t const step = sizeof(Header) + size + padding;
				Node * const rest = get<Node>(get<std::byte>(current) + step);
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

			Header * const header = get<Header>(current);
			header->size = size + padding;

			return get<std::byte>(current) + sizeof(Header);
		}

		return nullptr;
	}

	void deallocate(void * pointer) override
	{
		if (pointer == nullptr)
		{
			return;
		}

		Node * const node = get<Node>(get<std::byte>(pointer) - sizeof(Header));

		Node * previous = nullptr;
		Node * current = m_head;

		while (current != nullptr && get<std::byte>(node) >= get<std::byte>(current))
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

private:

	std::pair<Node *, Node *> find_first(std::size_t required) const
	{
		Node * current = m_head;
		Node * prev = nullptr;

		while (current != nullptr && required > current->size)
		{
			prev = current;
			current = current->next;
		}

		return { current, prev };
	}

	void merge(Node * previous, Node * node)
	{
		if (node->next != nullptr &&
			get<std::byte>(node) + sizeof(Header) + node->size == get<std::byte>(node->next))
		{
			node->size += sizeof(Header) + node->next->size;
			node->next = node->next->next;
		}

		if (previous != nullptr &&
			get<std::byte>(previous) + sizeof(Header) + previous->size == get<std::byte>(node))
		{
			previous->size += sizeof(Header) + node->size;
			previous->next = node->next;
		}
	}

	std::size_t m_size{};
	void * m_begin{};
	Node * m_head{};
};

static void exercise_polymorphic(Allocator & allocator, std::size_t bytes)
{
	void * const block = allocator.allocate(bytes);
	assert(block != nullptr);
	allocator.deallocate(block);
}

int main()
{
	LinearBumpAllocator linear(1u << 12);
	StackAllocator stack(1u << 12);
	PoolAllocator pool(64, 8);
	FreeListAllocator free_list(1u << 10);

	std::vector<Allocator *> strategies{
		&linear,
		&stack,
		&pool,
		&free_list,
	};

	for (Allocator * const strategy : strategies)
	{
		if (dynamic_cast<PoolAllocator *>(strategy) != nullptr)
		{
			exercise_polymorphic(*strategy, 8);
		}
		else
		{
			exercise_polymorphic(*strategy, 32);
		}
	}

	FreeListAllocator checker(1u << 10);
	assert(checker.allocate(16) != nullptr);
	void * const x = checker.allocate(16);
	void * const y = checker.allocate(16);
	assert(checker.allocate(16) != nullptr);
	checker.deallocate(y);
	checker.deallocate(x);
	void * const z = checker.allocate(32);
	assert(z == x);
	(void)z;

	return 0;
}
