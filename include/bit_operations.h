#ifndef CRYPATURA_BIT_OPERATIONS_H
#define CRYPATURA_BIT_OPERATIONS_H
#include <cstdint>
#include <cstddef>

enum class ByteOrder {
    BO_BIG_ENDIAN,
    BO_LITTLE_ENDIAN
};

enum class PBlockStartIndex {
    START_ZERO,
    START_ONE,
};

uint8_t get_bit(const uint8_t *arr, const size_t pos, const size_t len, ByteOrder byte_order);

void set_bit_true(uint8_t *arr, const size_t pos, const size_t len, ByteOrder byte_order);

void permutation(const uint8_t *arr, const size_t len, const int *p_block, const size_t p_block_len, uint8_t *res, ByteOrder byte_order = ByteOrder::BO_BIG_ENDIAN, PBlockStartIndex p_start = PBlockStartIndex::START_ONE);


#endif //CRYPATURA_BIT_OPERATIONS_H