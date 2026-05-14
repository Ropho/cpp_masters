#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

class Stream
{
public :

	using token_t = std::variant < char, double, std::string > ;

//  ------------------------------------------------------------------------------------

	Stream(std::string const & string) : m_stream(string + ';') {}

//  ------------------------------------------------------------------------------------

	auto get()
	{
		if (m_has_token)
		{
			m_has_token = false;

			return m_token;
		}

		auto x = '\0';
		m_stream >> x;

		switch (x)
		{
			case '+' : case '-' : case '*' : case '/' : case '%' : case '^' :

			case '!' : case '(' : case ')' : case '[' : case ']' :

			case '{' : case '}' : case ';' :
			{
				return token_t(x);
			}

			case '0' : case '1' : case '2' : case '3' : case '4' :

			case '5' : case '6' : case '7' : case '8' : case '9' :

			case '.' :
			{
				m_stream.unget();

				auto y = 0.0;
				m_stream >> y;

				return token_t(y);
			}

			default :
			{
				std::string string(1, x);

				while
				(
					m_stream.get(x) &&
					(std::isalpha(static_cast < unsigned char > (x)) ||
					 std::isdigit(static_cast < unsigned char > (x)))
				)
				{
					string += x;
				}

				if (!std::isspace(static_cast < unsigned char > (x)))
				{
					m_stream.unget();
				}

				return token_t(string);
			}
		}
	}

//  ------------------------------------------------------------------------------------

	void put(token_t const & token)
	{
		m_token = token;

		m_has_token = true;
	}

private :

	std::stringstream m_stream;

	token_t m_token;

	bool m_has_token = false;
};

class Calculator
{
public :

	auto calculate(std::string const & string) -> double
	{
		Stream stream(string);
		auto result = statement(stream);
		auto token = stream.get();

		if (!std::holds_alternative < char > (token) || std::get < char > (token) != ';')
		{
			throw std::runtime_error("unexpected token after statement");
		}

		return result;
	}

private :

	auto statement(Stream & stream) -> double
	{
		auto token = stream.get();

		if (std::holds_alternative < std::string > (token))
		{
			if (std::get < std::string > (token) == "set")
			{
				return declaration(stream);
			}
		}

		stream.put(token);

		return expression(stream);
	}

//  ----------------------------------------------------------------------------------

	auto declaration(Stream & stream) -> double
	{
		auto token = stream.get();

		if (!std::holds_alternative < std::string > (token))
		{
			throw std::runtime_error("variable name expected");
		}

		auto const variable = std::get < std::string > (token);

		m_variables[variable] = expression(stream);

		return m_variables[variable];
	}

//  ----------------------------------------------------------------------------------

	auto expression(Stream & stream) const -> double
	{
		auto x = term(stream);
		auto token = stream.get();

		while (true)
		{
			if (!std::holds_alternative < char > (token))
			{
				throw std::runtime_error("operator expected");
			}

			switch (std::get < char > (token))
			{
				case '+' : { x += term(stream); break; }

				case '-' : { x -= term(stream); break; }

				default  :
				{
					stream.put(token);

					return x;
				}
			}

			token = stream.get();
		}
	}

//  ----------------------------------------------------------------------------------

	auto term(Stream & stream) const -> double
	{
		auto x = power(stream);
		auto token = stream.get();

		while (true)
		{
			if (!std::holds_alternative < char > (token))
			{
				throw std::runtime_error("operator expected");
			}

			switch (std::get < char > (token))
			{
				case '*' : { x *= power(stream); break; }

				case '/' : { x /= power(stream); break; }

				case '%' : { x = std::fmod(x, power(stream)); break; }

				default  :
				{
					stream.put(token);

					return x;
				}
			}

			token = stream.get();
		}
	}

//  ----------------------------------------------------------------------------------

	auto power(Stream & stream) const -> double
	{
		auto x = unary(stream);
		auto token = stream.get();

		if (!std::holds_alternative < char > (token))
		{
			throw std::runtime_error("operator expected");
		}

		if (std::get < char > (token) == '^')
		{
			return std::pow(x, power(stream));
		}

		stream.put(token);

		return x;
	}

//  ----------------------------------------------------------------------------------

	auto unary(Stream & stream) const -> double
	{
		auto token = stream.get();

		if (std::holds_alternative < char > (token))
		{
			switch (std::get < char > (token))
			{
				case '+' : { return      unary(stream); }

				case '-' : { return -1 * unary(stream); }
			}
		}

		stream.put(token);

		return postfix(stream);
	}

//  ----------------------------------------------------------------------------------

