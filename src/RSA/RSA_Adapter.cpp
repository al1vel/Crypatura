#include "RSA_Adapter.h"

RSA_Adapter::RSA_Adapter(RSA_Service::KeyGenerator::PrimalityTest test, double min_prime_prob, size_t bit_length) {
    rsa_service = new RSA_Service(test, min_prime_prob, bit_length);
}

RSA_Adapter::~RSA_Adapter() {
    delete rsa_service;
}

mpz_class bytes_to_mpz(uint8_t* bytes) {
    mpz_class result;
    mpz_import(
        result.get_mpz_t(),
        32,
        1,   // big-endian
        1,
        0,
        0,
        bytes
    );
    return result;
}

void mpz_to_bytes(const mpz_class& num, uint8_t* out) {
    std::memset(out, 0, 32);
    const size_t needed = (mpz_sizeinbase(num.get_mpz_t(), 2) + 7) / 8;
    size_t written = 0;
    mpz_export(
        out + (32 - needed),
        &written,
        1,                 // big-endian
        1,                 // 1 байт на элемент
        0,
        0,
        num.get_mpz_t()
    );
}
