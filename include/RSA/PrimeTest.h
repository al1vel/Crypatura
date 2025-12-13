#ifndef CRYPATURA_PRIMETEST_H
#define CRYPATURA_PRIMETEST_H

#include "../Interfaces/IPrimalityTest.h"
#include "../big_int.h"

class PrimeTest : IPrimalityTest {
private:
    [[nodiscard]] int calculate_iter_cnt(double p_min) const;

protected:
    virtual bool test_iteration(const BigInt &n) = 0;

    [[nodiscard]] virtual double one_iter_fail_probability() const = 0;

public:
    bool isPrime(const BigInt &n, double p_min) final;
};


#endif //CRYPATURA_PRIMETEST_H
