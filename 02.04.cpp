#include <iostream>
#include <vector>

int power(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }

    return result;
}

bool isArmstrong(int number) {
    int original = number;
    int sum = 0;
    int digit;
    
    while (number > 0) {
        digit = number % 10;
        sum += power(digit, 3);
        number /= 10;
    }
    
    return sum == original;
}

std::vector<int> findArmstrongNumbers() {
    std::vector<int> armstrongNumbers;
    
    for (int hundreds = 1; hundreds <= 9; hundreds++) {
        for (int tens = 0; tens <= 9; tens++) {
            for (int ones = 0; ones <= 9; ones++) {
                int number = hundreds * 100 + tens * 10 + ones;
                
                if (isArmstrong(number)) {
                    armstrongNumbers.push_back(number);
                }
            }
        }
    }
    
    return armstrongNumbers;
}

int main() {    
    std::vector<int> armstrongNumbers = findArmstrongNumbers();
    
    std::cout << "Найденные числа Армстронга:" << std::endl;
    for (size_t i = 0; i < armstrongNumbers.size(); i++) {
        int number = armstrongNumbers[i];
        int hundreds = number / 100;
        int tens = (number / 10) % 10;
        int ones = number % 10;
        
        std::cout << (i + 1) << ". " << number << " = ";
        std::cout << hundreds << "³ + " << tens << "³ + " << ones << "³ = ";
        std::cout << power(hundreds, 3) << " + " << power(tens, 3) << " + " << power(ones, 3);
        std::cout << " = " << number << std::endl;
    }
    
    std::cout << std::endl;
    
    return 0;
}
