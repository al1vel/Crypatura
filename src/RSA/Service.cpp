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
    a = a % n;
    if (a == 0) {
        return 0;
    }
    if (a == 1) {
        return 1;
    }
    if (a == -1) {
        return -1;
    }

    if (a % 2 == 0) {
        ll two_k = a & (!a + 1);
        ll k = two_k / 2;
        ll sign = 1;
        if (n % 8 == 3 || n % 8 == 5) {
            if (k % 2 == 1) {
                sign = -1;
            }
        }
        a = (a / two_k) * sign;
    }
    else {
        ll sign = 1;
        if (((a - 1) * (n - 1) / 4) % 2 == 1) {
            sign = -1;
        }
        ll tmp_a = a;
        a = (n % a) * sign;
        n = tmp_a;
    }
    return Jacobi_val(a, n);
}
