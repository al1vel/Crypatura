#include "../../include/AES/AES_Funcs.h"
#include "../../include/AES/GF_Service.h"

void AES_Funcs::sub_bytes(uint8_t *state, size_t byte_len, const uint8_t* S_box) {
    for (int i = 0; i < static_cast<int>(byte_len); ++i) {
        state[i] = S_box[state[i]];
    }
}

void AES_Funcs::shift_rows(uint8_t *state, size_t byte_len, bool inverted) {
    auto* temp = new uint8_t[byte_len]();
    for (int p_index = 0; p_index < static_cast<int>(byte_len); ++p_index) {
        int pos = 0;
        if (byte_len == 16) {
            pos = perm16[p_index];
        } else if (byte_len == 24) {
            pos = perm24[p_index];
        } else if (byte_len == 32) {
            pos = perm32[p_index];
        }
        temp[p_index] = state[pos];
    }

    for (int i = 1; i <= 3; ++i) {
        auto* str_ptr = reinterpret_cast<uint64_t*>(temp + i * (byte_len / 4));
        int shift = i * 8;

        size_t row_bits = (byte_len / 4) * 8;
        if (inverted) {
            *str_ptr = (*str_ptr >> shift) | (*str_ptr << (row_bits - shift));
        } else {
            *str_ptr = (*str_ptr << shift) | (*str_ptr >> (row_bits - shift));
        }
    }

    for (int p_index = 0; p_index < static_cast<int>(byte_len); ++p_index) {
        int pos = 0;
        if (byte_len == 16) {
            pos = perm16[p_index];
        } else if (byte_len == 24) {
            pos = perm24[p_index];
        } else if (byte_len == 32) {
            pos = perm32[p_index];
        }
        state[p_index] = temp[pos];
    }
    delete [] temp;
}

void matr_mult_col(const uint8_t* matr, const uint8_t* col, uint8_t* res_col) {
    for (int i = 0; i < 4; ++i) {
        uint8_t cell = 0;
        for (int j = 0; j < 4; ++j) {
            uint8_t mult_res = GF_Service::mult_mod(matr[i * 4 + j], col[j], 0x11B);
            cell = GF_Service::add(cell, mult_res);
        }
        res_col[i] = cell;
    }
}

void AES_Funcs::mix_columns(uint8_t *state, size_t byte_len, bool inverted) {
    uint8_t mix_col_matr[16] = {
        2, 3, 1, 1,
        1, 2, 3, 1,
        1, 1, 2, 3,
        3, 1, 1, 2,
    };

    uint8_t inv_mix_col_matr[16] = {
        0x0e, 0x0b, 0x0d, 0x09,
        0x09, 0x0e, 0x0b, 0x0d,
        0x0d, 0x09, 0x0e, 0x0b,
        0x0b, 0x0d, 0x09, 0x0e,
    };

    for (int i = 0; i < static_cast<int>(byte_len) / 4; ++i) {
        uint8_t col[4] = {*(state + i), *(state + i + (byte_len / 4)), *(state + i + byte_len / 4 * 2), *(state + i + byte_len / 4 * 3)};
        uint8_t new_col[4] = { 0 };
        if (inverted) {
            matr_mult_col(reinterpret_cast<uint8_t*>(&inv_mix_col_matr), reinterpret_cast<const uint8_t *>(&col), reinterpret_cast<uint8_t *>(&new_col));
        }
        else {
            matr_mult_col(reinterpret_cast<uint8_t*>(&mix_col_matr), reinterpret_cast<const uint8_t *>(&col), reinterpret_cast<uint8_t *>(&new_col));
        }
        for (int j = 0; j < 4; ++j) {
            state[i + j * byte_len / 4] = new_col[j];
        }
    }
}

void AES_Funcs::add_round_key(uint8_t *state, size_t byte_len, const uint8_t *r_key) {
    for (int i = 0; i < static_cast<int>(byte_len); ++i) {
        state[i] ^= r_key[i];
    }
}
