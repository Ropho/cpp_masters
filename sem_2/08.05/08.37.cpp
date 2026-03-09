#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

/////////////////////////////////////////////////////////////////////////////////

template < typename D = std::chrono::duration < double > >
class Timer
{
public:

	Timer(std::string const & scope) : Timer(scope, true) {}

	Timer(std::string const & scope, bool auto_start)
		: m_scope(scope), m_is_running(false), m_stopped_ever(false)
	{
		if (auto_start)
		{
			m_begin     = clock_t::now();
			m_is_running = true;
		}
	}

	~Timer()
	{
		if (m_is_running)
		{
			auto elapsed = clock_t::now() - m_begin;
			m_durations.push_back(
				std::chrono::duration_cast < std::chrono::duration < double > > (elapsed));
		}

		if (!m_durations.empty())
		{
			if (!m_stopped_ever && m_durations.size() == 1)
			{
				std::cout << m_scope << " " << std::chrono::duration_cast < D > (m_durations[0]).count() << "\n";
			}
			else
			{
				std::cout << m_scope << " : average of " << m_durations.size() << " measurements = " << average() << "\n";
			}
		}
	}

	void start()
	{
		assert(!m_is_running);
		m_begin     = clock_t::now();
		m_is_running = true;
	}

	void stop()
	{
		assert(m_is_running);
		auto end = clock_t::now();
		m_durations.push_back(
			std::chrono::duration_cast < std::chrono::duration < double > > (end - m_begin));
		m_is_running  = false;
		m_stopped_ever = true;
	}

	double average() const
	{
		if (m_durations.empty()) return 0.0;
		auto sum = std::accumulate(
			m_durations.begin(),
			m_durations.end(),
			std::chrono::duration < double > (0.0));
		return sum.count() / static_cast < double > (m_durations.size());
	}

private:

	using clock_t = std::chrono::steady_clock;

	std::string m_scope;
	bool m_is_running;
	bool m_stopped_ever;
	clock_t::time_point m_begin;
	std::vector < std::chrono::duration < double > > m_durations;
};

/////////////////////////////////////////////////////////////////////////////////

auto calculate(std::size_t size)
{
	auto x = 0.0;
	for (std::size_t i = 0; i < size; ++i)
	{
		x += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
	}
	return x;
}

/////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6)
{
	return std::abs(x - y) < epsilon;
}

/////////////////////////////////////////////////////////////////////////////////

int main()
{
	{
		Timer <> timer("main : single");
		volatile auto result = calculate(1'000'000);
		(void) result;
		assert(equal(result, 1'000'000));
	}

	{
		Timer <> timer("main : series", false);
		for (int i = 0; i < 5; ++i)
		{
			timer.start();
			volatile auto result = calculate(1'000'000);
			(void) result;
			timer.stop();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
