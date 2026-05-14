////////////////////////////////////////////////////////////////////////////////////////////

// chapter : Characters

////////////////////////////////////////////////////////////////////////////////////////////

// section : Regular Expressions

////////////////////////////////////////////////////////////////////////////////////////////

// content : Iterator std::sregex_token_iterator

////////////////////////////////////////////////////////////////////////////////////////////

// support : Boost.Tokenizer

////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////

using namespace std::literals;

////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    struct Email
    {
        std::string address;
        std::string domain;

        auto operator==(Email const &) const -> bool = default;
    };

//  ----------------------------------------------------------------------------------------

    auto extract_emails = [](std::string const & text)
    {
        std::regex pattern
        (
            R"(([[:alnum:]_.%+-]+)@(([[:alnum:]-]+\.)+[[:alpha:]]{2,}))",
            std::regex_constants::icase
        );

        std::vector < Email > emails;

//  ----------------------------------------------------------------------------------------

        for
        (
            auto iterator = std::sregex_iterator(text.cbegin(), text.cend(), pattern);
            iterator != std::sregex_iterator();
            ++iterator
        )
        {
            auto const & match = *iterator;

            emails.push_back
            (
                Email
                {
                    match.str(0),
                    match.str(2)
                }
            );
        }

        return emails;
    };

//  ----------------------------------------------------------------------------------------

    auto const text = R"(
        Contacts:
        support@example.com
        C++ course: cpp.master-2026@edu.mipt.ru
        Wrong values: not-an-email@, user@localhost, @example.org
    )"s;

    assert
    (
        extract_emails(text) == std::vector < Email >
        ({
            { "support@example.com", "example.com" },
            { "cpp.master-2026@edu.mipt.ru", "edu.mipt.ru" }
        })
    );

    auto const second_text = R"(Write to first.last+tag@sub-domain.example.org
or admin@EXAMPLE.RU.)"s;

    assert
    (
        extract_emails(second_text) == std::vector < Email >
        ({
            { "first.last+tag@sub-domain.example.org", "sub-domain.example.org" },
            { "admin@EXAMPLE.RU", "EXAMPLE.RU" }
        })
    );

//  ----------------------------------------------------------------------------------------

    std::cout << "Enter text: ";

    auto input = ""s;
    std::getline(std::cin, input);

    for (auto const & email : extract_emails(input))
    {
        std::cout << email.address << " -> " << email.domain << '\n';
    }
}

////////////////////////////////////////////////////////////////////////////////////////////