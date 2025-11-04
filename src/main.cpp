#include <cstdint>
#include <iostream>
#include <ostream>

#include "CipherModule.h"
#include "DES/DES.h"

int main() {
    uint8_t text[] = "Some text to check if DES works.\nIf you see this, I half won!";
    uint8_t key[8] = {52, 200, 10, 1, 0, 15, 101, 91};

    DES* des_encryptor = new DES();
    CipherModule cipher_module(des_encryptor, key, 64, Mode::ECB, Padding::Zeros);

    uint8_t* ciphertext = nullptr;
    cipher_module.encrypt(text, sizeof(text), ciphertext);

    printf("%s\n", "Ciphertext:\n");
    for (size_t i = 0; i < sizeof(text); i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    uint8_t* plaintext = nullptr;
    cipher_module.decrypt(ciphertext, sizeof(text), plaintext);

    std::cout << plaintext << std::endl;

    delete[] plaintext;
    delete[] ciphertext;
    return 0;
}
