#include "../../include/DES/DESKeyExtension.h"
#include "../../include/bit_operations.h"
#include <iostream>
#include <cstring>

void DESKeyExtension::key_extension(uint8_t *init_key, uint8_t *result) {
    // Check key

    printf("%s\n", "Key EXT");
    uint8_t reduced_key[7] = {0, 0, 0, 0, 0, 0, 0};
    permutation(init_key, 64, PC_1, 56, true, false, reduced_key);

    printf("%s\n", "Key EXT start cycle");
    for (int i = 0; i < 16; i++) {
        uint32_t mask = (1u << 28) - 1;

        uint32_t Ci = 0;
        memcpy(&Ci, reduced_key, 4);
        Ci = Ci >> 4;

        uint32_t Di = 0;
        memcpy(&Di, reduced_key + 3, 4);
        Di = Di >> 4;

        size_t shift;
        if (i == 0 || i == 1 || i == 3 || i == 15) {
            shift = 1;
        } else {
            shift = 2;
        }

        Ci = ((Ci << shift) | (Ci >> (28 - shift))) & mask;
        Di = ((Di << shift) | (Di >> (28 - shift))) & mask;

        printf("%s\n", "Key EXT - Ci Di");

        uint64_t CiDi = 0;
        CiDi = (Ci << 28) | (Di & mask);
        uint8_t CiDiArr[7] = {0};
        auto *ptr = reinterpret_cast<uint8_t *>(&CiDi);
        memcpy(CiDiArr, ptr + 1, 7);
        printf("%s\n", "Key EXT - CiDiArr");

        permutation(CiDiArr, 56, PC_2, 48, true, false, result + (i * 6));
        printf("%s\n", "Key EXT - done");
    }
}
