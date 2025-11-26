#include <cstdint>
#include <iostream>

#include "CipherModule.h"
#include "DES/DES.h"
#include "DEAL/DEAL.h"

int main() {
    //uint8_t text[] = "Some text to check if DES works.\nIf you see this, I half won!";
    uint8_t key[8] = {10, 23, 54, 3, 124, 43, 76, 255};
    uint8_t deal_key[16] = {10, 23, 54, 3, 124, 43, 76, 255, 0, 1, 2, 3, 4, 5, 6, 7};

    uint8_t iv[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t deal_iv[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};

    DES* des_encryptor = new DES();
    DEAL* deal_encryptor = new DEAL(128);

    CipherModule cipher_module(des_encryptor, 8, key, 64, Mode::CBC, Padding::Zeros, iv, { 4 });
    CipherModule deal(deal_encryptor, 16, deal_key, 128, Mode::ECB, Padding::Zeros, deal_iv, { 8 });

    // std::cout << "Text size: " << sizeof(text) << std::endl;
    // size_t enc_out_len = 0;
    // uint8_t* cipher = cipher_module.encrypt(text, sizeof(text), &enc_out_len);
    //
    // std::cout << "Cipher (" << enc_out_len << " bytes):" << std::endl;
    // for (size_t i = 0; i < enc_out_len; i++) {
    //     printf("%02X ", cipher[i]);
    // }
    // printf("\n\n");
    //
    // size_t dec_out_len = 0;
    // uint8_t* plain = cipher_module.decrypt(cipher, enc_out_len, &dec_out_len);
    //
    // std::cout << "Plain (" << dec_out_len << " bytes):" << std::endl;
    // for (size_t i = 0; i < dec_out_len; ++i) {
    //     printf("%c", plain[i]);
    // }
    // printf("\n");
    //
    // delete des_encryptor;
    // delete cipher;
    // delete plain;

    // std::cout << "Text size: " << sizeof(text) << std::endl;
    // size_t enc_out_len = 0;
    // uint8_t* cipher = deal.encrypt(text, sizeof(text), &enc_out_len);
    //
    // std::cout << "Cipher (" << enc_out_len << " bytes):" << std::endl;
    // for (size_t i = 0; i < enc_out_len; i++) {
    //     printf("%02X ", cipher[i]);
    // }
    // printf("\n\n");
    //
    // size_t dec_out_len = 0;
    // uint8_t* plain = deal.decrypt(cipher, enc_out_len, &dec_out_len);
    //
    // std::cout << "Plain (" << dec_out_len << " bytes):" << std::endl;
    // for (size_t i = 0; i < dec_out_len; ++i) {
    //     printf("%c", plain[i]);
    // }
    // printf("\n");
    //
    // delete des_encryptor;
    // delete cipher;
    // delete plain;

    //std::string text("../tests/t.txt");
    // std::string encrypted_text("../tests/c.txt");
    // std::string decrypted_text("../tests/d.txt");

    // std::string vid("../tests/vid.mp4");
    // std::string encrypted_text("../tests/c.txt");
    // std::string decrypted_text("../tests/d.mp4");

    std::string img("../tests/img2.png");
    std::string encrypted_text("../tests/c.txt");
    std::string decrypted_text("../tests/d.png");

    //
    // cipher_module.encrypt_file(vid, encrypted_text);
    // cipher_module.decrypt_file(encrypted_text, decrypted_text);

    deal.encrypt_file(img, encrypted_text);
    deal.decrypt_file(encrypted_text, decrypted_text, 16);

    return 0;
}
