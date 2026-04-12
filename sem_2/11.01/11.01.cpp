#include <cassert>
#include <cstddef>

class Wrapper
{
public:

	using fn_type = Wrapper (*)();

	explicit Wrapper(fn_type pointer) : m_pointer(pointer) {}

	fn_type operator*() const
	{
		return m_pointer;
	}

	explicit operator fn_type() const
	{
		return m_pointer;
	}

private:

	fn_type m_pointer = nullptr;
};

Wrapper test();

namespace
{
std::size_t g_calls = 0;
}

Wrapper test()
{
	++g_calls;

	return Wrapper(+test);
}

int main()
{
	g_calls = 0;

	Wrapper function = test();

	assert(g_calls == 1);

	(*function)();

	assert(g_calls == 2);

	return 0;
}
