#include "../../include/DES/TripleDES.h"
#include "../../include/DES/DES.h"

void TripleDES::encrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    DES des;
    uint8_t temp1[8] = { 0 };
    uint8_t temp2[8] = { 0 };
    des.encrypt(block, key, temp1);
    des.decrypt(temp1, key + 8, temp2);
    des.encrypt(temp2, key + 16, res);
}

void TripleDES::decrypt(uint8_t *block, uint8_t *key, uint8_t *res) {
    DES des;
    uint8_t temp1[8] = { 0 };
    uint8_t temp2[8] = { 0 };
    des.decrypt(block, key + 16, temp1);
    des.encrypt(temp1, key + 8, temp2);
    des.decrypt(temp2, key, res);
}
