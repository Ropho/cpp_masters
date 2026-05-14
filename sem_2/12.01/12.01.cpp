#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace
{
constexpr long double rubles_per_usd = 90.0L;

auto trim(std::string const & value) -> std::string
{
	auto const begin = value.find_first_not_of(" \t\n\r");

	if (begin == std::string::npos)
	{
		return {};
	}

	auto const end = value.find_last_not_of(" \t\n\r");

	return value.substr(begin, end - begin + 1);
}

auto locale_from_names(char const * primary, char const * fallback) -> std::locale
{
	try
	{
		return std::locale(primary);
	}
	catch (std::runtime_error const &)
	{
		return std::locale(fallback);
	}
}

auto remove_rub_designation(std::string const & value) -> std::string
{
	auto amount = trim(value);
	auto constexpr code = std::string_view("RUB");

	if (amount.starts_with(code))
	{
		return trim(amount.substr(code.size()));
	}

	if (amount.ends_with(code))
	{
		return trim(amount.substr(0, amount.size() - code.size()));
	}

	throw std::invalid_argument("RUB designation is required");
}

auto read_rub_kopecks(std::string const & value, std::locale const & locale) -> long double
{
	std::stringstream input(remove_rub_designation(value));
	input.imbue(locale);

	long double kopecks = 0;
	input >> std::get_money(kopecks);

	if (!input)
	{
		throw std::invalid_argument("invalid RUB amount");
	}

	input >> std::ws;

	if (!input.eof())
	{
		throw std::invalid_argument("unexpected characters after RUB amount");
	}

	return kopecks;
}

auto convert_to_usd_cents(long double rub_kopecks) -> long double
{
	auto const rubles = rub_kopecks / 100.0L;
	auto const dollars = rubles / rubles_per_usd;

	return std::round(dollars * 100.0L);
}

auto write_usd(long double usd_cents, std::locale const & locale) -> std::string
{
	std::stringstream output;
	output.imbue(locale);
	output << std::showbase << std::put_money(usd_cents, true);

	return output.str();
}

auto convert_rub_to_usd(std::string const & value,
                        std::locale const & input_locale,
                        std::locale const & output_locale) -> std::string
{
	auto const rub_kopecks = read_rub_kopecks(value, input_locale);
	auto const usd_cents = convert_to_usd_cents(rub_kopecks);

	return write_usd(usd_cents, output_locale);
}

void test_conversion(std::locale const & input_locale,
                     std::locale const & output_locale)
{
	assert(read_rub_kopecks("RUB 9 000,00", input_locale) == 900000);
	assert(read_rub_kopecks("9 000,00 RUB", input_locale) == 900000);
	assert(convert_rub_to_usd("RUB 9 000,00", input_locale, output_locale) ==
	       "USD 100.00");
}
} // namespace

int main()
{
	auto const input_locale = locale_from_names("ru_RU.utf8", "ru_RU.UTF-8");
	auto const output_locale = locale_from_names("en_US.utf8", "en_US.UTF-8");

	test_conversion(input_locale, output_locale);

	std::cout << "Rate: 1 USD = " << static_cast<int>(rubles_per_usd) << " RUB\n";
	std::cout << "Enter RUB amount, for example RUB 9 000,00 or 9 000,00 RUB: ";

	std::string rub_amount;
	std::getline(std::cin, rub_amount);

	try
	{
		std::cout << "USD amount: "
		          << convert_rub_to_usd(rub_amount, input_locale, output_locale)
		          << '\n';
	}
	catch (std::exception const & error)
	{
		std::cout << "Error: " << error.what() << '\n';

		return 1;
	}

	return 0;
}
