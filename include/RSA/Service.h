#ifndef CRYPATURA_SERVICE_H
#define CRYPATURA_SERVICE_H

#include "../big_int.h"

class Service {
public:
    static BigInt Legengre_val(const BigInt& a, const BigInt& p);

    static BigInt Jacobi_val(BigInt a, const BigInt& n);

    static BigInt gcd(BigInt a, BigInt b);

    static BigInt egcd(const BigInt& a, BigInt b, BigInt &x, BigInt &y);

    static BigInt powmod(const BigInt& a, BigInt b, const BigInt& mod);

    static BigInt pow(const BigInt& a, BigInt b);

    static BigInt root4(const BigInt& N);
};


#endif //CRYPATURA_SERVICE_H
