#include <iostream>
#include <cmath>
#include <complex>

const double EPSILON = 1e-9;

enum SolutionType {
    NO_SOLUTIONS        = 0,        // нет решений
    ONE_SOLUTION        = 1,        // одно решение
    TWO_SOLUTIONS       = 2,        // два решения
    INFINITE_SOLUTIONS  = 3,        // бесконечно много решений
    COMPLEX_SOLUTIONS   = 4         // комплексные решения
};

bool isZero(double value) {
    return std::abs(value) < EPSILON;
}

void printComplex(const std::complex<double> z) {
    double real = z.real();
    double imag = z.imag();
    
    std::cout << real;
    if (imag >= 0) {
        std::cout << " + " << imag << "i";
    } else {
        std::cout << " - " << std::abs(imag) << "i";
    }
}


struct QuadraticResult {
    SolutionType type;
    double x1, x2;
    std::complex<double> z1, z2;
};

QuadraticResult solveQuadratic(double a, double b, double c) {
    
    QuadraticResult result = {};
    
    if (isZero(a)) {

        // линейное уравнение: bx + c = 0
        if (isZero(b)) {
            
            if (isZero(c)) {
                result.type = SolutionType::INFINITE_SOLUTIONS;
            } else {
                result.type = SolutionType::NO_SOLUTIONS;
            }

            return result;
        } 
        
        // Линейное уравнение: bx + c = 0
        result.type = SolutionType::ONE_SOLUTION;
        result.x1 = -c / b;
        
        return result;
    }
    
    double discriminant = b * b - 4 * a * c;

    if (isZero(discriminant)) {
        result.type = SolutionType::ONE_SOLUTION; 
        result.x1 = -b / (2 * a);

    } else if (discriminant > 0) {
        result.type = SolutionType::TWO_SOLUTIONS; 
        double sqrt_discriminant = std::sqrt(discriminant);
        result.x1 = (-b + sqrt_discriminant) / (2 * a);
        result.x2 = (-b - sqrt_discriminant) / (2 * a);

    } else {
        result.type = SolutionType::COMPLEX_SOLUTIONS;
        std::complex<double> sqrt_discriminant(0, std::sqrt(-discriminant));
        result.z1 = (-b + sqrt_discriminant) / (2 * a);
        result.z2 = (-b - sqrt_discriminant) / (2 * a);
    }
    
    return result;
}

int main() {
    double a, b, c = 0;
    
    std::cout << "Введите коэффициенты a, b, c: ";
    std::cin >> a >> b >> c;
    std::cout << "\nУравнение: " << a << "x² + " << b << "x + " << c << " = 0" << std::endl;
    
    QuadraticResult result = solveQuadratic(a, b, c);
        switch (result.type) {
        case SolutionType::NO_SOLUTIONS:
            std::cout << "Уравнение не имеет решений" << std::endl;
            break;
        case SolutionType::ONE_SOLUTION:
            std::cout << "Один корень: x = " << result.x1 << std::endl;
            break;
        case SolutionType::TWO_SOLUTIONS:
            std::cout << "Два действительных корня:" << std::endl;
            std::cout << "x1 = " << result.x1 << std::endl;
            std::cout << "x2 = " << result.x2 << std::endl;
            break;
        case SolutionType::INFINITE_SOLUTIONS:
            std::cout << "Уравнение имеет бесконечно много решений" << std::endl;
            break;
        case SolutionType::COMPLEX_SOLUTIONS:
            std::cout << "Комплексные корни:" << std::endl;
            std::cout << "x1 = ";
            printComplex(result.z1);
            std::cout << std::endl;
            std::cout << "x2 = ";
            printComplex(result.z2);
            std::cout << std::endl;
            break;
    }
    
    return 0;
}