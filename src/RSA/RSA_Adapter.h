#ifndef CRYPATURA_RSA_ADAPTER_H
#define CRYPATURA_RSA_ADAPTER_H

#include "../include/Interfaces/ISymmetricCipher.h"
#include "../include/RSA/RSA_Service.h"

class RSA_Adapter : public ISymmetricCipher {
private:
    RSA_Service* rsa_service = nullptr;

public:
    RSA_Adapter(RSA_Service::KeyGenerator::PrimalityTest test, double min_prime_prob, size_t bit_length);

    ~RSA_Adapter();

    void encrypt(uint8_t *block, uint8_t *key, uint8_t *res) override;

    void decrypt(uint8_t *block, uint8_t *key, uint8_t *res) override;
};


#endif //CRYPATURA_RSA_ADAPTER_H
