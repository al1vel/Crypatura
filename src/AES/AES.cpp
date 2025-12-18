#include <iostream>
#include <cstring>

#include "../include/AES/AESKeyExtension.h"
#include "../include/AES/AES.h"
#include "../include/AES/AES_Funcs.h"


AES::AES(size_t block_len, size_t key_len, uint8_t* init_key_ptr) : key_len(key_len), block_len(block_len) {
    init_key = new uint8_t[key_len]();
    memcpy(init_key, init_key_ptr, key_len);

    uint16_t mod = 0x11B;
    for (int iter = 0; iter < 256; ++iter) {
        auto i = static_cast<uint8_t>(iter);
        uint8_t y = GF_Service::inverse_mod(i, mod);
        uint8_t y0 = (y >> 0) & 1;
        uint8_t y1 = (y >> 1) & 1;
        uint8_t y2 = (y >> 2) & 1;
        uint8_t y3 = (y >> 3) & 1;
        uint8_t y4 = (y >> 4) & 1;
        uint8_t y5 = (y >> 5) & 1;
        uint8_t y6 = (y >> 6) & 1;
        uint8_t y7 = (y >> 7) & 1;

        uint8_t z0 = y0 ^ y4 ^ y5 ^ y6 ^ y7 ^ 1;
        uint8_t z1 = y0 ^ y1 ^ y5 ^ y6 ^ y7 ^ 1;
        uint8_t z2 = y0 ^ y1 ^ y2 ^ y6 ^ y7 ^ 0;
        uint8_t z3 = y0 ^ y1 ^ y2 ^ y3 ^ y7 ^ 0;
        uint8_t z4 = y0 ^ y1 ^ y2 ^ y3 ^ y4 ^ 0;
        uint8_t z5 = y1 ^ y2 ^ y3 ^ y4 ^ y5 ^ 1;
        uint8_t z6 = y2 ^ y3 ^ y4 ^ y5 ^ y6 ^ 1;
        uint8_t z7 = y3 ^ y4 ^ y5 ^ y6 ^ y7 ^ 0;

        this->S_box[i] = z0 | (z1 << 1) | (z2 << 2) | (z3 << 3) | (z4 << 4) | (z5 << 5) | (z6 << 6) | (z7 << 7);
    }

    for (int i = 0; i < 256; ++i) {
        S_box_inv[S_box[i]] = i;
    }

    int rounds_cnt = key_len == 16 ? 10 : (key_len == 192 ? 12 : 14);
    ++rounds_cnt;
    size_t exp_key_size = rounds_cnt * block_len;
    exp_key = new uint8_t[exp_key_size]();

    AESKeyExtension key_extenser(block_len, reinterpret_cast<uint8_t*>(&S_box));
    key_extenser.key_extension(init_key, key_len, exp_key);
}

AES::~AES() {
    delete[] exp_key;
    delete[] init_key;
}

void AES::encrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    uint8_t state[32] = { 0 };
    memcpy(state, block, block_len);

    AES_Funcs::add_round_key(state, block_len, key);

    int rounds_cnt = key_len == 16 ? 10 : (key_len == 192 ? 12 : 14);
    for (int i = 0; i < rounds_cnt; ++i) {
        AES_Funcs::sub_bytes(state, block_len, reinterpret_cast<uint8_t*>(&S_box));
        AES_Funcs::shift_rows(state, block_len, false);
        if (i != rounds_cnt - 1) {
            AES_Funcs::mix_columns(state, block_len, false);
        }
        AES_Funcs::add_round_key(state, block_len, key + block_len * (i + 1));
    }

    memcpy(res, state, block_len);
}

void AES::decrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    uint8_t state[32] = { 0 };
    memcpy(state, block, block_len);

    int rounds_cnt = key_len == 16 ? 10 : (key_len == 192 ? 12 : 14);
    ++rounds_cnt;
    size_t exp_key_size = rounds_cnt * block_len;
    --rounds_cnt;

    AES_Funcs::add_round_key(state, block_len, key + exp_key_size - block_len);

    for (int i = 0; i < rounds_cnt; ++i) {
        AES_Funcs::sub_bytes(state, block_len, reinterpret_cast<uint8_t*>(&S_box_inv));
        AES_Funcs::shift_rows(state, block_len, true);
        if (i != rounds_cnt - 1) {
            AES_Funcs::mix_columns(state, block_len, true);
        }
        AES_Funcs::add_round_key(state, block_len, key + exp_key_size - (i + 1) * block_len);
    }

    memcpy(res, state, block_len);
}
