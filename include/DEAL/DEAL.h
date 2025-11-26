#ifndef CRYPATURA_DEAL_H
#define CRYPATURA_DEAL_H

#include "../Interfaces/ISymmetricCipher.h"
#include "../FestelNet.h"

class DEAL : public ISymmetricCipher {
private:
    FestelNet festel_net;
    size_t key_len;

public:
    DEAL(size_t key_length);

    void encrypt(uint8_t *block, uint8_t *key, uint8_t *res) override;

    void decrypt(uint8_t *block, uint8_t *key, uint8_t *res) override;
};


#endif //CRYPATURA_DEAL_H