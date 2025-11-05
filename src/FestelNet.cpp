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


void FestelNet::do_festel_net(uint8_t *block, uint8_t *key) const {
    uint8_t keys[96] = {0};
    key_extenser->key_extension(key, keys);

    uint32_t l32 = (*reinterpret_cast<uint32_t*>(block));
    uint32_t r32 = (*reinterpret_cast<uint32_t*>(block + 4));

    for (int i = 0; i < 16; i++) {
        uint8_t round_F_result[4] = {0};
        uint8_t* r_ptr = reinterpret_cast<uint8_t*>(&r32);
        round_F->do_round_func(r_ptr, keys + i * 6, round_F_result);

        uint32_t xor_result = (*reinterpret_cast<uint32_t*>(round_F_result)) ^ l32;

        l32 = r32;
        r32 = xor_result;
    }

    uint32_t* bl = reinterpret_cast<uint32_t*>(block);
    (*bl) = r32;
    *(bl + 1) = l32;
}

void FestelNet::do_festel_net_reverse(uint8_t *block, uint8_t *key) const {
    uint8_t keys[96] = {0};
    key_extenser->key_extension(key, keys);

    uint32_t l32 = (*reinterpret_cast<uint32_t*>(block));
    uint32_t r32 = (*reinterpret_cast<uint32_t*>(block + 4));

    for (int i = 0; i < 16; i++) {
        uint8_t round_F_result[4] = {0};
        uint8_t* r_ptr = reinterpret_cast<uint8_t*>(&r32);
        round_F->do_round_func(r_ptr, keys + ((15 - i) * 6), round_F_result);

        uint32_t xor_result = (*reinterpret_cast<uint32_t*>(round_F_result)) ^ l32;

        l32 = r32;
        r32 = xor_result;
    }

    uint32_t* bl = reinterpret_cast<uint32_t*>(block);
    (*bl) = r32;
    *(bl + 1) = l32;
}
