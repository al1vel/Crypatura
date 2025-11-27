#include "../../include/DEAL/DEAL.h"
#include <cstring>
#include "../../include/DEAL/DEALKeyExtension.h"
#include "../../include/DEAL/DEALRoundFunction.h"

DEAL::DEAL(size_t key_length) : festel_net(FestelNet(new DEALRoundFunction(), new DEALKeyExtension())), key_len(key_length) {}

void DEAL::encrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    size_t rounds_cnt = 0;
    if (key_len == 128 || key_len == 192) {
        rounds_cnt = 6;
    } else if (key_len == 256) {
        rounds_cnt = 8;
    }
    festel_net.do_festel_net(block, key, res, key_len, rounds_cnt, 8, 16);
    //memcpy(res, block, 16);
}

void DEAL::decrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    size_t rounds_cnt = 0;
    if (key_len == 128 || key_len == 192) {
        rounds_cnt = 6;
    } else if (key_len == 256) {
        rounds_cnt = 8;
    }
    festel_net.do_festel_net_reverse(block, key, res, key_len, rounds_cnt, 8, 16);
    //memcpy(res, block, 16);
}
