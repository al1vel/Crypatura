#include "DES.h"

DES::DES() : festel_net(FestelNet(new DESRoundFunction(), new DESKeyExtension())) {}

void DES::encrypt(uint8_t *block, size_t len, uint8_t *key, size_t key_len, uint8_t *res) {

}

void DES::decrypt(uint8_t *block, size_t len, uint8_t *key, size_t key_len, uint8_t *res) {

}


