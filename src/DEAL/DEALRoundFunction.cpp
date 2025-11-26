#include "../../include/DEAL/DEALRoundFunction.h"
#include "../../include/DES/DES.h"

void DEALRoundFunction::do_round_func(uint8_t *arr, uint8_t *r_key, uint8_t *res) {
    DES des_encryptor;
    des_encryptor.encrypt(arr, r_key, res);
}