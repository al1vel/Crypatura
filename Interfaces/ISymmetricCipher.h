#ifndef CRYPATURA_ISYMMETRICCIPHER_H
#define CRYPATURA_ISYMMETRICCIPHER_H
#include <cstdint>

class ISymmetricCipher {
public:
    virtual void encrypt(uint8_t *block, size_t len, uint8_t *key, size_t key_len, uint8_t *res) = 0;

    virtual void decrypt(uint8_t *block, size_t len, uint8_t *key, size_t key_len, uint8_t *res) = 0;

    virtual ~ISymmetricCipher() = default;
};

#endif //CRYPATURA_ISYMMETRICCIPHER_H
