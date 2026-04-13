////////////////////////////////////////////////////////////////////////////////

#ifndef INTEGER_08_13_HPP
#define INTEGER_08_13_HPP

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////

class Integer
{
public:

	Integer() : m_positive(true), m_digits("0") {}

	Integer(std::string s)
	{
		if (s.empty())
		{
			m_positive = true;
			m_digits   = "0";
			return;
		}
		std::size_t i = 0;
		if (s[i] == '+') { m_positive = true;  ++i; }
		else if (s[i] == '-') { m_positive = false; ++i; }
		else { m_positive = true; }
		while (i < s.size() && s[i] == '0') ++i;
		if (i == s.size())
		{
			m_digits = "0";
			m_positive = true;
			return;
		}
		m_digits.reserve(s.size() - i);
		for (; i < s.size(); ++i)
		{
			if (!std::isdigit(static_cast < unsigned char > (s[i]))) break;
			m_digits += s[i];
		}
		if (m_digits == "0") m_positive = true;
	}

	std::string to_string() const
	{
		return (m_positive ? "+" : "-") + m_digits;
	}

	bool positive() const { return m_positive; }
	const std::string & digits() const { return m_digits; }

	Integer & operator+=(Integer other)
	{
		if (m_positive == other.m_positive)
		{
			m_digits = add_unsigned(m_digits, other.m_digits);
			return normalize();
		}
		int c = compare_unsigned(m_digits, other.m_digits);
		if (c == 0) { m_digits = "0"; m_positive = true; return *this; }
		if (c > 0)
			m_digits = sub_unsigned(m_digits, other.m_digits);
		else
		{
			m_digits = sub_unsigned(other.m_digits, m_digits);
			m_positive = other.m_positive;
		}
		return normalize();
	}

	Integer & operator-=(Integer other)
	{
		other.m_positive = !other.m_positive;
		return *this += other;
	}

	Integer & operator*=(Integer const & other)
	{
		m_positive = (m_positive == other.m_positive);
		m_digits   = mul_unsigned(m_digits, other.m_digits);
		return normalize();
	}

	Integer & operator/=(Integer other)
	{
		bool result_positive = (m_positive == other.m_positive);
		other.m_positive = true;
		Integer abs_this(*this);
		abs_this.m_positive = true;
		auto qr = div_mod_unsigned(abs_this.m_digits, other.m_digits);
		m_digits   = qr.first;
		m_positive = result_positive;
		return normalize();
	}

	Integer & operator%=(Integer other)
	{
		other.m_positive = true;
		Integer abs_this(*this);
		abs_this.m_positive = true;
		auto qr = div_mod_unsigned(abs_this.m_digits, other.m_digits);
		m_digits   = qr.second;
		return normalize();
	}

	Integer & operator++()
	{
		*this += Integer("+1");
		return *this;
	}

	Integer operator++(int)
	{
		Integer old = *this;
		++*this;
		return old;
	}

	Integer & operator--()
	{
		*this -= Integer("+1");
		return *this;
	}

	Integer operator--(int)
	{
		Integer old = *this;
		--*this;
		return old;
	}

	int sign() const
	{
		if (m_digits == "0") return 0;
		return m_positive ? 1 : -1;
	}

	Integer abs() const
	{
		Integer r = *this;
		r.m_positive = true;
		return r;
	}

	friend Integer operator+(Integer a, Integer const & b) { return a += b; }
	friend Integer operator-(Integer a, Integer const & b) { return a -= b; }
	friend Integer operator*(Integer a, Integer const & b) { return a *= b; }
	friend Integer operator/(Integer a, Integer const & b) { return a /= b; }
	friend Integer operator%(Integer a, Integer const & b) { return a %= b; }

	friend Integer pow(Integer base, unsigned int exp)
	{
		Integer result("+1");
		Integer b = base;
		while (exp)
		{
			if (exp & 1) result *= b;
			b *= b;
			exp /= 2;
		}
		return result;
	}

