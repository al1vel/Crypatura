#include <cstdint>
#include <iostream>

#include "CipherModule.h"
#include "DES/DES.h"

int main() {
    uint8_t text[] = "Some text to check if DES works.\nIf you see this, I half won!";
    uint8_t key[8] = {52, 200, 10, 1, 0, 15, 101, 91};

    DES* des_encryptor = new DES();
    CipherModule cipher_module(des_encryptor, key, 64, Mode::ECB, Padding::Zeros);

    size_t cipher_blocks = sizeof(text) / 8;
    if (sizeof(text) % 8 != 0) {
        cipher_blocks += 1;
    }

    auto* ciphertext = new uint8_t[cipher_blocks * 8];
    cipher_module.encrypt(text, sizeof(text), ciphertext);

    printf("%s\n", "Ciphertext:");
    for (size_t i = 0; i < cipher_blocks * 8; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    auto* plaintext = new uint8_t[cipher_blocks * 8];
    cipher_module.decrypt(ciphertext, sizeof(text), plaintext);

    printf("%s\n", "Plain text:");
    for (size_t i = 0; i < cipher_blocks * 8; i++) {
        printf("%02X ", plaintext[i]);
    }
    printf("\n");

    delete[] plaintext;
    delete[] ciphertext;
    return 0;
}
