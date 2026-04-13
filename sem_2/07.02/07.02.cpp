#include <compare>
#include <exception>
#include <iostream>
#include <istream>
#include <numeric>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>


class Exception : public std::exception
{
public:

	explicit Exception(std::string message) : m_message(std::move(message)) {}

	auto what() const noexcept -> const char * override
	{
		return m_message.c_str();
	}

private:

	std::string m_message;
};


template < typename T >
struct addable
{
	friend T operator+(T lhs, T const & rhs) { return lhs += rhs; }
};

template < typename T >
struct subtractable
{
	friend T operator-(T lhs, T const & rhs) { return lhs -= rhs; }
};

template < typename T >
struct multipliable
{
	friend T operator*(T lhs, T const & rhs) { return lhs *= rhs; }
};

template < typename T >
struct dividable
{
	friend T operator/(T lhs, T const & rhs) { return lhs /= rhs; }
};

template < typename T >
struct incrementable
{
	friend T operator++(T & x, int)
	{
		T tmp(x);
		++x;
		return tmp;
	}
};

template < typename T >
struct decrementable
{
	friend T operator--(T & x, int)
	{
		T tmp(x);
		--x;
		return tmp;
	}
};

///////////////////////////////////////////////////////////////////////////////

template < typename T >
class Rational

	: public    addable < Rational < T > >
	, public subtractable < Rational < T > >
	, public multipliable < Rational < T > >
	, public   dividable < Rational < T > >
	, public incrementable < Rational < T > >
	, public decrementable < Rational < T > >
{
public:

	Rational(T num = T{0}, T den = T{1}) : m_num(num), m_den(den)
	{
		if (m_den == T{0})
		{
			throw Exception("Rational: zero denominator");
		}
		reduce();
	}

	explicit operator double() const
	{
		return static_cast < double > (m_num) / static_cast < double > (m_den);
	}

	auto & operator+=(Rational const & other)
	{
		auto lcm = std::lcm(m_den, other.m_den);
		m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
		m_den = lcm;
		reduce();
		return *this;
	}

	auto & operator-=(Rational const & other)
	{
		return *this += Rational(-other.m_num, other.m_den);
	}

	auto & operator*=(Rational const & other)
	{
		m_num *= other.m_num;
		m_den *= other.m_den;
		reduce();
		return *this;
	}

	auto & operator/=(Rational const & other)
	{
		return *this *= Rational(other.m_den, other.m_num);
	}

	auto & operator++() { *this += 1; return *this; }
	auto & operator--() { *this -= 1; return *this; }

	friend auto operator<=>(Rational const & lhs, Rational const & rhs) -> std::strong_ordering
	{
		auto lhs_value = static_cast < long long > (lhs.m_num) * static_cast < long long > (rhs.m_den);
		auto rhs_value = static_cast < long long > (rhs.m_num) * static_cast < long long > (lhs.m_den);
		if (lhs_value < rhs_value) return std::strong_ordering::less;
		if (lhs_value > rhs_value) return std::strong_ordering::greater;
		return std::strong_ordering::equal;
	}

	friend auto operator==(Rational const & lhs, Rational const & rhs) -> bool
	{
		return (lhs <=> rhs) == std::strong_ordering::equal;
	}

	friend auto & operator>>(std::istream & stream, Rational & rational)
	{
		return (stream >> rational.m_num).ignore() >> rational.m_den;
	}

	friend auto & operator<<(std::ostream & stream, Rational const & rational)
	{
		return stream << rational.m_num << '/' << rational.m_den;
	}

private:

	void reduce()
	{
		if (m_den < 0)
		{
			m_num = -m_num;
			m_den = -m_den;
		}
		auto g = std::gcd(m_num, m_den);
		m_num /= g;
		m_den /= g;
	}

	T m_num = T{0};
	T m_den = T{1};
};

///////////////////////////////////////////////////////////////////////////////

int main()
{

	try
	{
		Rational < int > r(1, 0);
	}
	catch (const std::exception & exception)
	{
		std::cerr << "std::exception: " << exception.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "unknown exception" << std::endl;
	}

//  ---------------------------------------------------------------------------
//  std::bad_alloc
//  ---------------------------------------------------------------------------

	try
	{
		auto size = static_cast < std::size_t > (-1);
		auto * data = new int[size];
		delete[] data;
	}
	catch (const std::exception & exception)
	{
		std::cerr << "std::exception: " << exception.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "unknown exception" << std::endl;
	}

//  ---------------------------------------------------------------------------
//  std::bad_variant_access
//  ---------------------------------------------------------------------------

	try
	{
		std::variant < int, double > v = 42;
		std::get < double > (v);
	}
	catch (const std::exception & exception)
	{
		std::cerr << "std::exception: " << exception.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "unknown exception" << std::endl;
	}

//  ---------------------------------------------------------------------------
//  std::bad_optional_access
//  ---------------------------------------------------------------------------

	try
	{
		std::optional < int > opt;
		opt.value();
	}
	catch (const std::exception & exception)
	{
		std::cerr << "std::exception: " << exception.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "unknown exception" << std::endl;
	}

//  ---------------------------------------------------------------------------
//  std::length_error
//  ---------------------------------------------------------------------------

	try
	{
		std::vector < int > vec;
		vec.resize(vec.max_size() + 1);
	}
	catch (const std::length_error & exception)
	{
		std::cerr << "std::length_error: " << exception.what() << std::endl;
	}
	catch (const std::exception & exception)
	{
		std::cerr << "std::exception: " << exception.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "unknown exception" << std::endl;
	}

//  ---------------------------------------------------------------------------
//  std::out_of_range
//  ---------------------------------------------------------------------------

	try
	{
		std::vector < int > vec = { 1, 2, 3 };
		vec.at(10);
	}
	catch (const std::out_of_range & exception)
	{
		std::cerr << "std::out_of_range: " << exception.what() << std::endl;
	}
	catch (const std::exception & exception)
	{
		std::cerr << "std::exception: " << exception.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "unknown exception" << std::endl;
	}
}