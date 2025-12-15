#ifndef CRYPATURA_IPRIMALITYTEST_H
#define CRYPATURA_IPRIMALITYTEST_H

#include <gmpxx.h>

class IPrimalityTest {
    public:
    virtual bool isPrime(const mpz_class& n, double p_min) = 0;
    virtual ~IPrimalityTest() = default;
};

#endif //CRYPATURA_IPRIMALITYTEST_H