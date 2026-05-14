#include <iostream>
int main()
{
    auto text = R"(Raw string keeps // comment text

    
and /* block comment text */ inside the string.)";
    auto marked = R"tag(Delimiter can be custom: )" is not the end here.

Only the matching delimiter closes this literal.)tag";
    std::cout << text << '\n' << marked << '\n';
}
