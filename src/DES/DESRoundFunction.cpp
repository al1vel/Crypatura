#include "../../include/DES/DESRoundFunction.h"
#include "../../include/bit_operations.h"

void DESRoundFunction::do_round_func(uint8_t *arr, uint8_t *r_key, uint8_t *res) {
    uint8_t extended_R[6] = {0};
    permutation(arr, 32, E_table, 48, true, true, extended_R);

    for (int i = 0; i < 4; i++) {
        extended_R[i] = extended_R[i] ^ r_key[i];
    }

    uint8_t reduced_R[4] = {0};
    int iter = 0;
    for (int i = 0; i < 48; i += 6) {
        int col_num = get_bit(extended_R, i + 1, 48, true) << 3 |
                      get_bit(extended_R, i + 2, 48, true) << 2 |
                      get_bit(extended_R, i + 3, 48, true) << 1 |
                      get_bit(extended_R, i + 4, 48, true) << 0;

        int row_num = get_bit(extended_R, i + 0, 48, true) << 1 |
                      get_bit(extended_R, i + 5, 48, true) << 0;

        int index = iter * 64 + row_num * 16 + col_num;
        uint8_t reduced4bits = S_table[index];
        if (iter % 2 == 1) {
            reduced4bits = reduced4bits << 4;
        }
        reduced_R[iter / 2] |= reduced4bits;

        ++iter;
    }

    permutation(reduced_R, 32, P_table, 32, true, true, res);
}
