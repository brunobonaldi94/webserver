#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

std::string generateRandomString(int length) {
    // Caracteres permitidos na string aleatória
    const std::string charset =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()-_=+";

    // Inicializa o gerador de números aleatórios com o tempo atual
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    std::string randomString;
    for (int i = 0; i < length; ++i) {
        // Gera um índice aleatório para selecionar um caractere do conjunto de caracteres
        int randomIndex = std::rand() % charset.length();
        randomString += charset[randomIndex];
    }

    return randomString;
}

int main() {
    int length = 12; // Tamanho da string aleatória desejada
    std::string randomStr = generateRandomString(30);
    std::cout << "String Aleatória: " << randomStr << std::endl;

    return 0;
}
