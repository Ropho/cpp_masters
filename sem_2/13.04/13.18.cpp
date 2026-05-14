//////////////////////////////////////////////////////////////////////////////////////

// chapter : Streams

//////////////////////////////////////////////////////////////////////////////////////

// section : Filesystem

//////////////////////////////////////////////////////////////////////////////////////

// content : Iterator std::filesystem::directory_iterator
//
// content : Helper std::filesystem::directory_entry
//
// content : Helper std::filesystem::file_status
//
// content : Function std::filesystem::is_directory
//
// content : Function std::filesystem::is_regular_file
//
// content : Function std::filesystem::is_symlink
//
// content : Enumeration std::filesystem::perms
//
// content : Function std::filesystem::file_size
//
// content : Iterator std::filesystem::recursive_directory_iterator
//
// content : Clock std::chrono::file_clock

//////////////////////////////////////////////////////////////////////////////////////

// support : ls -la
// filter  : std::regex (имена вхождений; по умолчанию как grep: подстрока в имени)

//////////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////////

auto make_type(std::filesystem::file_status const & status)
{
    if (std::filesystem::is_directory   (status)) { return 'd'; }

    if (std::filesystem::is_regular_file(status)) { return 'f'; }

    if (std::filesystem::is_symlink     (status)) { return 'l'; }

    return '?';
}

//////////////////////////////////////////////////////////////////////////////////////

auto make_permissions(std::filesystem::perms permissions) -> std::string
{
    auto lambda = [permissions](auto x, auto y)
    {
        return (permissions & x) == std::filesystem::perms::none ? '-' : y;
    };

    return
    {
        lambda(std::filesystem::perms::owner_read,  'r'),

        lambda(std::filesystem::perms::owner_write, 'w'),

        lambda(std::filesystem::perms::owner_exec,  'x')
    };
}

//////////////////////////////////////////////////////////////////////////////////////

auto size(std::filesystem::path const & path)
{
	auto size = std::size_t{};

	if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
	{
		for (auto const & entry : std::filesystem::recursive_directory_iterator(path))
		{
			if (!std::filesystem::is_directory(entry.status()))
			{
				size += std::filesystem::file_size(entry);
			}
		}
	}

	return size;
}

//////////////////////////////////////////////////////////////////////////////////////

auto size(std::filesystem::directory_entry const & entry)
{
    auto size = std::size_t{};

    if (std::filesystem::is_regular_file(entry.status()))
    {
        size = std::filesystem::file_size(entry);
    }
    else
    {
        size = ::size(entry.path());
    }

    std::vector < char > vector = { 'B', 'K', 'M', 'G' };

    auto i = std::size_t{};

    while (i < std::size(vector) && size >= 1 << 10)
    {
        size /= (1 << 10);

        ++i;
    }

    return (std::stringstream() << std::format("{: >4} ({})", size, vector[i])).str();
}

//////////////////////////////////////////////////////////////////////////////////////

void show(std::filesystem::path const & path, std::regex const & name_pattern)
{
	if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
	{
        for (auto const & entry : std::filesystem::directory_iterator(path))
		{
            auto const name = entry.path().filename().string();

            if (!std::regex_search(name, name_pattern))
            {
                continue;
            }

            std::cout << std::format
            (
                "show : entry : {} | {} | {} | {} | {}\n",

                make_type(entry.status()),

                make_permissions(entry.status().permissions()),

                size(entry),

                std::chrono::floor < std::chrono::seconds >
                (
                    std::chrono::file_clock::to_sys(entry.last_write_time())
                ),

                name
            );
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////

void print_grep_comparison()
{
    std::cout <<
        R"(Сравнение с grep:
  • Здесь фильтруются только имена файлов в одной директории (не рекурсивно).
  • По умолчанию используется std::regex_search — как типичный вывод
    «ls | grep ШАБЛОН»: вхождение шаблона где-либо в имени.
  • std::regex использует синтаксис ECMAScript (ближе к grep -E / egrep).
  • У grep свой диалект BRE/ERE, флаги -i, -w, -x, -F и т.д.; у C++
    отдельные std::regex_constants (icase, и т.п.) настраиваются в коде.
  • Для рекурсивного обхода в grep обычно: grep -r или find ... -exec;
    здесь при необходимости можно заменить directory_iterator на
    recursive_directory_iterator.
)";
}

//////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << "usage: " << (argc > 0 ? argv[0] : "13.18")
                  << " <regex> [directory]\n"
                  << "  directory defaults to current path\n";

        return 1;
    }

    std::regex name_pattern;

    try
    {
        name_pattern.assign(argv[1], std::regex::ECMAScript);
    }
    catch (std::regex_error const & error)
    {
        std::cout << "invalid regex: " << error.what() << '\n';

        return 1;
    }

    auto const directory =
        argc > 2 ? std::filesystem::path(argv[2]) : std::filesystem::current_path();

    std::cout << "filter (regex): " << argv[1] << "\n"
              << "directory: " << directory.string() << "\n\n";

    show(directory, name_pattern);

    std::cout << '\n';
    print_grep_comparison();

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
