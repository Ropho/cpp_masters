#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace
{
using Bytes = std::vector<std::uint8_t>;

auto bytes_to_hex(Bytes const & bytes) -> std::string
{
	std::stringstream stream;

	stream << std::hex << std::right << std::setfill('0');

	for (auto byte : bytes)
	{
		stream << std::setw(2) << static_cast<unsigned>(byte);
	}

	return stream.str();
}

auto hex_digit_to_value(char digit) -> std::uint8_t
{
	if (digit >= '0' && digit <= '9')
	{
		return static_cast<std::uint8_t>(digit - '0');
	}

	if (digit >= 'a' && digit <= 'f')
	{
		return static_cast<std::uint8_t>(digit - 'a' + 10);
	}

	throw std::invalid_argument("hex string must contain lowercase hex digits only");
}

auto hex_to_bytes(std::string_view hex) -> Bytes
{
	if (hex.size() % 2 != 0)
	{
		throw std::invalid_argument("hex string must contain an even number of digits");
	}

	Bytes bytes;
	bytes.reserve(hex.size() / 2);

	for (std::size_t i = 0; i < hex.size(); i += 2)
	{
		auto const high = hex_digit_to_value(hex[i]);
		auto const low = hex_digit_to_value(hex[i + 1]);

		bytes.push_back(static_cast<std::uint8_t>((high << 4U) | low));
	}

	return bytes;
}

auto bytes_to_decimal_list(Bytes const & bytes) -> std::string
{
	std::stringstream stream;

	stream << '[';

	for (std::size_t i = 0; i < bytes.size(); ++i)
	{
		if (i != 0)
		{
			stream << ", ";
		}

		stream << static_cast<unsigned>(bytes[i]);
	}

	stream << ']';

	return stream.str();
}

void print_examples()
{
	std::vector<Bytes> const byte_examples{
	    {},
	    {0x00},
	    {0x0f, 0x10, 0x2a, 0xff},
	    {0x48, 0x65, 0x6c, 0x6c, 0x6f},
	    {0x00, 0x01, 0x02, 0x7f, 0x80, 0xff},
	};

	std::cout << "Bytes -> lowercase hex examples:\n";

	for (auto const & bytes : byte_examples)
	{
		std::cout << bytes_to_decimal_list(bytes) << " -> "
		          << bytes_to_hex(bytes) << '\n';
	}

	std::vector<std::string_view> const hex_examples{
	    "",
	    "00",
	    "0f102aff",
	    "48656c6c6f",
	    "0001027f80ff",
	};

	std::cout << "\nLowercase hex -> bytes examples:\n";

	for (auto hex : hex_examples)
	{
		std::cout << hex << " -> "
		          << bytes_to_decimal_list(hex_to_bytes(hex)) << '\n';
	}
}

void run_tests()
{
	assert(bytes_to_hex({}) == "");
	assert(bytes_to_hex({0x00, 0x01, 0x0a, 0x10, 0xff}) == "00010a10ff");
	assert(bytes_to_hex({0x0f}) == "0f");
	assert(bytes_to_hex({0x2a, 0x7f, 0x80}) == "2a7f80");

	assert(hex_to_bytes("") == Bytes{});
	assert(hex_to_bytes("00010a10ff") == Bytes({0x00, 0x01, 0x0a, 0x10, 0xff}));
	assert(hex_to_bytes("ff00") == Bytes({0xff, 0x00}));
	assert(bytes_to_hex(hex_to_bytes("48656c6c6f")) == "48656c6c6f");

	try
	{
		(void)hex_to_bytes("abc");
		assert(false);
	}
	catch (std::invalid_argument const &)
	{
	}

	try
	{
		(void)hex_to_bytes("0A");
		assert(false);
	}
	catch (std::invalid_argument const &)
	{
	}
}
} // namespace

int main()
{
	run_tests();
	print_examples();

	std::cout << "\nEnter lowercase hex string: ";

	std::string hex;
	std::getline(std::cin, hex);

	try
	{
		auto const bytes = hex_to_bytes(hex);
		auto const restored_hex = bytes_to_hex(bytes);

		std::cout << "Bytes count: " << bytes.size() << '\n';
		std::cout << "Bytes: " << bytes_to_decimal_list(bytes) << '\n';
		std::cout << "Restored hex string: " << restored_hex << '\n';
	}
	catch (std::exception const & error)
	{
		std::cout << "Error: " << error.what() << '\n';

		return 1;
	}

	return 0;
}
