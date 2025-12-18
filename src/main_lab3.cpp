#include <iostream>

#include "../include/AES/GF_Service.h"
#include "../include/AES/AES.h"

int main() {
    uint8_t key128[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t message[] = "Hello world! It";
    uint8_t ciphertext[16] = { 0 };
    uint8_t plain[16] = { 0 };

    AES aes(16, 16, key128);
    aes.encrypt(message, key128, ciphertext);
    aes.decrypt(ciphertext, key128, plain);
    std::cout << ciphertext << std::endl;
    std::cout << plain << std::endl;
}