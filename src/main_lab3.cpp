#include <iostream>

#include "../include/AES/GF_Service.h"
#include "../include/AES/AES.h"

int main() {
    constexpr int len = 32;
    uint8_t key128[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    //uint8_t key192[24] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    //uint8_t key256[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    uint8_t message[] = "Hello world! It is me and my fri";
    uint8_t ciphertext[len] = { 0 };
    uint8_t plain[len] = { 0 };

    AES aes(len, 16, key128);
    aes.encrypt(message, aes.exp_key, ciphertext);
    aes.decrypt(ciphertext, aes.exp_key, plain);
    std::cout << "Ciphertext: ";
    for (int i = 0; i < len; ++i)
        printf("%02x ", ciphertext[i]);
    std::cout << std::endl << "Plaintext: ";
    for (int i = 0; i < len; ++i)
        printf("%c", plain[i]);
    std::cout << std::endl;
}