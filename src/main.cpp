#include <cstdint>
#include <iostream>

#include "CipherModule.h"
#include "DES/DES.h"

int main() {
    uint8_t text[] = "Some text to check if DES works.\nIf you see this, I half won!!1";
    uint8_t key[8] = {10, 23, 54, 3, 124, 43, 76, 255};

    // uint8_t text[] = "Some text to check DES.\nSome more text.";
    // uint8_t key[8] = {0, 124, 2, 0, 12, 0, 0, 132};

    uint8_t iv[8] = {1, 2, 3, 4, 5, 6, 7, 8};


    DES* des_encryptor = new DES();
    CipherModule cipher_module(des_encryptor, key, 64, Mode::CTR, Padding::ISO10126, iv, { 2 });

    std::cout << "Text size: " << sizeof(text) << std::endl;
    size_t enc_out_len = 0;
    uint8_t* cipher = cipher_module.encrypt(text, sizeof(text), &enc_out_len);

    std::cout << "Cipher (" << enc_out_len << " bytes):" << std::endl;
    for (size_t i = 0; i < enc_out_len; i++) {
        printf("%02X ", cipher[i]);
    }
    printf("\n\n");

    size_t dec_out_len = 0;
    uint8_t* plain = cipher_module.decrypt(cipher, enc_out_len, &dec_out_len);

    std::cout << "Plain (" << dec_out_len << " bytes):" << std::endl;
    for (size_t i = 0; i < dec_out_len; ++i) {
        printf("%c", plain[i]);
    }
    printf("\n");

    delete des_encryptor;
    delete cipher;
    delete plain;
    return 0;
}
