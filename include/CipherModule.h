#ifndef CRYPATURA_CIPHERMODULE_H
#define CRYPATURA_CIPHERMODULE_H

#include <any>
#include <cstdint>
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

    int getThreadsCount(int default_value) const;

    uint8_t* encrypt(uint8_t *data, size_t len_bytes, size_t* out_len) const;

    uint8_t* decrypt(uint8_t *data, size_t len, size_t* out_len) const;

    void encrypt_file(const std::string &inputPath, const std::string &outputPath) const;

    void decrypt_file(const std::string &inputPath, const std::string &outputPath) const;

    void ECB_thread(int index, int threads_cnt, int total_blocks, uint8_t* data, uint8_t* res, bool enc) const;

    void CBC_thread(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res) const;

    void CFB_thread(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res) const;

    void Delta_thread_enc(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res, int delta) const;

    void Delta_thread_dec(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res, int delta) const;
};


#endif //CRYPATURA_CIPHERMODULE_H
