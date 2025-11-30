#include <cstdint>

#include "CipherModule.h"
#include "DES/DES.h"
#include "DEAL/DEAL.h"

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
    uint8_t key[8] = {10, 23, 54, 3, 124, 43, 76, 255};
    uint8_t deal_key128[16] = {10, 23, 54, 3, 124, 43, 76, 255, 0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t deal_key192[24] = {10, 23, 54, 3, 124, 43, 76, 255, 0, 1, 2, 3, 4, 5, 6, 7, 213, 32, 12, 32, 5, 7, 9, 9};
     uint8_t deal_key256[32] = {
         10, 23, 54, 3, 124, 43, 76, 255, 0, 1, 2, 3, 4, 5, 6, 7, 23, 23, 1, 3, 45, 75, 23, 123, 42, 0, 32, 1, 3, 42, 52,
         52
     };

    uint8_t iv[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t deal_iv[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};

    DES *des_encryptor = new DES();
    DEAL *deal128_encryptor = new DEAL(128);
    DEAL *deal192_encryptor = new DEAL(192);
    DEAL *deal256_encryptor = new DEAL(256);

    std::string img("../tests/simg.png");
    std::string encrypted_path("../tests/c.txt");

    constexpr Mode all_modes[] = {Mode::ECB, Mode::PCBC, Mode::CBC, Mode::CFB, Mode::OFB, Mode::CTR, Mode::RandomDelta};
    constexpr Padding all_pads[] = {Padding::Zeros, Padding::ANSI_X923, Padding::ISO10126, Padding::PKCS7};

    for (Mode mode: all_modes) {
        for (Padding pad: all_pads) {
            std::string decrypted_path("../tests/done/DES-");
            decrypted_path += name_mode(mode);
            decrypted_path += name_pad(pad);
            decrypted_path += ".png";

            CipherModule des_module(des_encryptor, 8, key, 64, mode, pad, iv, { 8 });
            des_module.encrypt_file(img, encrypted_path);
            des_module.decrypt_file(encrypted_path, decrypted_path, 8);
        }
    }

    for (Mode mode: all_modes) {
        for (Padding pad: all_pads) {
            std::string decrypted_path("../tests/done/DEAL128-");
            decrypted_path += name_mode(mode);
            decrypted_path += name_pad(pad);
            decrypted_path += ".png";

            CipherModule deal128_module(deal128_encryptor, 16, deal_key128, 128, mode, pad, deal_iv, { 8 });
            deal128_module.encrypt_file(img, encrypted_path);
            deal128_module.decrypt_file(encrypted_path, decrypted_path, 16);
        }
    }

    for (Mode mode: all_modes) {
        for (Padding pad: all_pads) {
            std::string decrypted_path("../tests/done/DEAL192-");
            decrypted_path += name_mode(mode);
            decrypted_path += name_pad(pad);
            decrypted_path += ".png";

            CipherModule deal128_module(deal192_encryptor, 16, deal_key192, 192, mode, pad, deal_iv, { 8 });
            deal128_module.encrypt_file(img, encrypted_path);
            deal128_module.decrypt_file(encrypted_path, decrypted_path, 16);
        }
    }

    for (Mode mode: all_modes) {
        for (Padding pad: all_pads) {
            std::string decrypted_path("../tests/done/DEAL256-");
            decrypted_path += name_mode(mode);
            decrypted_path += name_pad(pad);
            decrypted_path += ".png";

            CipherModule deal128_module(deal256_encryptor, 16, deal_key256, 256, mode, pad, deal_iv, { 8 });
            deal128_module.encrypt_file(img, encrypted_path);
            deal128_module.decrypt_file(encrypted_path, decrypted_path, 16);
        }
    }

    // std::cout << "Text size: " << sizeof(text) << std::endl;
    // size_t enc_out_len = 0;
    // uint8_t* cipher = cipher_module.encrypt(text, sizeof(text), &enc_out_len);
    //
    // std::cout << "Cipher (" << enc_out_len << " bytes):" << std::endl;
    // for (size_t i = 0; i < enc_out_len; i++) {
    //     //printf("%02X ", cipher[i]);
    //     printf("%c", cipher[i]);
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

    // std::cout << "Text size: " << sizeof(text) << std::endl;
    // size_t enc_out_len = 0;
    // uint8_t* cipher = deal.encrypt(text, sizeof(text), &enc_out_len);
    //
    // std::cout << "Cipher (" << enc_out_len << " bytes):" << std::endl;
    // for (size_t i = 0; i < enc_out_len; i++) {
    //     //printf("%02X ", cipher[i]);
    //     printf("%c", cipher[i]);
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


    //std::string text("../tests/t.txt");
    // std::string encrypted_text("../tests/c.txt");
    // std::string decrypted_text("../tests/d.txt");

    // std::string vid("../tests/vid.mp4");
    // std::string encrypted_text("../tests/c.txt");
    // std::string decrypted_text("../tests/d.mp4");

    // std::string img("../tests/img2.png");
    // std::string encrypted_text("../tests/c.txt");
    // std::string decrypted_text("../tests/d.png");

    //
    //cipher_module.encrypt_file(img, encrypted_text);
    //cipher_module.decrypt_file(encrypted_text, decrypted_text, 8);

    // deal.encrypt_file(img, encrypted_text);
    // deal.decrypt_file(encrypted_text, decrypted_text, 16);

    //delete des_encryptor;
    //delete deal_encryptor;
    // delete[] cipher;
    // delete[] plain;

    return 0;
}
