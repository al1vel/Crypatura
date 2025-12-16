#ifndef CRYPATURA_WIENERATTACK_H
#define CRYPATURA_WIENERATTACK_H

#include <gmpxx.h>


class WienerAttack {
private:
    mpz_class N;
    mpz_class e;
    mpz_class d;

public:
    struct frac {
        mpz_class a;
        mpz_class b;
    };

    static std::vector<mpz_class> decompose_to_chained_fraction(mpz_class a, mpz_class b);

    frac get_frac_part(size_t i);

    WienerAttack(mpz_class N, mpz_class e);

    void crack_key();
};


#endif //CRYPATURA_WIENERATTACK_H
