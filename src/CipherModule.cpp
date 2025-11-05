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

            for (size_t i = 0; i < blocks; ++i) {
                cipher->encrypt(data + i * 8, key, out + i * 8);
            }

            size_t rem = len_bytes % 8;
            if (rem != 0) {
                uint8_t last_block[8] = {0};
                memcpy(last_block, data + blocks * 8, rem);
                cipher->encrypt(last_block, key, out + blocks * 8);
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
            break;
        }
        default: {
            break;
        }
    }
}
