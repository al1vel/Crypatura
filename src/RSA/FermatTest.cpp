#include "../../include/RSA/FermatTest.h"
#include "../../include/RSA/Service.h"

double FermatTest::one_iter_fail_probability() const {
    return 0.5;
}

bool FermatTest::test_iteration(const BigInt &n) {
    //std::cout << "Iter started, n = " << n << std::endl;
    if (n >= BigInt(0) && n <= BigInt(3)) {
        return true;
    }
    if (n % BigInt(2) == BigInt(0)) {
        return false;
    }

    BigInt a = BigInt(0);
    a.random(BigInt(1), n - BigInt(1));
    //std::cout << "Random a: " << a << std::endl;

    BigInt gcd = Service::gcd(a, n);
    //std::cout << "Gcd: " << gcd << std::endl;
    if (gcd > BigInt(1)) {
        return false;
    }


    BigInt r = Service::powmod(a, n - BigInt(1), n);
    //std::cout << "Powmode: " << r << std::endl;
    if (r != BigInt(1)) {
        return false;
    }
    return true;
}
