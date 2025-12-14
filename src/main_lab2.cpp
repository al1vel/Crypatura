#include <iostream>
#include <ostream>

#include "RSA/FermatTest.h"
#include "RSA/MilRabTest.h"
#include "RSA/Service.h"
#include "RSA/SolStrTest.h"

#include "../include/RSA/RSA_Service.h"

int main() {
    // BigInt v = Service::Jacobi_val(BigInt(5), BigInt(21));
    // std::cout << v << std::endl;

    // BigInt a("974634777231");
    // SolStrTest t;
    // bool b = t.isPrime(a, 0.999);
    // std::cout << b << std::endl;

    RSA_Service rsa(RSA_Service::KeyGenerator::PrimalityTest::Fermat, 0.9, 256);
    std::cout << "RSA created" << std::endl;
    rsa.generate_rsa_keys();
    std::cout << "Keys created" << std::endl;

    BigInt m("123456789123456789123456789");
    BigInt c = rsa.encrypt(m);
    std::cout << "Encrypted message: " << c << std::endl;
    BigInt d = rsa.decrypt(c);
    std::cout << d << std::endl;
}