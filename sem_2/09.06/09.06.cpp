#include <boost/iterator/iterator_facade.hpp>

#include <cassert>
#include <cstddef>
#include <iterator>

class Iterator
	: public boost::iterator_facade<
		  Iterator,
		  int,
		  boost::forward_traversal_tag,
		  int>
{
public:

	using iterator_category = std::forward_iterator_tag;

	Iterator()
		: m_current(0)
		, m_next(1)
	{
	}

private:

	friend class boost::iterator_core_access;

	void increment()
	{
		int const computed = m_current + m_next;
		m_current = m_next;
		m_next = computed;
	}

	bool equal(Iterator const & other) const
	{
		return m_current == other.m_current && m_next == other.m_next;
	}

	int dereference() const
	{
		return m_current;
	}

	int m_current{};
	int m_next{};
};

namespace
{
int fibonacci_sum_two_variables(std::size_t count)
{
	int a = 0;
	int b = 1;
	int sum = 0;

	for (std::size_t i = 0; i < count; ++i)
	{
		sum += a;
		int const c = a + b;
		a = b;
		b = c;
	}

	return sum;
}

int fibonacci_sum_iterator(std::size_t count)
{
	int sum = 0;

	for (Iterator it; count > 0; --count, ++it)
	{
		sum += *it;
	}

	return sum;
}
} // namespace

int main()
{
	Iterator a;
	Iterator b;

	assert(*a == 0);
	assert(a == b);

	++a;

	assert(*a == 1);
	assert(!(a == b));

	Iterator c = a;
	assert(c == a);

	c++;

	assert(*c == 1);

	Iterator reaches_two;
	++reaches_two;
	++reaches_two;
	++reaches_two;
	assert(*reaches_two == 2);

	Iterator x;
	assert(*x == 0);
	*x;
	assert(*x == 0);

	Iterator p;
	Iterator q = p;
	++p;
	++q;
	assert(p == q);

	std::size_t const terms = 12;

	assert(fibonacci_sum_two_variables(terms) == fibonacci_sum_iterator(terms));

	return 0;
}
