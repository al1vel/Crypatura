#ifndef CRYPATURA_TRIPLEDES_H
#define CRYPATURA_TRIPLEDES_H

#include "../Interfaces/ISymmetricCipher.h"

class TripleDES : public ISymmetricCipher {
    void encrypt(uint8_t *block, uint8_t *key, uint8_t *res) override;

    void decrypt(uint8_t *block, uint8_t *key, uint8_t *res) override;
};


#endif //CRYPATURA_TRIPLEDES_H
