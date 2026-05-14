////////////////////////////////////////////////////////////////////////////////////////////

// chapter : Streams

////////////////////////////////////////////////////////////////////////////////////////////

// section : Iterators

////////////////////////////////////////////////////////////////////////////////////////////

// content : Comment Deletion Algorithm

////////////////////////////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////

using namespace std::literals;

////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
struct Range
{
    std::size_t begin = 0;
    std::size_t end = 0;
};

//  --------------------------------------------------------------------------------------------

auto starts_with(std::string_view text, std::size_t position, std::string_view prefix) -> bool
{
    return position + prefix.size() <= text.size() &&
           text.substr(position, prefix.size()) == prefix;
}

//  --------------------------------------------------------------------------------------------

auto is_identifier_character(char character) -> bool
{
    auto const value = static_cast < unsigned char > (character);

    return std::isalnum(value) || character == '_';
}

//  --------------------------------------------------------------------------------------------

auto raw_prefix_size(std::string_view text, std::size_t position) -> std::size_t
{
    for (auto prefix : { "u8R\""sv, "uR\""sv, "UR\""sv, "LR\""sv, "R\""sv })
    {
        if (starts_with(text, position, prefix))
        {
            if (position == 0 || !is_identifier_character(text[position - 1]))
            {
                return prefix.size();
            }
        }
    }

    return 0;
}

//  --------------------------------------------------------------------------------------------

auto raw_literal_end(std::string_view text, std::size_t position) -> std::size_t
{
    auto const prefix_size = raw_prefix_size(text, position);

    if (prefix_size == 0)
    {
        return position;
    }

    auto const delimiter_begin = position + prefix_size;
    auto const content_begin = text.find('(', delimiter_begin);

    if (content_begin == std::string_view::npos || content_begin - delimiter_begin > 16)
    {
        return position;
    }

    auto const delimiter = text.substr(delimiter_begin, content_begin - delimiter_begin);
    auto const closing = ")" + std::string(delimiter) + "\"";
    auto const closing_begin = text.find(closing, content_begin + 1);

    if (closing_begin == std::string_view::npos)
    {
        return text.size();
    }

    return closing_begin + closing.size();
}

//  --------------------------------------------------------------------------------------------

auto quoted_literal_end(std::string_view text, std::size_t position, char quote) -> std::size_t
{
    for (auto current = position + 1; current < text.size(); ++current)
    {
        if (text[current] == '\\')
        {
            ++current;
        }
        else if (text[current] == quote)
        {
            return current + 1;
        }
    }

    return text.size();
}

//  --------------------------------------------------------------------------------------------

auto remove_comments(std::string_view text) -> std::string
{
    std::string result;

    for (std::size_t position = 0; position < text.size();)
    {
        if (auto const end = raw_literal_end(text, position); end != position)
        {
            result.append(text.substr(position, end - position));
            position = end;

            continue;
        }

        if (text[position] == '\'' || text[position] == '"')
        {
            auto const end = quoted_literal_end(text, position, text[position]);
            result.append(text.substr(position, end - position));
            position = end;

            continue;
        }

        if (text[position] == '/' && position + 1 < text.size())
        {
            if (text[position + 1] == '/')
            {
                position = text.find('\n', position);

                if (position == std::string_view::npos)
                {
                    break;
                }

                continue;
            }

            if (text[position + 1] == '*')
            {
                auto const end = text.find("*/", position + 2);
                position = end == std::string_view::npos ? text.size() : end + 2;

                continue;
            }
        }

        result += text[position];
        ++position;
    }

    return result;
}

//  --------------------------------------------------------------------------------------------

auto raw_literal_ranges(std::string_view text) -> std::vector < Range >
{
    std::vector < Range > ranges;

    for (std::size_t position = 0; position < text.size();)
    {
        if (auto const end = raw_literal_end(text, position); end != position)
        {
            ranges.push_back({ position, end });
            position = end;

            continue;
        }

        if (text[position] == '\'' || text[position] == '"')
        {
            position = quoted_literal_end(text, position, text[position]);

            continue;
        }

        ++position;
    }

    return ranges;
}

//  --------------------------------------------------------------------------------------------

auto is_space_only(std::string_view line) -> bool
{
    for (auto character : line)
    {
        if (!std::isspace(static_cast < unsigned char > (character)))
        {
            return false;
        }
    }

    return true;
}

//  --------------------------------------------------------------------------------------------

auto intersects(Range lhs, Range rhs) -> bool
{
    return lhs.begin < rhs.end && rhs.begin < lhs.end;
}

//  --------------------------------------------------------------------------------------------

auto remove_empty_lines_outside_raw_literals(std::string_view text) -> std::string
{
    auto const raw_ranges = raw_literal_ranges(text);
    auto raw_range_index = std::size_t{ 0 };
    std::string result;

    for (std::size_t line_begin = 0; line_begin < text.size();)
    {
        auto const newline = text.find('\n', line_begin);
        auto const line_end = newline == std::string_view::npos ? text.size() : newline;
        auto const next_line_begin = newline == std::string_view::npos ? line_end : line_end + 1;
        auto const line_range = Range{ line_begin, next_line_begin };

        while (raw_range_index < raw_ranges.size() &&
               raw_ranges[raw_range_index].end <= line_begin)
        {
            ++raw_range_index;
        }

        auto const inside_raw_literal =
            raw_range_index < raw_ranges.size() &&
            intersects(line_range, raw_ranges[raw_range_index]);

        auto const content_end =
            line_end > line_begin && text[line_end - 1] == '\r' ? line_end - 1 : line_end;
        auto const line = text.substr(line_begin, content_end - line_begin);

        if (!is_space_only(line) || inside_raw_literal)
        {
            result.append(text.substr(line_begin, next_line_begin - line_begin));
        }

        line_begin = next_line_begin;
    }

    return result;
}

//  --------------------------------------------------------------------------------------------

void transform(std::string const & path_1, std::string const & path_2)
{
    auto input = std::fstream(path_1, std::ios::in);

    if (!input)
    {
        throw std::runtime_error("failed to open input file");
    }

    auto const source = (std::stringstream() << input.rdbuf()).str();
    auto const without_comments = remove_comments(source);
    auto const without_empty_lines = remove_empty_lines_outside_raw_literals(without_comments);

    std::fstream(path_2, std::ios::out) << without_empty_lines;
}
} // namespace

////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
    auto path_1 = argc > 1 ? argv[1] : "source.cpp";

	auto path_2 = argc > 2 ? argv[2] : "output.cpp";

//  ---------------------------------------------------

    try
    {
        transform(path_1, path_2);
    }
    catch (std::exception const & error)
    {
        std::cout << "main : error : " << error.what() << '\n';

        return 1;
    }

//  ---------------------------------------------------

    std::cout << "main : transformed " << path_1 << " -> " << path_2 << '\n';

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////