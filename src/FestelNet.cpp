#include "../include/FestelNet.h"
#include <iostream>
#include <bits/fs_fwd.h>

FestelNet::FestelNet(IRoundFunction *round_F, IKeyExtension *key_extenser) {
    this->round_F = round_F;
    this->key_extenser = key_extenser;
}

FestelNet::~FestelNet() {
    delete this->round_F;
    delete this->key_extenser;
}


void FestelNet::do_festel_net(uint8_t *block, uint8_t *key, uint8_t* out, size_t key_len, size_t rounds_cnt, size_t r_key_size, size_t block_size) const {
    auto* keys = new uint8_t[r_key_size * rounds_cnt]();
    key_extenser->key_extension(key, key_len, keys);

    uint64_t l = (*reinterpret_cast<uint64_t*>(block));
    uint64_t r = (*reinterpret_cast<uint64_t*>(block + (block_size / 2)));

    for (size_t i = 0; i < rounds_cnt; i++) {
        uint64_t round_F_result = 0;
        auto* r_ptr = reinterpret_cast<uint8_t*>(&r);
        round_F->do_round_func(r_ptr, keys + i * r_key_size, reinterpret_cast<uint8_t*>(&round_F_result));
        uint64_t xor_result = round_F_result ^ l;

        l = r;
        r = xor_result;
    }

    memcpy(out, &r, block_size / 2);
    memcpy(out + block_size / 2, &l, block_size / 2);
    delete[] keys;
}

void FestelNet::do_festel_net_reverse(uint8_t *block, uint8_t *key, uint8_t* out, size_t key_len, size_t rounds_cnt, size_t r_key_size, size_t block_size) const {
    auto* keys = new uint8_t[r_key_size * rounds_cnt]();
    key_extenser->key_extension(key, key_len, keys);

    uint64_t l = (*reinterpret_cast<uint64_t*>(block));
    uint64_t r = (*reinterpret_cast<uint64_t*>(block + (block_size / 2)));

    for (size_t i = 0; i < rounds_cnt; i++) {
        uint64_t round_F_result = 0;
        auto* r_ptr = reinterpret_cast<uint8_t*>(&r);
        round_F->do_round_func(r_ptr, keys + (rounds_cnt - i - 1) * r_key_size, reinterpret_cast<uint8_t*>(&round_F_result));
        uint64_t xor_result = round_F_result ^ l;

        l = r;
        r = xor_result;
    }


    memcpy(out, &r, block_size / 2);
    memcpy(out + block_size / 2, &l, block_size / 2);
    delete[] keys;
}
