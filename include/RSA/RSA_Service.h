#ifndef CRYPATURA_RSA_SERVICE_H
#define CRYPATURA_RSA_SERVICE_H

#include "../../include/big_int.h"

class RSA_Service {
public:
    struct PublicKey {
        BigInt n;
        BigInt e;
    };

    struct PrivateKey {
        BigInt n;
        BigInt d;
    };

    class KeyGenerator {
    public:
        enum class PrimalityTest {
            Fermat,
            SoloveyStrassen,
            MillerRabin,
        };

        KeyGenerator(RSA_Service &parent, PrimalityTest p_test, double p_min, size_t bit_len);

        void generate_keys() const;

    private:
        RSA_Service &service_;
        PrimalityTest p_test;
        size_t bit_len;
        double p_min;
    };

    RSA_Service(KeyGenerator::PrimalityTest test, double min_prime_probability, size_t bit_length);

    void generate_rsa_keys() const;

    BigInt encrypt(const BigInt &message) const;

    BigInt decrypt(const BigInt &cipher) const;

private:
    PublicKey public_key;
    PrivateKey private_key;
    KeyGenerator keys_generator;
};


#endif //CRYPATURA_RSA_SERVICE_H
