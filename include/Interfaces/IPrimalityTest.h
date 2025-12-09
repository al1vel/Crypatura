#ifndef CRYPATURA_IPRIMALITYTEST_H
#define CRYPATURA_IPRIMALITYTEST_H

using

class IPrimalityTest {
    public:
    virtual bool isPrime(const BigInt& n, double p_min) = 0;
    virtual ~IPrimalityTest() = default;
};

#endif //CRYPATURA_IPRIMALITYTEST_H