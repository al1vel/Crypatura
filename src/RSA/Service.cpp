#include "../../include/RSA/Service.h"

#include <utility>

BigInt Service::powmod(const BigInt& a, BigInt b, const BigInt& mod) {
    BigInt base = a % mod;
    BigInt result = BigInt(1);

    while (b > BigInt(0)) {
        if (b.isOdd()) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        b /= BigInt(2);
    }

    return result;
}

BigInt Service::pow(const BigInt& a, BigInt b) {
    BigInt base = a;
    BigInt result = BigInt(1);

    while (b > BigInt(0)) {
        if (b.isOdd()) {
            result *= base;
        }
        base *= base;
        b /= BigInt(2);
    }
    return result;
}
BigInt Service::Legengre_val(const BigInt& a, const BigInt& p) {
    BigInt t = powmod(a, (p - BigInt(1)) / BigInt(2), p);
    if (t == BigInt(0)) {
        return BigInt(0);
    }
    if (t == BigInt(1)) {
        return BigInt(1);
    }
    if (t == BigInt(p - BigInt(1))) {
        return BigInt(-1);
    }
    return BigInt(-2);
}

BigInt Service::Jacobi_val(BigInt a, const BigInt& n) {
    if (n <= BigInt(0) || (n % BigInt(2)) == BigInt(0)) return BigInt(0);
    if (n == BigInt(1)) return BigInt(1);

    a = a % n;
    if (a == BigInt(0)) return BigInt(0);
    if (a == BigInt(1)) return BigInt(1);

    BigInt result = BigInt(1);

    if (a < BigInt(0)) {
        a = a * BigInt(-1);
        if (n % BigInt(4) == BigInt(3)) {
            result *= BigInt(-1);
        }
    }

    BigInt t = BigInt(0);
    while (a.isEven()) {
        a /= BigInt(2);
        ++t;
    }
    if (t > BigInt(0)) {
        if ((n % BigInt(8) == BigInt(3) || n % BigInt(8) == BigInt(5)) && (t % BigInt(2) == BigInt(1))) {
            result *= BigInt(-1);
        }
    }

    if (a % BigInt(4) == BigInt(3) && n % BigInt(4) == BigInt(3)) {
        result *= BigInt(-1);
    }

    if (a == BigInt(1)) return result;
    return result * Jacobi_val(n % a, a);
}

BigInt Service::gcd(BigInt a, BigInt b) {
    while (a > BigInt(0)) {
        if (a <= b) {
            b = b % a;
        }
        std::swap(a ,b);
    }
    return b;
}

BigInt Service::egcd(const BigInt& a, BigInt b, BigInt &x, BigInt &y) {
    if (a == BigInt(0)) {
        x = BigInt(0);
        y = BigInt(1);
        return b;
    }
    BigInt x1, y1;
    BigInt g = egcd(b % a, a, x1, y1);
    x = y1 - (b % a) * x1;
    y = x1;
    return g;
}

BigInt Service::root4(const BigInt &N) {
    if (N < BigInt(0)) throw std::runtime_error("корень четвёртой степени из отрицательного числа");
    if (N == BigInt(0) || N == BigInt(1)) return N;

    BigInt left(0);
    BigInt right = N;
    BigInt result;

    while (left <= right) {
        BigInt mid = (left + right) / BigInt(2);
        BigInt mid4 = pow(mid, BigInt(4));

        if (mid4 == N) {
            return mid;
        }

        if (mid4 < N) {
            result = mid;
            left = mid + BigInt(1);
        } else {
            right = mid - BigInt(1);
        }
    }
    return result;
}
