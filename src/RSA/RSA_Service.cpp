#include "../../include/RSA/RSA_Service.h"

#include "../../include/Interfaces/IPrimalityTest.h"
#include "../../include/RSA/PrimeTest.h"
#include "../../include/RSA/FermatTest.h"
#include "../../include/RSA/SolStrTest.h"
#include "../../include/RSA/MilRabTest.h"
#include "../../include/RSA/Service.h"

std::string name_test(RSA_Service::KeyGenerator::PrimalityTest test) {
    switch (test) {
        case RSA_Service::KeyGenerator::PrimalityTest::Fermat: {
            return "Fermat";
        }
        case RSA_Service::KeyGenerator::PrimalityTest::MillerRabin: {
            return "MillerRabin";
        }
        case RSA_Service::KeyGenerator::PrimalityTest::SoloveyStrassen: {
            return "SoloveyStrassen";
        }
        default:
            return "";
    }
}

RSA_Service::KeyGenerator::KeyGenerator(RSA_Service &parent, PrimalityTest p_test, double p_min,
                                        size_t bit_len) : service_(parent) {
    this->p_test = p_test;
    this->p_min = p_min;
    this->bit_len = bit_len;
}

void RSA_Service::KeyGenerator::generate_keys() const {
    std::cout << "Generating keys..." << std::endl;
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
    std::cout << "Test " + name_test(this->p_test) + " created." << std::endl;

    while (true) {
        BigInt p, q;
        while (true) {
            do {
                p = BigInt::random_odd_with_len(bit_len);
                std::cout << "p: " << p << std::endl;
            } while (!test->isPrime(p, p_min));

            do {
                q = BigInt::random_odd_with_len(bit_len);
                std::cout << "q: " << q << std::endl;
            } while (!test->isPrime(q, p_min));

            BigInt diff = (p - q).abs();
            if (diff >= Service::pow(BigInt(2), BigInt(static_cast<long long>(bit_len) / 2) - BigInt(10))) {
                break;
            }
            std::cout << "P & Q generated. Fermat attack denied.\nP: " << p << "\nQ: " << q << std::endl;
        }

        BigInt N = p * q;
        BigInt phi = (p - BigInt(1)) * (q - BigInt(1));
        BigInt e = BigInt(65537);

        BigInt d, y;
        BigInt gcd = Service::egcd(e, phi, d, y);
        if (gcd != BigInt(1)) {
            continue;
        }
        std::cout << "GCD: " << gcd << "\nd: " << d << std::endl;

        if (d > (Service::root4(bit_len) / BigInt(3))) {
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
