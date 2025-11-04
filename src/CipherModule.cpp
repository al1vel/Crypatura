#include "../include/CipherModule.h"

CipherModule::CipherModule(ISymmetricCipher *cipher, uint8_t *key, size_t key_len, Mode mode, Padding padding,
                           uint8_t *iv, std::initializer_list<std::any> additional) {
    this->cipher = cipher;
    this->key = key;
    this->key_len = key_len;
    this->padding = padding;
    this->iv = iv;
    this->mode = mode;
    additionalParams = additional;
}

void CipherModule::encrypt(uint8_t *data, size_t len_bytes, uint8_t *out) const {
    switch (this->mode) {
        case Mode::ECB: {
            size_t blocks = len_bytes / 8;
            //size_t total_blocks = blocks;
            // if (len_bytes % 8 != 0) {
            //     total_blocks++;
            // }

            for (size_t i = 0; i < blocks; ++i) {
                cipher->encrypt(data + i * 8, key, out + i * 8);
            }
            if (len_bytes % 8 != 0) {
                uint8_t last_block[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                memcpy(last_block, data + len_bytes, len_bytes % 8);
                cipher->encrypt(last_block, key, out + len_bytes);
            }

            break;
        }
        case Mode::CBC: {
            break;
        }
        case Mode::PCBC: {
            break;
        }
        case Mode::CFB: {
            break;
        }
        case Mode::OFB: {
            break;
        }
        case Mode::CTR: {
            break;
        }
        case Mode::RandomDelta: {
        }
        default: {
            break;
        }
    }
}

void CipherModule::decrypt(uint8_t *data, size_t len_bytes, uint8_t *out) const {
    switch (this->mode) {
        case Mode::ECB: {
            size_t blocks = len_bytes / 8;
            //out = new uint8_t[len_bytes];

            for (size_t i = 0; i < blocks; ++i) {
                cipher->decrypt(data + i * 8, key, out + i * 8);
            }
        }
        default: {
            break;
        }
    }
}
