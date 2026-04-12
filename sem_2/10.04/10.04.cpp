#include <boost/numeric/ublas/matrix.hpp>

#include <cassert>
#include <iostream>
#include <string>

namespace
{
using ull = unsigned long long;
using matrix = boost::numeric::ublas::matrix<ull>;

matrix identity2()
{
	matrix i(2, 2);
	i(0, 0) = 1;
	i(0, 1) = 0;
	i(1, 0) = 0;
	i(1, 1) = 1;
	return i;
}

matrix fibonacci_base()
{
	matrix q(2, 2);
	q(0, 0) = 1;
	q(0, 1) = 1;
	q(1, 0) = 1;
	q(1, 1) = 0;
	return q;
}

matrix multiply(matrix const & a, matrix const & b)
{
	return matrix(boost::numeric::ublas::prod(a, b));
}

matrix power(matrix base, ull exponent)
{
	matrix result = identity2();

	while (exponent > 0)
	{
		if ((exponent & 1ULL) != 0)
		{
			result = multiply(result, base);
		}

		base = multiply(base, base);
		exponent >>= 1;
	}

	return result;
}

ull fibonacci(ull n)
{
	if (n == 0)
	{
		return 0;
	}

	matrix const q_to_n = power(fibonacci_base(), n);
	return q_to_n(0, 1);
}
} // namespace

int main(int argc, char * argv[])
{
	ull n = 40;

	if (argc >= 2)
	{
		n = std::stoull(argv[1]);
	}

	assert(fibonacci(0) == 0);
	assert(fibonacci(1) == 1);
	assert(fibonacci(10) == 55);
	assert(fibonacci(20) == 6765);

	ull const value = fibonacci(n);

	std::cout << "F(" << n << ") = " << value << '\n';

	std::cout
		<< "Matrix exponentiation uses O(log n) 2x2 multiplications; each is O(1) "
		   "integer ops, so time O(log n) vs O(n) for linear recurrence, O(n) "
		   "space for memoization, or O(1) time for Binet with floating error.\n";

	return 0;
}
