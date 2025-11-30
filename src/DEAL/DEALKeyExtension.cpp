#include "../../include/DEAL/DEALKeyExtension.h"
#include "../../include/DES/DES.h"
#include "../../include/CipherModule.h"

void DEALKeyExtension::key_extension(uint8_t *init_key, size_t init_key_len, uint8_t *result) {
    uint8_t des_key[8] = {1, 35, 69, 103, 137, 171, 205, 239};
    DES des_encryptor;

    if (init_key_len == 128) {
        uint64_t t = *reinterpret_cast<uint64_t*>(init_key);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result);

        t = *reinterpret_cast<uint64_t*>(init_key + 8) ^ *reinterpret_cast<uint64_t*>(result);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 8);

        uint64_t i = 0;
        i |= (1ULL << 63);
        t = *reinterpret_cast<uint64_t*>(init_key) ^ *reinterpret_cast<uint64_t*>(result + 8) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 16);

        i = 0;
        i |= (1ULL << 62);
        t = *reinterpret_cast<uint64_t*>(init_key + 8) ^ *reinterpret_cast<uint64_t*>(result + 16) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 24);

        i = 0;
        i |= (1ULL << 60);
        t = *reinterpret_cast<uint64_t*>(init_key) ^ *reinterpret_cast<uint64_t*>(result + 24) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 32);

        i = 0;
        i |= (1ULL << 56);
        t = *reinterpret_cast<uint64_t*>(init_key + 8) ^ *reinterpret_cast<uint64_t*>(result + 32) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 40);
    }
    else if (init_key_len == 192) {
        uint64_t t = *reinterpret_cast<uint64_t*>(init_key);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result);

        t = *reinterpret_cast<uint64_t*>(init_key + 8) ^ *reinterpret_cast<uint64_t*>(result);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 8);

        t = *reinterpret_cast<uint64_t*>(init_key + 16) ^ *reinterpret_cast<uint64_t*>(result + 8);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 16);

        uint64_t i = 0;
        i |= (1ULL << 63);
        t = *reinterpret_cast<uint64_t*>(init_key) ^ *reinterpret_cast<uint64_t*>(result + 16) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 24);

        i = 0;
        i |= (1ULL << 62);
        t = *reinterpret_cast<uint64_t*>(init_key + 8) ^ *reinterpret_cast<uint64_t*>(result + 24) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 32);

        i = 0;
        i |= (1ULL << 60);
        t = *reinterpret_cast<uint64_t*>(init_key + 16) ^ *reinterpret_cast<uint64_t*>(result + 32) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 40);
    }
    else if (init_key_len == 256) {
        uint64_t t = *reinterpret_cast<uint64_t*>(init_key);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result);

        t = *reinterpret_cast<uint64_t*>(init_key + 8) ^ *reinterpret_cast<uint64_t*>(result);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 8);

        t = *reinterpret_cast<uint64_t*>(init_key + 16) ^ *reinterpret_cast<uint64_t*>(result + 8);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 16);

        t = *reinterpret_cast<uint64_t*>(init_key + 24) ^ *reinterpret_cast<uint64_t*>(result + 16);
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 24);

        uint64_t i = 0;
        i |= (1ULL << 63);
        t = *reinterpret_cast<uint64_t*>(init_key) ^ *reinterpret_cast<uint64_t*>(result + 24) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 32);

        i = 0;
        i |= (1ULL << 62);
        t = *reinterpret_cast<uint64_t*>(init_key + 8) ^ *reinterpret_cast<uint64_t*>(result + 32) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 40);

        i = 0;
        i |= (1ULL << 60);
        t = *reinterpret_cast<uint64_t*>(init_key + 16) ^ *reinterpret_cast<uint64_t*>(result + 40) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 48);

        i = 0;
        i |= (1ULL << 56);
        t = *reinterpret_cast<uint64_t*>(init_key + 24) ^ *reinterpret_cast<uint64_t*>(result + 48) ^ i;
        des_encryptor.encrypt(reinterpret_cast<uint8_t*>(&t), des_key, result + 56);
    }
}