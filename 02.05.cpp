#include <iostream>

double calculateE(double epsilon) {
    double e = 1.0;
    double term = 1.0;
    int n = 1;
    
    while (term >= epsilon) {
        // term(n) = term(n-1) / n
        term = term / n;
        e += term;
        n++;
    }
    
    return e;
}

int main() {
    double epsilon = 0;
    
    std::cout << "Введите точность (epsilon): ";
    std::cin >> epsilon;
    
    if (epsilon <= 0) {
        std::cout << "Точность должна быть положительным числом!" << std::endl;
        return 1;
    }
    
    double e = calculateE(epsilon);
    
    std::cout << "Число e с точностью " << epsilon << " = " << e << std::endl;
    
    return 0;
}
