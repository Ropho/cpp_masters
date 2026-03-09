#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

////////////////////////////////////////////////////////////////////////////////

using Roots = std::variant < double, std::pair < double, double > , std::monostate > ;

////////////////////////////////////////////////////////////////////////////////

auto solve(double a, double b, double c) -> std::optional < Roots >
{
	if (auto epsilon = 1e-9; std::abs(a) < epsilon)
	{
		if (std::abs(b) < epsilon)
		{
			if (std::abs(c) < epsilon)
			{
				return std::monostate();
			}

			return std::nullopt;
		}

		return -c / b;
	}

//  ----------------------------------------------------------------------------

	auto discriminant = b * b - 4 * a * c;

//  ----------------------------------------------------------------------------

	if (discriminant < 0)
	{
		return std::nullopt;
	}

	if (auto epsilon = 1e-9; std::abs(discriminant) < epsilon)
	{
		return -b / (2 * a);
	}

	auto sqrt_discriminant = std::sqrt(discriminant);

	return std::pair((-b - sqrt_discriminant) / (2 * a),
	                 (-b + sqrt_discriminant) / (2 * a));
}

////////////////////////////////////////////////////////////////////////////////

int main()
{
	double a = 0, b = 0, c = 0;

	std::cout << "enter coefficients a, b, c: ";

	std::cin >> a >> b >> c;

//  ----------------------------------------------------------------------------

	auto result = solve(a, b, c);

//  ----------------------------------------------------------------------------

	if (!result)
	{
		std::cout << "no roots" << std::endl;
	}
	else if (std::holds_alternative < std::monostate > (*result))
	{
		std::cout << "infinite roots" << std::endl;
	}
	else if (std::holds_alternative < double > (*result))
	{
		std::cout << "one root: x = " << std::get < double > (*result) << std::endl;
	}
	else if (std::holds_alternative < std::pair < double, double > > (*result))
	{
		auto [x1, x2] = std::get < std::pair < double, double > > (*result);

		std::cout << "two roots: x1 = " << x1 << ", x2 = " << x2 << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////
