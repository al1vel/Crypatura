#include <cstdint>
#include <iostream>

#include "CipherModule.h"
#include "DES/DES.h"

int main() {
    uint8_t text[] = "Some text to check if DES works.\nIf you see this, I half won!";
    uint8_t key[8] = {10, 23, 54, 3, 124, 43, 76, 255};

    // uint8_t text[] = "Some text to check DES.\nSome more text.";
    // uint8_t key[8] = {0, 124, 2, 0, 12, 0, 0, 132};


    DES* des_encryptor = new DES();
    CipherModule cipher_module(des_encryptor, key, 64, Mode::ECB, Padding::Zeros);

    constexpr uint64_t blocks = ((sizeof(text) + 8 - 1) / 8);

    uint8_t ciphertext[blocks * 8] = {0};
    cipher_module.encrypt(text, sizeof(text), ciphertext);

    printf("%s\n", "Ciphertext:");
    for (size_t i = 0; i < blocks * 8; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n\n");

    uint8_t plaintext[blocks * 8] = {0};
    cipher_module.decrypt(ciphertext, blocks * 8, plaintext);

    printf("%s\n", "Plain text:");
    for (size_t i = 0; i < (sizeof(text)); i++) {
        //printf("%02X ", plaintext[i]);
        printf("%c", plaintext[i]);
    }
    printf("\n");

    delete des_encryptor;
    return 0;
}
