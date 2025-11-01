#ifndef CRYPATURA_CIPHERMODULE_H
#define CRYPATURA_CIPHERMODULE_H

#include <any>
#include <string>
#include <vector>
#include "Interfaces/ISymmetricCipher.h"

class CipherModule {
    ISymmetricCipher* cipher;
    uint8_t* key;
    size_t key_len;
    enum class Mode { ECB, CBC, PCBC, CFB, OFB, CTR, RandomDelta } mode;
    enum class Padding { Zeros, ANSI_X923, PKCS7, ISO10126 } padding;
    uint8_t* iv;
    std::vector<std::any> additionalParams;

    CipherModule(ISymmetricCipher* cipher, uint8_t* key, size_t key_len, Mode mode, Padding padding, uint8_t* iv = nullptr, std::initializer_list<std::any> additional = {});

    void encrypt(uint8_t* data, size_t len, uint8_t* out);

    void decrypt(uint8_t* data, size_t len, uint8_t* out);

    void encrypt_file(const std::string& inputPath, const std::string& outputPath);

    void decrypt_file(const std::string& inputPath, const std::string& outputPath);
};


#endif //CRYPATURA_CIPHERMODULE_H