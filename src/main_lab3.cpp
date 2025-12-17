#include <iostream>

#include "../include/AES/GF_Service.h"

int main() {
    std::cout << GF_Service::is_prime(0x11B) << std::endl;
    std::vector<uint16_t> primes = GF_Service::get_primes();
    for (uint16_t f : primes) {
        std::cout << std::showbase << std::hex << f << std::endl;
    }
    std::vector<uint32_t> pr = GF_Service::decompose_to_primes(0xF4);
    for (uint32_t f : pr) {
        std::cout << std::showbase << std::hex << f << std::endl;
    }
}