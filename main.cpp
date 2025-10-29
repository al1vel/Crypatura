#include <iostream>

bool get_bit(const char *arr, const size_t pos, const size_t len, const bool isMajorFirst) {
    size_t byte_index;
    size_t bit_index;
    if (isMajorFirst) {
        byte_index = pos / 8;
        bit_index = pos % 8;
    } else {
        byte_index = len - 1 - (pos / 8);
        bit_index = 7 - (pos % 8);
    }
    char byte = arr[byte_index];
    return (byte >> bit_index) & 1;
}

void set_bit_true(char *arr, const size_t pos, const size_t len, const bool isMajorFirst) {
    size_t byte_index;
    size_t bit_index;
    if (isMajorFirst) {
        byte_index = pos / 8;
        bit_index = pos % 8;
    } else {
        byte_index = len - 1 - (pos / 8);
        bit_index = 7 - (pos % 8);
    }
    char b = 1 << bit_index;
    arr[byte_index] |= b;
}

void permutation(char *arr, const size_t len, const int *p_block, size_t p_block_len, bool isMajorFirst,bool isStartPermZero) {
    for (size_t arr_ind = 0; arr_ind < len; ++arr_ind) {
        if (get_bit(arr, arr_ind, len, isMajorFirst)) {
            int pos = p_block[arr_ind];
            if (!isStartPermZero) {
                --pos;
            }
            set_bit_true(arr, pos, len, isMajorFirst);
        }
    }
}

int main() {

    return 0;
}
