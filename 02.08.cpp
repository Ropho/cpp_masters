#include <iostream>
#include <vector>

std::size_t calculateCollatzLength(unsigned long long int n, std::vector<std::size_t>& cache) {
    if (n < cache.size() && cache[n] != 0) {
        return cache[n];
    }
    
    std::size_t length = 1; 
    unsigned long long int current = n;
    
    while (current != 1) {
        if (current % 2 == 0) {
            current = current / 2; 
        } else {
            current = 3 * current + 1; 
        }
        length++;
        
        
        if (current < cache.size() && cache[current] != 0) {
            length += cache[current] - 1;
            break;
        }
    }
    
    
    if (n < cache.size()) {
        cache[n] = length;
    }
    
    return length;
}

int main() {
    const unsigned long long int MAX_VALUE = 100;
    
    std::vector<std::size_t> cache(MAX_VALUE * 10, 0);
    
    std::size_t maxLength = 0;
    unsigned long long int maxStartValue = 1;
    
    
    for (unsigned long long int start = 1; start <= MAX_VALUE; start++) {
        std::size_t length = calculateCollatzLength(start, cache);
        
        if (length > maxLength) {
            maxLength = length;
            maxStartValue = start;
        }
        
        std::cout << "Start value: " << start << ", Length: " << length << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Result:" << std::endl;
    std::cout << "Maximum sequence length: " << maxLength << std::endl;
    std::cout << "Start value: " << maxStartValue << std::endl;
    
    return 0;
}
