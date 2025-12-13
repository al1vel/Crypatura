#include "../../include/RSA/RSA_Service.h"
#include "../../include/Interfaces/IPrimalityTest.h"
#include "../../include/RSA/PrimeTest.h"
#include "../../include/RSA/FermatTest.h"
#include "../../include/RSA/SolStrTest.h"
#include "../../include/RSA/MilRabTest.h"
#include "../../include/RSA/Service.h"

RSA_Service::KeyGenerator::KeyGenerator(RSA_Service &parent, PrimalityTest p_test, double p_min,
                                        size_t bit_len) : service_(parent) {
    this->p_test = p_test;
    this->p_min = p_min;
    this->bit_len = bit_len;
}

void RSA_Service::KeyGenerator::generate_keys() const {
    PrimeTest *test = nullptr;
    switch (this->p_test) {
        case PrimalityTest::Fermat: {
            test = new FermatTest();
            break;
        }
        case PrimalityTest::SoloveyStrassen: {
            test = new SolStrTest();
            break;
        }
        case PrimalityTest::MillerRabin: {
            test = new MilRabTest();
            break;
        }
        default:
            break;
    }

    if (test == nullptr) {
        return;
    }

    while (true) {
        BigInt p, q;
        while (true) {
            p = BigInt::random_odd_with_len(bit_len);
            while (!test->isPrime(p, this->p_min)) {
                p += BigInt(2);
            }

            q = BigInt::random_odd_with_len(bit_len);
            while (!test->isPrime(q, this->p_min)) {
                q += BigInt(2);
            }

            BigInt diff = (p - q).abs();
            if (diff >= Service::pow(BigInt(2), BigInt(static_cast<long long>(bit_len) / 2))) {
                break;
            }
        }

        BigInt N = p * q;
        BigInt phi = (p - BigInt(1)) * (q - BigInt(1));
        BigInt e = BigInt(65537);

        BigInt d, y;
        Service::egcd(e, phi, d, y);

        if (d > (Service::root4(N) / BigInt(3))) {
            service_.private_key = PrivateKey(N, d);
            service_.public_key = PublicKey(N, e);
            break;
        }
    }

    delete test;
}

void RSA_Service::generate_rsa_keys() const {
    keys_generator.generate_keys();
}


RSA_Service::RSA_Service(KeyGenerator::PrimalityTest test, double min_prime_probability,
                         size_t bit_length) : keys_generator(*this, test, min_prime_probability, bit_length) {
}

BigInt RSA_Service::encrypt(const BigInt &m) const {
    BigInt c = Service::powmod(m, public_key.e, public_key.n);
    return c;
}

BigInt RSA_Service::decrypt(const BigInt &c) const {
    BigInt d = Service::powmod(c, private_key.d, private_key.n);
    return d;
}
