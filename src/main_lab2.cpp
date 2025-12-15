#include <iostream>
#include <ostream>

#include "../include/RSA/RSA_Service.h"

#include <gmpxx.h>

int main() {
    RSA_Service rsa(RSA_Service::KeyGenerator::PrimalityTest::MillerRabin, 0.99, 4096);
    std::cout << "RSA created" << std::endl;
    rsa.generate_rsa_keys();
    std::cout << "Keys created" << std::endl;

    mpz_class m("123456789123456789123456789");
    mpz_class c = rsa.encrypt(m);
    std::cout << "Encrypted message: " << c << std::endl;
    mpz_class d = rsa.decrypt(c);
    std::cout << "Decrypted message: " << d << std::endl;
}