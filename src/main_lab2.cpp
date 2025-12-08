#include <iostream>
#include <ostream>

#include "RSA/Service.h"
using ll = long long int;

int main() {
    ll v = Service::Jacobi_val(5, 21);
    std::cout << v << std::endl;
}