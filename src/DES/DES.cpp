#include "../../include/DES/DES.h"
#include "../../include/bit_operations.h"
#include "../../include/DES/DESRoundFunction.h"

DES::DES() : festel_net(FestelNet(new DESRoundFunction(), new DESKeyExtension())) {}

void DES::encrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    uint8_t permutated_block[8] = {0};
    permutation(block, 64, IP, 64, permutated_block);
    festel_net.do_festel_net(permutated_block, key, 64, 16, 6, 8);
    permutation(permutated_block, 64, rev_IP, 64, res);
}

void DES::decrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    uint8_t permutated_block[8] = {0};
    permutation(block, 64, IP, 64, permutated_block);
    festel_net.do_festel_net_reverse(permutated_block, key, 64, 16, 6, 8);
    permutation(permutated_block, 64, rev_IP, 64, res);
}
