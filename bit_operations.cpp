#include "bit_operations.h"
#include <cstdint>
#include <iostream>

uint8_t get_bit(const uint8_t *arr, const size_t pos, const size_t len, const bool isMajorFirst) {
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

void permutation(const uint8_t *arr, uint8_t *res, const size_t len, const int *p_block, const size_t p_block_len, bool isMajorFirst, bool isStartPermZero) {

    for (size_t p_index = 0; p_index < len; ++p_index) {
        // if (get_bit(arr, arr_ind, len, isMajorFirst)) {
        //     int pos = p_block[arr_ind];
        //     if (!isStartPermZero) {
        //         --pos;
        //     }
        //     set_bit_true(res, pos, len, isMajorFirst);
        // }

        int pos = p_block[p_index];
        if (!isStartPermZero) {
            --pos;
        }
        if (get_bit(arr, pos, len, isMajorFirst)) {
            set_bit_true(res, p_index, p_block_len, isMajorFirst);
        }
    }
}

void left_loop_shift(uint8_t* arr, size_t shift, size_t valueble_bits) {
    if (shift != 0) {
    size_t bytes_cnt = valueble_bits / 8;
    if (valueble_bits % 8 != 0) {
      bytes_cnt += 1;
    }

    uint64_t value = 0;
    memcpy(&value, arr, bytes_cnt);

    uint32_t mask = (1ULL << valueble_bits) - 1;
    value &= mask;
    value = ((value << shift) | (value >> (valueble_bits - shift))) & mask;

    memcpy(arr, &value, bytes_cnt);
    }
}