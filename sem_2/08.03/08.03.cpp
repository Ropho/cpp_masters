/////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>

/////////////////////////////////////////////////////////////////////////////////////////

auto binary_log_int(int x) -> int
{
	assert(x > 0);

	auto value = static_cast < unsigned int > (x);

	int result = 0;

	while (value >>= 1) { ++result; }

	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

auto binary_log_float(float x) -> int
{
	assert(x > 0.0f);

	assert(!std::numeric_limits < float > ::has_infinity  || x != std::numeric_limits < float > ::infinity());

	assert(!std::numeric_limits < float > ::has_quiet_NaN || x == x);

//  -------------------------------------------------------------------------------------

	static_assert(sizeof(float) == sizeof(unsigned int));

	union { float f; unsigned int u; } converter { .f = x };

	auto bits = converter.u;

//  -------------------------------------------------------------------------------------

	auto exponent = static_cast < int > ((bits >> 23) & 0xFF);

//  -------------------------------------------------------------------------------------

	if (exponent == 0)
	{
		auto mantissa = bits & 0x7FFFFF;

		int result = -149;

		while (mantissa >>= 1) { ++result; }

		return result;
	}

//  -------------------------------------------------------------------------------------

	return exponent - 127;
}

/////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	assert(binary_log_int(  1) == 0);
	assert(binary_log_int(  2) == 1);
	assert(binary_log_int(  3) == 1);
	assert(binary_log_int(  4) == 2);
	assert(binary_log_int(  7) == 2);
	assert(binary_log_int(  8) == 3);
	assert(binary_log_int(255) == 7);

//  -------------------------------------------------------------------------------------

	assert(binary_log_float(1.0f) ==   0);
	assert(binary_log_float(2.0f) ==   1);
	assert(binary_log_float(4.0f) ==   2);
	assert(binary_log_float(0.5f) ==  -1);

//  -------------------------------------------------------------------------------------

	assert(binary_log_float(1e-38f) == -127);

	assert(binary_log_float(1e-45f) == -149);

//  -------------------------------------------------------------------------------------

	std::cout << "binary_log_int(255)    = " << binary_log_int(255)      << std::endl;

	std::cout << "binary_log_float(0.5f) = " << binary_log_float(0.5f)  << std::endl;

	std::cout << "binary_log_float(1e-45f) = " << binary_log_float(1e-45f) << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////
