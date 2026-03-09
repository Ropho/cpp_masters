
#include <iostream>
#include <numeric>
#include <cmath>
#include <vector>
#include <utility>

int recursive_gcd (int a, int b) {
    if (b == 0) {
        return a;
    }
    return recursive_gcd(b, a % b);
}

int iterative_gcd (int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    return a * b / std::gcd(a, b);
}

void run_unit_tests() {
    int tests_passed = 0;
    int tests_total = 0;
    
    std::vector<std::pair<int, int>> gcd_test_cases = {
        {48, 18},
        {18, 48},
        {100, 25},
        {17, 13},
        {0, 5},
        {5, 0},
        {0, 0},
        {1, 1},
        {100, 1},
        {123456, 7890},
        {-48, 18},
        {48, -18},
        {-48, -18},
        {1, 1000000},
        {1000000, 1}
    };
    
    std::cout << "=== Testing recursive_gcd ===" << std::endl;
    for (const auto& [a, b] : gcd_test_cases) {
        tests_total++;
        int result = std::abs(recursive_gcd(a, b));
        int std_result = std::gcd(std::abs(a), std::abs(b));
        
        if (result == std_result) {
            std::cout << "✓ Test " << tests_total << ": gcd(" << a << ", " << b 
                      << ") = " << result << " [PASSED]" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test " << tests_total << ": gcd(" << a << ", " << b 
                      << ") = " << result << ", std::gcd = " << std_result << " [FAILED]" << std::endl;
        }
    }
    
    std::cout << "\n=== Testing iterative_gcd ===" << std::endl;
    for (const auto& [a, b] : gcd_test_cases) {
        tests_total++;
        int result = std::abs(iterative_gcd(a, b));
        int std_result = std::gcd(std::abs(a), std::abs(b));
        
        if (result == std_result) {
            std::cout << "✓ Test " << tests_total << ": gcd(" << a << ", " << b 
                      << ") = " << result << " [PASSED]" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test " << tests_total << ": gcd(" << a << ", " << b 
                      << ") = " << result << ", std::gcd = " << std_result << " [FAILED]" << std::endl;
        }
    }
    
    std::vector<std::pair<int, int>> lcm_test_cases = {
        {4, 6},
        {6, 4},
        {5, 7},
        {12, 18},
        {1, 100},
        {100, 1},
        {10, 10},
        {15, 20},
        {21, 14},
        {100, 25},
        {1, 1},
        {7, 13},
        {48, 18}
    };
    
    std::cout << "\n=== Testing lcm ===" << std::endl;
    for (const auto& [a, b] : lcm_test_cases) {
        tests_total++;
        int result = std::abs(lcm(a, b));
        long long std_result = std::lcm(std::abs(static_cast<long long>(a)), 
                                       std::abs(static_cast<long long>(b)));
        
        if (result == std_result) {
            std::cout << "✓ Test " << tests_total << ": lcm(" << a << ", " << b 
                      << ") = " << result << " [PASSED]" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test " << tests_total << ": lcm(" << a << ", " << b 
                      << ") = " << result << ", std::lcm = " << std_result << " [FAILED]" << std::endl;
        }
    }
    
    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "Tests passed: " << tests_passed << " / " << tests_total << std::endl;
    std::cout << "Success rate: " << (100.0 * tests_passed / tests_total) << "%" << std::endl;
    
    if (tests_passed == tests_total) {
        std::cout << "\n✓ All tests passed successfully!" << std::endl;
    } else {
        std::cout << "\n✗ Some tests failed!" << std::endl;
    }
}

int main() {
    run_unit_tests();
    return 0;
}