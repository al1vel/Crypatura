#ifndef CRYPATURA_BIT_OPERATIONS_H
#define CRYPATURA_BIT_OPERATIONS_H
#include <cstdint>


uint8_t get_bit(const uint8_t *arr, const size_t pos, const size_t len, const bool isMajorFirst);

void set_bit_true(uint8_t *arr, const size_t pos, const size_t len, const bool isMajorFirst);

void permutation(const uint8_t *arr, const size_t len, const int *p_block, const size_t p_block_len, bool isStraight, bool isStartPermZero, uint8_t *res);


#endif //CRYPATURA_BIT_OPERATIONS_H