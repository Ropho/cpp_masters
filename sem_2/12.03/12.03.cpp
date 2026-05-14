#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace
{
auto cache_index(std::size_t row, std::size_t column, std::size_t width)
    -> std::size_t
{
	return row * width + column;
}

auto longest_palindrome_substring(std::string_view text) -> std::string_view
{
	auto const size = text.size();

	if (size == 0)
	{
		return {};
	}

	std::vector<bool> palindrome_cache(size * size, false);
	std::size_t best_begin = 0;
	std::size_t best_length = 1;

	for (std::size_t i = 0; i < size; ++i)
	{
		palindrome_cache[cache_index(i, i, size)] = true;
	}

	for (std::size_t length = 2; length <= size; ++length)
	{
		for (std::size_t begin = 0; begin + length <= size; ++begin)
		{
			auto const end = begin + length - 1;
			auto is_palindrome = text[begin] == text[end];

			if (is_palindrome && length > 2)
			{
				is_palindrome =
				    palindrome_cache[cache_index(begin + 1, end - 1, size)];
			}

			palindrome_cache[cache_index(begin, end, size)] = is_palindrome;

			if (is_palindrome && length > best_length)
			{
				best_begin = begin;
				best_length = length;
			}
		}
	}

	return text.substr(best_begin, best_length);
}

void run_tests()
{
	assert(longest_palindrome_substring("") == "");
	assert(longest_palindrome_substring("a") == "a");
	assert(longest_palindrome_substring("abcdef") == "a");
	assert(longest_palindrome_substring("cbbd") == "bb");
	assert(longest_palindrome_substring("bananas") == "anana");
	assert(longest_palindrome_substring("forgeeksskeegfor") == "geeksskeeg");
	assert(longest_palindrome_substring("aaaa") == "aaaa");
}
} // namespace

int main()
{
	run_tests();

	std::cout << "Enter a string: ";

	std::string text;
	std::getline(std::cin, text);

	auto const palindrome = longest_palindrome_substring(text);

	std::cout << "Longest palindrome substring: " << palindrome << '\n';
	std::cout << "Length: " << palindrome.size() << '\n';

	return 0;
}
