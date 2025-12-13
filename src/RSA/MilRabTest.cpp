#include "../../include/RSA/MilRabTest.h"
#include "../../include/RSA/Service.h"

double MilRabTest::one_iter_fail_probability() const {
    return 0.25;
}

bool MilRabTest::test_iteration(const BigInt &n) {
    if (n >= BigInt(0) && n <= BigInt(3)) {
        return true;
    }
    if (n % BigInt(2) == BigInt(0)) {
        return false;
    }

    BigInt a = BigInt(0);
    a.random(BigInt(2), n - BigInt(2));
    //std::cout << "Random done\n";

    BigInt gcd = Service::gcd(a, n);
    //std::cout << "Gcd done\n";
    if (gcd > BigInt(1)) {
        return false;
    }

    BigInt d = n - BigInt(1);
    BigInt s(0);
    while (d.isEven()) {
        d /= BigInt(2);
        ++s;
    }

    BigInt x = Service::powmod(a, d, n);
    if (x == BigInt(1) || x == n - BigInt(1)) {
        return true;
    }

    for (BigInt i(0); i < s - BigInt(1); ++i) {
        x = Service::powmod(x, BigInt(2), n);
        if (x == n - BigInt(1)) {
            return true;
        }
    }
    return false;
}
