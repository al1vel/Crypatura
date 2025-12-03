#ifndef CRYPATURA_SERVICE_H
#define CRYPATURA_SERVICE_H

using ll = long long int;

class Service {
    public:
    ll Legengre_val(ll a, ll p);
    ll Jacobi_val(ll a, ll n);
    ll gcd(ll a, ll b);
    ll egcd(ll a, ll b, ll &x, ll &y);
    ll powmod(ll a, ll b, ll mod);
};


#endif //CRYPATURA_SERVICE_H