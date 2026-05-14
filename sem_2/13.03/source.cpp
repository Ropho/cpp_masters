#include <iostream>

int main()
{
    // This line comment must be removed.

    auto text = R"(Raw string keeps // comment text

    
and /* block comment text */ inside the string.)";

    auto marked = R"tag(Delimiter can be custom: )" is not the end here.

Only the matching delimiter closes this literal.)tag";

    /* This block comment must be removed.
       The blank line left after it must be removed too. */

    std::cout << text << '\n' << marked << '\n';
}
