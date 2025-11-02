#include "DESKeyExtension.h"
#include "../bit_operations.h"
#include <iostream>
#include <cstring>

void DESKeyExtension::key_extension(uint8_t *init_key, size_t init_key_len, uint8_t **result, size_t rounds) {
    // Check key

    uint8_t reduced_key[4];
    permutation(init_key, 64, PC_1, 56, true, true, reduced_key);

//    uint64_t key56;
//    memcpy(key56, reduced_key, 56);
//    key56 = key56 >> 8;

    result = new uint8_t*[16];

    for (int i = 0; i < 16; i++) {
      result[i] = new uint8_t[48];

      uint8_t Ci[4];

    }
}
