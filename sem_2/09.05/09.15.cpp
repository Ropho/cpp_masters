#include <cassert>
#include <iterator>
#include <memory>

template<typename T>
class List
{
private:

	struct Node
	{
		T x{};
		std::shared_ptr<Node> next{};
		std::weak_ptr<Node> prev{};
	};

	std::shared_ptr<Node> m_head{};
	std::shared_ptr<Node> m_tail{};

public:

	class Iterator
	{
	public:

		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T *;
		using reference = T &;

		Iterator() = default;

		Iterator(List const * owner, std::shared_ptr<Node> node)
			: m_owner(owner)
			, m_node(std::move(node))
		{
		}

		Iterator operator++(int)
		{
			Iterator before = *this;
			++*this;
			return before;
		}

		Iterator & operator++()
		{
			if (m_node)
			{
				m_node = m_node->next;
			}

			return *this;
		}

		Iterator operator--(int)
		{
			Iterator before = *this;
			--*this;
			return before;
		}

		Iterator & operator--()
		{
			if (m_node)
			{
				m_node = m_node->prev.lock();
			}
			else if (m_owner != nullptr)
			{
				m_node = m_owner->m_tail;
			}

			return *this;
		}

		reference operator*() const
		{
			return m_node->x;
		}

		pointer operator->() const
		{
			return &m_node->x;
		}

		friend bool operator==(Iterator const & lhs, Iterator const & rhs)
		{
			return lhs.m_node == rhs.m_node;
		}

		friend bool operator!=(Iterator const & lhs, Iterator const & rhs)
		{
			return !(lhs == rhs);
		}

	private:

		List const * m_owner{};
		std::shared_ptr<Node> m_node{};
	};

	Iterator begin() const
	{
		return Iterator(this, m_head);
	}

	Iterator end() const
	{
		return Iterator(this, nullptr);
	}

	void push_back(T x)
	{
		auto node = std::make_shared<Node>();
		node->x = std::move(x);

		if (!m_head)
		{
			m_head = node;
			m_tail = node;
			return;
		}

		node->prev = m_tail;
		m_tail->next = node;
		m_tail = std::move(node);
	}

	friend class Iterator;
};

int main()
{
	List<int> list;

	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	auto backward = list.end();
	--backward;
	assert(*backward == 3);
	--backward;
	assert(*backward == 2);
	--backward;
	assert(*backward == 1);

	for (auto iterator = std::begin(list); iterator != std::end(list); ++iterator)
	{
		(void)iterator;
	}

	for ([[maybe_unused]] int element : list)
	{
	}
}
