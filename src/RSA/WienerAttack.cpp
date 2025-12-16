#include <utility>

#include "../../include/RSA/WienerAttack.h"

std::vector<mpz_class> WienerAttack::decompose_to_chained_fraction(mpz_class a, mpz_class b) {
    std::vector<mpz_class> result{};
    while (b != 1) {
        mpz_class complete = a / b;
        result.push_back(complete);
        mpz_class left = a - b * complete;
        a = b;
        b = left;
    }
    result.push_back(a);
    return result;
}

WienerAttack::frac get_frac_part(size_t i) {
    WienerAttack::frac frac{};

}

WienerAttack::WienerAttack(mpz_class N, mpz_class e) : N(std::move(N)), e(std::move(e)) {}

void WienerAttack::crack_key() {

}

