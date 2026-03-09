#include <iostream>
#include <cmath>

double findMax(double numbers[], int size) {
    double max = numbers[0];
    for (int i = 1; i < size; i++) {
        if (numbers[i] > max) {
            max = numbers[i];
        }
    }
    return max;
}

double findMin(double numbers[], int size) {
    double min = numbers[0];
    for (int i = 1; i < size; i++) {
        if (numbers[i] < min) {
            min = numbers[i];
        }
    }
    return min;
}

double calculateMean(double numbers[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += numbers[i];
    }
    return sum / size;
}

double calculateStandardDeviation(double numbers[], int size, double mean) {
    double sumSquaredDifferences = 0.0;
    for (int i = 0; i < size; i++) {
        double difference = numbers[i] - mean;
        sumSquaredDifferences += difference * difference;
    }
    return std::sqrt(sumSquaredDifferences / size);
}

int main() {
    int size;
    
    std::cin >> size;
    
    if (size <= 0) {
        std::cout << "Размер должен быть положительным числом!" << std::endl;
        return 1;
    }
    
    double* numbers = new double[size];
    
    std::cout << "Введите " << size << " чисел:" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "Число " << (i + 1) << ": ";
        std::cin >> numbers[i];
    }
    
    double max = findMax(numbers, size);
    double min = findMin(numbers, size);
    double mean = calculateMean(numbers, size);
    double stdDev = calculateStandardDeviation(numbers, size, mean);
    
    std::cout << std::endl;
    std::cout << "Максимальное значение: " << max << std::endl;
    std::cout << "Минимальное значение: " << min << std::endl;
    std::cout << "Среднее арифметическое: " << mean << std::endl;
    std::cout << "Стандартное отклонение: " << stdDev << std::endl;
    
    delete[] numbers;
    
    return 0;
}
