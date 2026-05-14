////////////////////////////////////////////////////////////////////////////////////////

// chapter : Characters

////////////////////////////////////////////////////////////////////////////////////////

// section : Parsers

////////////////////////////////////////////////////////////////////////////////////////

// content : Arithmetic Grammar
//
// content : Extended Backus-Naur Form (EBNF)
//
// content : Statements, Declarations, Expressions, Terms, Primaries and Tokens
//
// content : Parsers
//
// content : Recursive Descent Parsers
//
// content : Arithmetic Parser
//
// content : Functions std::isalpha, std::isdigit and std::isspace
//
// content : Function std::getline
//
// content : Manipulator std::ws

////////////////////////////////////////////////////////////////////////////////////////

#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

////////////////////////////////////////////////////////////////////////////////////////

class Stream
{
public :

	using token_t = std::variant < char, double, std::string > ;

//  ------------------------------------------------------------------------------------

	Stream(std::string const & string) : m_stream(string + ';') {}

//  ------------------------------------------------------------------------------------

	auto empty()
	{
		return m_stream.peek() == ';';
	}

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

				while (m_stream.get(x) && (std::isalpha(x) || std::isdigit(x)))
				{
					string += x;
				}

				if (!std::isspace(x))
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

////////////////////////////////////////////////////////////////////////////////////////

class Calculator
{
public :

	auto calculate(std::string const & string) -> double
	{
		Stream stream(string);

		return statement(stream);
	}

//  ------------------------------------------------------------------------------------

	void test()
	{
		std::string string;

		std::cout << "Calculator::test : enter std::string string(s) : \n";

		while (std::getline(std::cin >> std::ws, string, '\n'))
		{
			if (Stream stream(string); !stream.empty())
			{
				std::cout << "Calculator::test : " << string << " = "
				          << calculate(string) << '\n';
			}
			else
			{
				break;
			}
		}
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
		auto string = std::get < std::string > (stream.get());

		m_variables[string] = expression(stream);

		return m_variables[string];
	}

//  ----------------------------------------------------------------------------------

	auto expression(Stream & stream) const -> double
	{
		auto x = term(stream);

		auto token = stream.get();

		while (true)
		{
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

		while (std::get < char > (token) == '!')
		{
			x = factorial(x);

			token = stream.get();
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
				case '(' :
				{
					return brackets(stream, ')');
				}

				case '[' :
				{
					return brackets(stream, ']');
				}

				case '{' :
				{
					return brackets(stream, '}');
				}
			}
		}

		if (std::holds_alternative < double > (token))
		{
			return std::get < double > (token);
		}

		return m_variables.at(std::get < std::string > (token));
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

////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	Calculator calculator;

	assert(calculator.calculate("2 + 3 * 4") == 14);
	assert(calculator.calculate("10 % 4") == 2);
	assert(calculator.calculate("2 ^ 3 ^ 2") == 512);
	assert(calculator.calculate("5!") == 120);
	assert(calculator.calculate("[2 + 3] * {4 + 1}") == 25);
	assert(calculator.calculate("(3 + 2)! / 5!") == 1);

	calculator.test();
}

////////////////////////////////////////////////////////////////////////////////////////