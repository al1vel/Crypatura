#include "../../include/DES/DESKeyExtension.h"
#include "../../include/bit_operations.h"
#include <iostream>
#include <cstring>

void DESKeyExtension::key_extension(uint8_t *init_key, size_t init_key_len, uint8_t *result) {
    uint8_t CiArr[4] = {0};
    uint8_t DiArr[4] = {0};
    permutation(init_key, init_key_len, PC_1[0], 28, CiArr);
    permutation(init_key, init_key_len, PC_1[1], 28, DiArr);

    uint32_t Ci = *(reinterpret_cast<uint32_t*>(CiArr));
    uint32_t Di = *(reinterpret_cast<uint32_t*>(DiArr));

    uint32_t mask = (1u << 28) - 1;

    for (int i = 0; i < 16; i++) {
        size_t shift;
        if (i == 0 || i == 1 || i == 8 || i == 15) {
            shift = 1;
        } else {
            shift = 2;
        }

        Ci = ((Ci << shift) | (Ci >> (28 - shift))) & mask;
        Di = ((Di << shift) | (Di >> (28 - shift))) & mask;

        uint64_t CiDi = 0;
        uint64_t CiShift = Ci;
        CiShift = CiShift << 28;
        CiDi = CiShift | (Di & mask);

        uint8_t* CiDiArr = reinterpret_cast<uint8_t*>(&CiDi);

        permutation(CiDiArr, 56, PC_2, 48, result + (i * 6));
    }
}
