#ifndef CRYPATURA_FERMATTEST_H
#define CRYPATURA_FERMATTEST_H

#include "PrimeTest.h"

class FermatTest : public PrimeTest {
protected:
    double one_iter_fail_probability() const override;
    bool test_iteration(const BigInt &n) override;
};


#endif //CRYPATURA_FERMATTEST_H
