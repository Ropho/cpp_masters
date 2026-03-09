#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>

template <typename T>
struct addable
{
    friend T operator+(T lhs, T const& rhs)
    {
        return lhs += rhs;
    }
};

template <typename T>
struct subtractable
{
    friend T operator-(T lhs, T const& rhs)
    {
        return lhs -= rhs;
    }
};

template <typename T>
struct multipliable
{
    friend T operator*(T lhs, T const& rhs)
    {
        return lhs *= rhs;
    }
};

template <typename T>
struct dividable
{
    friend T operator/(T lhs, T const& rhs)
    {
        return lhs /= rhs;
    }
};

template <typename T>
struct incrementable
{
    friend T operator++(T& x, int)
    {
        T tmp(x);
        ++x;
        return tmp;
    }
};

template <typename T>
struct decrementable
{
    friend T operator--(T& x, int)
    {
        T tmp(x);
        --x;
        return tmp;
    }
};


template <typename T>
class Rational
    : public addable<Rational<T>>
    , public subtractable<Rational<T>>
    , public multipliable<Rational<T>>
    , public dividable<Rational<T>>
    , public incrementable<Rational<T>>
    , public decrementable<Rational<T>>
{
public:
    Rational(T num = T{0}, T den = T{1}) : m_num(num), m_den(den)
    {
        reduce();
    }

    explicit operator double() const
    {
        return 1.0 * static_cast<double>(m_num) / static_cast<double>(m_den);
    }

    auto& operator+=(Rational const& other)
    {
        auto lcm = std::lcm(m_den, other.m_den);
        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        m_den = lcm;
        reduce();
        return *this;
    }

    auto& operator-=(Rational const& other)
    {
        return *this += Rational(-other.m_num, other.m_den);
    }

    auto& operator*=(Rational const& other)
    {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }

    auto& operator/=(Rational const& other)
    {
        return *this *= Rational(other.m_den, other.m_num);
    }

    auto& operator++()
    {
        *this += 1;
        return *this;
    }

    auto& operator--()
    {
        *this -= 1;
        return *this;
    }

    friend auto operator<=>(Rational const& lhs, Rational const& rhs) -> std::strong_ordering
    {
        auto lhs_value = static_cast<long long>(lhs.m_num) * static_cast<long long>(rhs.m_den);
        auto rhs_value = static_cast<long long>(rhs.m_num) * static_cast<long long>(lhs.m_den);
        if (lhs_value < rhs_value)
            return std::strong_ordering::less;
        if (lhs_value > rhs_value)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    friend auto operator==(Rational const& lhs, Rational const& rhs) -> bool
    {
        return (lhs <=> rhs) == std::strong_ordering::equal;
    }

    friend auto& operator>>(std::istream& stream, Rational& rational)
    {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    friend auto& operator<<(std::ostream& stream, Rational const& rational)
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

///////////////////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    using R = Rational<int>;
    R x = 1, y(2, 1);

    assert(equal(static_cast<double>(x), 1));

    assert((x += y) == R(3, 1));
    assert((x -= y) == R(1, 1));
    assert((x *= y) == R(2, 1));
    assert((x /= y) == R(1, 1));
    assert((x++) == R(1, 1));
    assert((x--) == R(2, 1));
    assert((++y) == R(3, 1));
    assert((--y) == R(2, 1));

    assert((x + y) == R(3, 1));
    assert((x - y) == R(-1, 1));
    assert((x * y) == R(2, 1));
    assert((x / y) == R(1, 2));

    assert((x += 1) == R(2, 1));
    assert((x + 1) == R(3, 1));
    assert((1 + y) == R(3, 1));
    assert((1 + 1) == 2);

    assert((x < y) == 0);
    assert((x > y) == 0);
    assert((x <= y) == 1);
    assert((x >= y) == 1);
    assert((x == y) == 1);
    assert((x != y) == 0);

    std::stringstream stream_1("1/2");
    std::stringstream stream_2;
    stream_1 >> x;
    stream_2 << x;
    assert(stream_2.str() == stream_1.str());

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
