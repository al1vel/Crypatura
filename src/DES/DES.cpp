#include "../../include/DES/DES.h"

#include <cstdio>

#include "../../include/bit_operations.h"
#include "../../include/DES/DESRoundFunction.h"

DES::DES() : festel_net(FestelNet(new DESRoundFunction(), new DESKeyExtension())) {}

void DES::encrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    printf("%s\n", "DES encrypt");
    uint8_t permutated_block[8] = {0};
    printf("%s\n", "DES encrypt - perm IP");
    permutation(block, 64, IP, 64, true, false, permutated_block);
    printf("%s\n", "DES encrypt - festel");
    festel_net.do_festel_net(permutated_block, key);
    printf("%s\n", "DES encrypt - IP -1");
    permutation(permutated_block, 64, rev_IP, 64, true, false, res);
    printf("%s\n", "DES  done");
}

void DES::decrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    uint8_t permutated_block[8] = {0};
    permutation(block, 64, IP, 64, true, false, permutated_block);
    festel_net.do_festel_net_reverse(permutated_block, key);
    permutation(permutated_block, 64, rev_IP, 64, true, false, res);
}


