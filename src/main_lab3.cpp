#include <iostream>

#include "../include/AES/GF_Service.h"
#include "../include/AES/AES.h"
#include "CipherModule.h"

std::string name_mode(Mode mode) {
    switch (mode) {
        case Mode::ECB:
            return "ECB-";
        case Mode::CBC:
            return "CBC-";
        case Mode::CTR:
            return "CTR-";
        case Mode::OFB:
            return "OFB-";
        case Mode::CFB:
            return "CFB-";
        case Mode::PCBC:
            return "PCBC-";
        case Mode::RandomDelta:
            return "RD-";
        default:
            return "";
    }
}

std::string name_pad(Padding pad) {
    switch (pad) {
        case Padding::Zeros:
            return "zeros";
        case Padding::ANSI_X923:
            return "ansi_x923";
        case Padding::ISO10126:
            return "ISO10126";
        case Padding::PKCS7:
            return "PKCS7";
        default:
            return "";
    }
}

int main() {
    // constexpr int len = 32;
    // uint8_t key128[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    // //uint8_t key192[24] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    // //uint8_t key256[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    // uint8_t message[] = "Hello world! It is me and my fri";
    // uint8_t ciphertext[len] = { 0 };
    // uint8_t plain[len] = { 0 };
    //
    // AES aes(len, 16, key128);
    // aes.encrypt(message, aes.exp_key, ciphertext);
    // aes.decrypt(ciphertext, aes.exp_key, plain);
    // std::cout << "Ciphertext: ";
    // for (int i = 0; i < len; ++i)
    //     printf("%02x ", ciphertext[i]);
    // std::cout << std::endl << "Plaintext: ";
    // for (int i = 0; i < len; ++i)
    //     printf("%c", plain[i]);
    // std::cout << std::endl;

    //uint8_t key128[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    //uint8_t key192[24] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    uint8_t key256[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};

    //uint8_t iv128[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    //uint8_t iv192[24] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t iv256[32] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};

    std::string img("../tests/simg.png");
    std::string encrypted_path("../tests/c.txt");

    constexpr Mode all_modes[] = {Mode::ECB, Mode::PCBC, Mode::CBC, Mode::CFB, Mode::OFB, Mode::CTR, Mode::RandomDelta};

    for (Mode mode: all_modes) {
        std::string decrypted_path("../tests/done/AES256-256-");
        decrypted_path += name_mode(mode);
        decrypted_path += ".png";

        AES *aes = new AES(32, 24, key256);

        CipherModule aes_module(aes, 32, aes->exp_key, 24, mode, Padding::PKCS7, iv256, { 4 });
        aes_module.encrypt_file(img, encrypted_path);
        aes_module.decrypt_file(encrypted_path, decrypted_path, 32);
        delete aes;
    }
}