#include "bit_operations.h"

bool get_bit(const uint8_t *arr, const size_t pos, const size_t len, const bool isMajorFirst) {
    size_t byte_index;
    size_t bit_index;
    if (isMajorFirst) {
        byte_index = pos / 8;
        bit_index = pos % 8;
    } else {
        byte_index = len - 1 - (pos / 8);
        bit_index = 7 - (pos % 8);
    }
    uint8_t byte = arr[byte_index];
    return (byte >> bit_index) & 1;
}

void set_bit_true(uint8_t *arr, const size_t pos, const size_t len, const bool isMajorFirst) {
    size_t byte_index;
    size_t bit_index;
    if (isMajorFirst) {
        byte_index = pos / 8;
        bit_index = pos % 8;
    } else {
        byte_index = len - 1 - (pos / 8);
        bit_index = 7 - (pos % 8);
    }
    uint8_t b = 1 << bit_index;
    arr[byte_index] |= b;
}

void permutation(const uint8_t *arr, uint8_t *res, const size_t len, const int *p_block, bool isMajorFirst, bool isStartPermZero) {
    for (size_t arr_ind = 0; arr_ind < len; ++arr_ind) {
        if (get_bit(arr, arr_ind, len, isMajorFirst)) {
            int pos = p_block[arr_ind];
            if (!isStartPermZero) {
                --pos;
            }
            set_bit_true(res, pos, len, isMajorFirst);
        }
    }
}