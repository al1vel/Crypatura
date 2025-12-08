#ifndef CRYPATURA_SERVICE_H
#define CRYPATURA_SERVICE_H

using ll = long long int;

class Service {
    public:
    static ll Legengre_val(ll a, ll p);
    static ll Jacobi_val(ll a, ll n);
    static ll gcd(ll a, ll b);
    static ll egcd(ll a, ll b, ll &x, ll &y);
    static ll powmod(ll a, ll b, ll mod);
};


#endif //CRYPATURA_SERVICE_H