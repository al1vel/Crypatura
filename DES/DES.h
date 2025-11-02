#ifndef CRYPATURA_DES_H
#define CRYPATURA_DES_H
#include "DESKeyExtension.h"
#include "DESRoundFunction.h"
#include "../Interfaces/IKeyExtension.h"
#include "../Interfaces/IRoundFunction.h"
#include "../Interfaces/ISymmetricCipher.h"
#include "../FestelNet.h"

class DES : public ISymmetricCipher {
private:
    FestelNet festel_net;

public:
    DES();

    ~DES();

    void encrypt(uint8_t *block, size_t len, uint8_t *key, size_t key_len, uint8_t *res) override;

    void decrypt(uint8_t *block, size_t len, uint8_t *key, size_t key_len, uint8_t *res) override;
};


#endif //CRYPATURA_DES_H
