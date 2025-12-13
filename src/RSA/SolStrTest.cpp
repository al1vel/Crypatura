#include "../../include/RSA/SolStrTest.h"
#include "../../include/RSA/Service.h"

double SolStrTest::one_iter_fail_probability() const {
    return 0.5;
}

bool SolStrTest::test_iteration(const BigInt &n) {
    if (n >= BigInt(0) && n <= BigInt(3)) {
        return true;
    }
    if (n % BigInt(2) == BigInt(0)) {
        return false;
    }

    BigInt a = BigInt(0);
    a.random(BigInt(1), n - BigInt(1));
    //std::cout << "Random: " << a << std::endl;

    BigInt gcd = Service::gcd(a, n);
    //std::cout << "Gcd done: " << gcd << std::endl;
    if (gcd > BigInt(1)) {
        return false;
    }

    BigInt J = Service::Jacobi_val(a, n);
    BigInt x = Service::powmod(a, (n - BigInt(1)) / BigInt(2), n);
    if (J == BigInt(-1)) {
        J = n - BigInt(1);
    }
    //std::cout << "Jacobi: " << J << std::endl;
    //std::cout << "x: " << x << std::endl;
    if (x != J) {
        return false;
    }
    return true;
}