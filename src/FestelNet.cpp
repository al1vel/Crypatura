#include "../include/FestelNet.h"
#include <iostream>

FestelNet::FestelNet(IRoundFunction *round_F, IKeyExtension *key_extenser) {
    this->round_F = round_F;
    this->key_extenser = key_extenser;
}

FestelNet::~FestelNet() {
    delete this->round_F;
    delete this->key_extenser;
}


void FestelNet::do_festel_net(uint8_t *block, uint8_t *key) const {
    uint8_t keys[16][6];
    key_extenser->key_extension(key, &keys[0][0]);

    uint8_t l[4];
    memcpy(l, block, 4);
    uint8_t r[4];
    memcpy(r, block + 4, 4);

    for (int i = 0; i < 16; i++) {
        uint8_t round_F_result[4];
        round_F->do_round_func(r, keys[i], round_F_result);

        for (int j = 0; j < 4; j++) {
            round_F_result[j] ^= l[j];
        }

        memcpy(l, r, 4);
        memcpy(r, round_F_result, 4);
    }

    memcpy(block, r, 4);
    memcpy(block + 4, l, 4);
}

void FestelNet::do_festel_net_reverse(uint8_t *block, uint8_t *key) const {
    uint8_t keys[16][6];
    key_extenser->key_extension(key, &keys[0][0]);

    uint8_t l[4];
    memcpy(l, block, 4);
    uint8_t r[4];
    memcpy(r, block + 4, 4);

    for (int i = 0; i < 16; i++) {
        uint8_t round_F_result[4];
        round_F->do_round_func(r, keys[15 - i], round_F_result);

        for (int j = 0; j < 4; j++) {
            round_F_result[j] ^= l[j];
        }

        memcpy(l, r, 4);
        memcpy(r, round_F_result, 4);
    }

    memcpy(block, r, 4);
    memcpy(block + 4, l, 4);
}
