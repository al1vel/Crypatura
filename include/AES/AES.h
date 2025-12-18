#ifndef CRYPATURA_AES_H
#define CRYPATURA_AES_H

#include <cstddef>
#include "../Interfaces/ISymmetricCipher.h"
#include "GF_Service.h"

class AES : public ISymmetricCipher {
private:
    uint8_t* init_key;

public:
    uint8_t* exp_key;
    size_t key_len;
    size_t block_len;
    uint8_t S_box[256] = {};
    uint8_t S_box_inv[256] = {};

    AES(size_t block_len, size_t key_len, uint8_t* init_key);

    ~AES();

    void encrypt(uint8_t *block, uint8_t *key, uint8_t *res) override;

    void decrypt(uint8_t *block, uint8_t *key, uint8_t *res) override;
};


#endif //CRYPATURA_AES_H
