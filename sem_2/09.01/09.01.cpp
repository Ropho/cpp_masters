#include <iostream>
#include <source_location>
#include <string_view>

#define TRACE_PASTE_IMPL(a, b) a##b
#define TRACE_PASTE(a, b) TRACE_PASTE_IMPL(a, b)

#ifdef NDEBUG

#define trace() ((void)0)

#else

#define trace() Tracer const TRACE_PASTE(_tracer_, __LINE__)(std::source_location::current())

#endif

class Tracer
{
public:

	explicit Tracer(std::source_location location = std::source_location::current())
		: m_location{ location }
	{
		write_prefix(k_enter_label);
		write_location(m_location);
	}

	~Tracer()
	{
		write_prefix(k_exit_label);
		write_location(m_location);
	}

	Tracer(Tracer const &) = delete;
	Tracer & operator=(Tracer const &) = delete;
	Tracer(Tracer &&) = delete;
	Tracer & operator=(Tracer &&) = delete;

private:

	static constexpr std::string_view k_enter_label = "enter";
	static constexpr std::string_view k_exit_label = "exit";

	std::source_location m_location{};

	static void write_prefix(std::string_view label)
	{
		std::cout << label << ": ";
	}

	static void write_location(std::source_location const & location)
	{
		std::cout
			<< "file=" << location.file_name()
			<< " line=" << location.line()
			<< " column=" << location.column()
			<< " function=" << location.function_name()
			<< '\n';
	}
};

static void demo_inner()
{
	trace();

	std::cout << "demo_inner body\n";
}

static void demo_outer()
{
	trace();

	demo_inner();

	std::cout << "demo_outer body\n";
}

int main()
{
	trace();

	std::cout << "main: nested trace demonstration\n";

	demo_outer();

	return 0;
}
