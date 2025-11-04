#include "../include/bit_operations.h"
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

void permutation(const uint8_t *arr, const size_t len, const int *p_block, const size_t p_block_len, bool isMajorFirst, bool isStartPermZero, uint8_t *res) {
    for (size_t p_index = 0; p_index < len; ++p_index) {
        int pos = p_block[p_index];
        if (!isStartPermZero) {
            --pos;
        }
        if (get_bit(arr, pos, len, isMajorFirst)) {
            set_bit_true(res, p_index, p_block_len, isMajorFirst);
        }
    }
}
