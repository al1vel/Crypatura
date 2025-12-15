#ifndef CRYPATURA_MILRABTEST_H
#define CRYPATURA_MILRABTEST_H

#include "PrimeTest.h"

class MilRabTest: public PrimeTest {
    double one_iter_fail_probability() const override;
    bool test_iteration(const mpz_class &n) override;
};


#endif //CRYPATURA_MILRABTEST_H