	auto postfix(Stream & stream) const -> double
	{
		auto x = primary(stream);
		auto token = stream.get();

		if (!std::holds_alternative < char > (token))
		{
			throw std::runtime_error("operator expected");
		}

		while (std::get < char > (token) == '!')
		{
			x = factorial(x);

			token = stream.get();

			if (!std::holds_alternative < char > (token))
			{
				throw std::runtime_error("operator expected");
			}
		}

		stream.put(token);

		return x;
	}

//  ----------------------------------------------------------------------------------

	auto primary(Stream & stream) const -> double
	{
		auto token = stream.get();

		if (std::holds_alternative < char > (token))
		{
			switch (std::get < char > (token))
			{
				case '(' : { return brackets(stream, ')'); }

				case '[' : { return brackets(stream, ']'); }

				case '{' : { return brackets(stream, '}'); }
			}
		}

		if (std::holds_alternative < double > (token))
		{
			return std::get < double > (token);
		}

		if (std::holds_alternative < std::string > (token))
		{
			return m_variables.at(std::get < std::string > (token));
		}

		throw std::runtime_error("primary expected");
	}

//  ----------------------------------------------------------------------------------

	auto brackets(Stream & stream, char expected) const -> double
	{
		auto x = expression(stream);
		auto token = stream.get();

		if (!std::holds_alternative < char > (token) || std::get < char > (token) != expected)
		{
			throw std::runtime_error("closing bracket expected");
		}

		return x;
	}

//  ----------------------------------------------------------------------------------

	static auto factorial(double x) -> double
	{
		if (x < 0)
		{
			throw std::domain_error("factorial is not defined for negative numbers");
		}

		auto result = 1.0;

		for (auto i = 2U; i <= static_cast < unsigned > (x); ++i)
		{
			result *= i;
		}

		return result;
	}

//  ----------------------------------------------------------------------------------

	std::unordered_map < std::string, double > m_variables;
};

auto trim(std::string_view view) -> std::string_view
{
	auto const begin = view.find_first_not_of(" \t\n\r");

	if (begin == std::string_view::npos)
	{
		return {};
	}

	auto const end = view.find_last_not_of(" \t\n\r");

	return view.substr(begin, end - begin + 1);
}

auto parse_expected(std::string_view view) -> double
{
	std::stringstream stream(std::string(trim(view)));

	auto expected = 0.0;
	stream >> expected;

	if (!stream)
	{
		throw std::runtime_error("expected result is invalid");
	}

	stream >> std::ws;

	if (!stream.eof())
	{
		throw std::runtime_error("unexpected characters after expected result");
	}

	return expected;
}

auto is_close(double lhs, double rhs) -> bool
{
	return std::abs(lhs - rhs) < 1e-9;
}

auto run_tests_from_file(std::string const & file_name) -> bool
{
	std::fstream input(file_name, std::ios::in);

	if (!input)
	{
		throw std::runtime_error("failed to open input file: " + file_name);
	}

	Calculator calculator;
	std::string line;
	auto line_number = 0U;
	auto test_count = 0U;
	auto passed_count = 0U;

	while (std::getline(input, line))
	{
		++line_number;

		auto const trimmed_line = trim(line);

		if (trimmed_line.empty() || trimmed_line.front() == '#')
		{
			continue;
		}

		auto const separator = trimmed_line.rfind('=');

		if (separator == std::string_view::npos)
		{
			throw std::runtime_error("line " + std::to_string(line_number) + ": '=' expected");
		}

		auto const instruction = trim(trimmed_line.substr(0, separator));
		auto const expected = parse_expected(trimmed_line.substr(separator + 1));
		auto const actual = calculator.calculate(std::string(instruction));
		auto const passed = is_close(actual, expected);

		++test_count;
		passed_count += passed ? 1U : 0U;

		std::cout << (passed ? "OK   " : "FAIL ") << instruction << " = "
		          << actual << " expected " << expected << '\n';
	}

	std::cout << "Passed " << passed_count << " of " << test_count << " tests\n";

	return test_count != 0 && test_count == passed_count;
}

int main(int argc, char * argv[])
{
	auto const file_name = argc > 1 ? argv[1] : "tests.txt";

	try
	{
		return run_tests_from_file(file_name) ? EXIT_SUCCESS : EXIT_FAILURE;
	}
	catch (std::exception const & error)
	{
		std::cout << "Error: " << error.what() << '\n';

		return EXIT_FAILURE;
	}
}
