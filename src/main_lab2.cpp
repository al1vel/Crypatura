#include <iostream>
#include <ostream>

#include "RSA/Service.h"
using ll = long long int;

int main() {
    Service s;
    ll v = s.Jacobi_val(100, 383);
    std::cout << v << std::endl;
}