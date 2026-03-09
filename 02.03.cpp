#include <iostream>

// Функция для классификации символа ASCII
std::string classifyChar(char c) {
    switch (c) {
        // Заглавные буквы A-Z (65-90)
        case 'A' ... 'Z':
            return "Заглавные буквы";
        
        // Строчные буквы a-z (97-122)
        case 'a' ... 'z':
            return "Строчные буквы";
        
        // Десятичные цифры 0-9 (48-57)
        case '0' ...  '9':
            return "Десятичные цифры";
        
        // Знаки препинания (33-47, 58-64, 91-96, 123-127)
        case '!' ... '/':
        case ':' ... '@':
        case '[' ... '`':
        case '{' ... '~':
            return "Знаки препинания";
        
        // Прочие символы (default)
        default:
            return "Прочие символы";
    }
}

int main() {
    char symbol = 0;
    
    std::cout << "Введите символ для классификации: ";
    std::cin.get(symbol);
    
    std::cout << "Символ: '" << symbol << "' (код: " << static_cast<int>(symbol) << ")" << std::endl;
    std::cout << "Класс: " << classifyChar(symbol) << std::endl;
    
    return 0;
}