	friend bool operator<(Integer const & a, Integer const & b)
	{
		if (a.m_positive != b.m_positive) return !a.m_positive;
		if (a.m_digits == "0" && b.m_digits == "0") return false;
		int c = compare_unsigned(a.m_digits, b.m_digits);
		if (a.m_positive) return c < 0;
		return c > 0;
	}
	friend bool operator>(Integer const & a, Integer const & b)  { return b < a; }
	friend bool operator<=(Integer const & a, Integer const & b) { return !(b < a); }
	friend bool operator>=(Integer const & a, Integer const & b) { return !(a < b); }
	friend bool operator==(Integer const & a, Integer const & b)
	{
		return a.m_positive == b.m_positive && a.m_digits == b.m_digits;
	}
	friend bool operator==(Integer const & a, std::string const & s)
	{
		return a.to_string() == s;
	}
	friend bool operator!=(Integer const & a, Integer const & b) { return !(a == b); }

	friend std::istream & operator>>(std::istream & in, Integer & n)
	{
		std::string s;
		if (in >> s) n = Integer(s);
		return in;
	}
	friend std::ostream & operator<<(std::ostream & out, Integer const & n)
	{
		return out << n.m_digits;
	}

private:

	Integer & normalize()
	{
		while (m_digits.size() > 1 && m_digits.front() == '0')
			m_digits.erase(0, 1);
		if (m_digits == "0") m_positive = true;
		return *this;
	}

	static std::string add_unsigned(std::string a, std::string b)
	{
		if (a.size() < b.size()) std::swap(a, b);
		std::string result;
		int carry = 0;
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			int d = (a[a.size() - 1 - i] - '0') + (i < b.size() ? (b[b.size() - 1 - i] - '0') : 0) + carry;
			result.push_back(static_cast < char > ('0' + d % 10));
			carry = d / 10;
		}
		if (carry) result.push_back('1');
		std::reverse(result.begin(), result.end());
		return result;
	}

	static std::string sub_unsigned(std::string a, std::string b)
	{
		std::string result;
		int borrow = 0;
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			int da = a[a.size() - 1 - i] - '0';
			int db = (i < b.size()) ? (b[b.size() - 1 - i] - '0') : 0;
			int d = da - db - borrow;
			borrow = 0;
			if (d < 0) { d += 10; borrow = 1; }
			result.push_back(static_cast < char > ('0' + d));
		}
		while (result.size() > 1 && result.back() == '0') result.pop_back();
		std::reverse(result.begin(), result.end());
		return result;
	}

	static int compare_unsigned(std::string const & a, std::string const & b)
	{
		if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
		}
		return 0;
	}

	static std::string mul_unsigned(std::string const & a, std::string const & b)
	{
		if (a == "0" || b == "0") return "0";
		std::string result = "0";
		for (std::size_t i = 0; i < b.size(); ++i)
		{
			int digit = b[b.size() - 1 - i] - '0';
			if (digit == 0) continue;
			std::string row = a;
			int carry = 0;
			for (std::size_t j = 0; j < row.size(); ++j)
			{
				int k = row.size() - 1 - j;
				int v = (row[k] - '0') * digit + carry;
				row[k] = static_cast < char > ('0' + v % 10);
				carry = v / 10;
			}
			if (carry) row.insert(0, 1, static_cast < char > ('0' + carry));
			row.append(i, '0');
			result = add_unsigned(result, row);
		}
		return result;
	}

	static std::pair < std::string, std::string > div_mod_unsigned(std::string a, std::string b)
	{
		if (b == "0") return { "0", "0" };
		if (compare_unsigned(a, b) < 0) return { "0", a };
		if (a == b) return { "1", "0" };
		std::string q = "0", r = "0";
		for (char digit : a)
		{
			r = add_unsigned(mul_unsigned(r, "10"), std::string(1, digit));
			int d = 0;
			while (compare_unsigned(r, b) >= 0)
			{
				r = sub_unsigned(r, b);
				++d;
			}
			q = add_unsigned(mul_unsigned(q, "10"), std::string(1, static_cast < char > ('0' + d)));
		}
		while (r.size() > 1 && r[0] == '0') r.erase(0, 1);
		if (r.empty()) r = "0";
		return { q, r };
	}

	bool m_positive;
	std::string m_digits;
};

////////////////////////////////////////////////////////////////////////////////

inline Integer multiply(Integer a, Integer const & b) { return a *= b; }

inline Integer sqrt(Integer n)
{
	if (n.sign() <= 0) return Integer("+0");
	Integer x = n;
	Integer y = (x + Integer("+1")) / Integer("+2");
	while (y < x)
	{
		x = y;
		y = (x + n / x) / Integer("+2");
	}
	return x;
}

////////////////////////////////////////////////////////////////////////////////

#endif
