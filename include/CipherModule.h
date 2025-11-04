#ifndef CRYPATURA_CIPHERMODULE_H
#define CRYPATURA_CIPHERMODULE_H

#include <any>
#include <string>
#include <vector>
#include "Interfaces/ISymmetricCipher.h"

enum class Mode { ECB, CBC, PCBC, CFB, OFB, CTR, RandomDelta };

enum class Padding { Zeros, ANSI_X923, PKCS7, ISO10126 };

class CipherModule {
private:
    ISymmetricCipher *cipher;
    uint8_t *key;
    size_t key_len;
    Mode mode;
    Padding padding;
    uint8_t *iv;
    std::vector<std::any> additionalParams;

public:
    CipherModule(ISymmetricCipher *cipher, uint8_t *key, size_t key_len, Mode mode, Padding padding,
                 uint8_t *iv = nullptr, std::initializer_list<std::any> additional = {});

    void encrypt(uint8_t *data, size_t len_bytes, uint8_t *out) const;

    void decrypt(uint8_t *data, size_t len, uint8_t *out) const;

    void encrypt_file(const std::string &inputPath, const std::string &outputPath);

    void decrypt_file(const std::string &inputPath, const std::string &outputPath);
};


#endif //CRYPATURA_CIPHERMODULE_H
