#ifndef CRYPATURA_BIT_OPERATIONS_H
#define CRYPATURA_BIT_OPERATIONS_H
#include <cstdint>


bool get_bit(const uint8_t *arr, const size_t pos, const size_t len, const bool isMajorFirst);

void set_bit_true(uint8_t *arr, const size_t pos, const size_t len, const bool isMajorFirst);

void permutation(const uint8_t *arr, uint8_t *res, const size_t len, const int *p_block, bool isMajorFirst, bool isStartPermZero);


#endif //CRYPATURA_BIT_OPERATIONS_H