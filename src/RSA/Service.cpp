#include "../../include/RSA/Service.h"

ll Service::Legengre_val(ll a, ll p) {
    ll t = Service::powmod(a, (p - 1) / 2, p);
    if (t == 0) {
        return 0;
    }
    if (t == 1) {
        return 1;
    }
    if (t == p - 1) {
        return -1;
    }
    return -2;
}

ll Service::powmod(long long a, long long b, long long mod) {
    a %= mod;
    long long result = 1;
    while (b > 0) {
        if (b & 1) {
            result = (result * a) % mod;
        }
        a = (a * a) % mod;
        b >>= 1;
    }
    return result;
}

ll Service::Jacobi_val(ll a, ll n) {
    if (n <= 0 || (n % 2) == 0) return 0;
    if (n == 1) return 1;

    a %= n;
    if (a == 0) return 0;
    if (a == 1) return 1;

    ll result = 1;

    if (a < 0) {
        a = -a;
        if (n % 4 == 3) result = -result;
    }

    // подсчёт t (количества делений на 2) без fancy-билтинов
    int t = 0;
    while ((a & 1) == 0) {
        a >>= 1;
        ++t;
    }
    if (t > 0) {
        if ((n % 8 == 3 || n % 8 == 5) && (t % 2 == 1))
            result = -result;
    }

    if (a % 4 == 3 && n % 4 == 3) result = -result;

    if (a == 1) return result;
    return result * Jacobi_val(n % a, a);
}

