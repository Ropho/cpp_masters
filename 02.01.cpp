#include <cmath>
#include <iostream>


int bineFormula(int n) {

    if (n < 0) {
        return -1;
    }

    const double first = (1 + std::sqrt(5)) / 2;
    const double second = (1 - std::sqrt(5)) / 2;

    double chislitel = std::pow(first, n) - std::pow(second, n);
    const double znamenatel = std::sqrt(5);

    double answer = std::round(chislitel / znamenatel);

    return static_cast<int>(answer);
}

int main () {

    int n = 0;
    std::cin>>n;

    std::cout<<bineFormula(n)<<std::endl;
    return 0;
}
