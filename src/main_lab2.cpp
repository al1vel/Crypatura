#include <iostream>
#include <ostream>

#include "../include/RSA/RSA_Service.h"

#include <gmpxx.h>

using PT = RSA_Service::KeyGenerator::PrimalityTest;

int main() {
    RSA_Service rsa(PT::Fermat, 0.99, 1024);
    std::cout << "RSA created" << std::endl;
    rsa.generate_rsa_keys();

    mpz_class m("564123456789123456789123456789");
    mpz_class c = rsa.encrypt(m);
    std::cout << "Encrypted message: " << c << std::endl;
    mpz_class d = rsa.decrypt(c);
    std::cout << "Decrypted message: " << d << std::endl;

    RSA_Service weak_rsa(PT::Fermat, 0.99, 1024);
    weak_rsa.generate_weak_rsa_keys();
}