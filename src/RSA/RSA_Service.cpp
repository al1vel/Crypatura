#include "../../include/RSA/RSA_Service.h"
#include "../../include/Interfaces/IPrimalityTest.h"
#include "../../include/RSA/PrimeTest.h"
#include "../../include/RSA/FermatTest.h"
#include "../../include/RSA/SolStrTest.h"
#include "../../include/RSA/MilRabTest.h"
#include "../../include/RSA/Service.h"

#include <iostream>

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

RSA_Service::KeyGenerator::KeyGenerator(RSA_Service &parent, PrimalityTest p_test, double p_min, size_t bit_len) : service_(parent) {
    this->p_test = p_test;
    this->p_min = p_min;
    this->bit_len = bit_len;
}

void RSA_Service::KeyGenerator::generate_keys() const {
    std::cout << "\n-------------------------------------------------------------\nGenerating keys..." << std::endl;
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
        mpz_class p, q;
        while (true) {
            do {
                p = Service::random_with_len(bit_len);
                //std::cout << "p: " << p << std::endl;
            } while (!test->isPrime(p, p_min));
            std::cout << "P generated." << std::endl;

            do {
                q = Service::random_with_len(bit_len);
                //std::cout << "q: " << q << std::endl;
            } while (!test->isPrime(q, p_min));
            std::cout << "Q generated." << std::endl;

            mpz_class diff = (p - q);
            if (diff < 0) diff = -diff;
            if (diff >= Service::pow(2, bit_len / 2 - 10)) {
                std::cout << "P & Q generated. Fermat attack denied.\nP: " << p << "\nQ: " << q << std::endl;
                break;
            }
            std::cout << "P & Q can be affected by Fermat attack. Trying again..." << std::endl;
        }

        mpz_class N = p * q;
        mpz_class phi = (p - 1) * (q - 1);
        mpz_class e = 65537;

        mpz_class d, y;
        mpz_class gcd = Service::egcd(e, phi, d, y);
        if (gcd != 1) {
            std::cout << "GCD of e and phi is not 1. Trying again..." << std::endl;
            continue;
        }
        std::cout << "GCD: " << gcd << "\nGood!" << std::endl;

        if (d < 0) {
            d = (d % phi + phi) % phi;
        } else {
            d %= phi;
        }

        if (d > (Service::root4(N) / 3)) {
            service_.private_key = PrivateKey(N, d);
            service_.public_key = PublicKey(N, e);
            std::cout << "d is ok - keys generated.\n-------------------------------------------------------------\n" << std::endl;
            break;
        }
        std::cout << "d can be affected by Wiener attack. Trying again..." << std::endl;
    }

    delete test;
}

void RSA_Service::KeyGenerator::generate_weak_keys() const {
    std::cout << "\n-------------------------------------------------------------\nGenerating weak keys..." << std::endl;
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

    mpz_class p, q;
    while (true) {
        do {
            p = Service::random_with_len(bit_len);
        } while (!test->isPrime(p, p_min));
        std::cout << "P generated." << std::endl;

        do {
            q = Service::random_with_len(bit_len);
        } while (!test->isPrime(q, p_min));
        std::cout << "Q generated." << std::endl;

        mpz_class N = p * q;
        mpz_class phi = (p - 1) * (q - 1);
        mpz_class border = Service::root4(N) / 3;

        mpz_class e, d, y;
        do {
            d = Service::random_in_range(1, border);
        } while (Service::gcd(d, phi) != 1);

        mpz_class gcd = Service::egcd(d, phi, e, y);

        if (gcd != 1) {
            std::cout << "GCD of d and phi is not 1. Trying again..." << std::endl;
        }
        else {
            if (e < 0) {
                e = (e % phi + phi) % phi;
            } else {
                e %= phi;
            }
            std::cout << "GCD: " << gcd << "\nd: " << d << "\nfound e: " << e << std::endl;
            service_.private_key = PrivateKey(N, d);
            service_.public_key = PublicKey(N, e);
            std::cout << "Weak keys generated.\n-------------------------------------------------------------\n" << std::endl;
            break;
        }
    }
}

void RSA_Service::generate_rsa_keys() const {
    keys_generator.generate_keys();
}

void RSA_Service::generate_weak_rsa_keys() const {
    keys_generator.generate_weak_keys();
}

RSA_Service::RSA_Service(KeyGenerator::PrimalityTest test, double min_prime_probability,
                         size_t bit_length) : keys_generator(*this, test, min_prime_probability, bit_length) {
}

mpz_class RSA_Service::encrypt(const mpz_class &m) const {
    mpz_class c = Service::powmod(m, public_key.e, public_key.n);
    return c;
}

mpz_class RSA_Service::decrypt(const mpz_class &c) const {
    mpz_class d = Service::powmod(c, private_key.d, private_key.n);
    return d;
}
