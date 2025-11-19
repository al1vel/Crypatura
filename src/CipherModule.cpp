#include "../include/CipherModule.h"
#include <random>

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

uint8_t *CipherModule::encrypt(uint8_t *data, size_t len_bytes, size_t *out_len) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    bool needPadding = false;

    size_t full_blocks = len_bytes / 8;
    size_t blocks = len_bytes / 8 + 1;
    size_t bytes_remain = len_bytes % 8;
    if (bytes_remain > 0) {
        ++blocks;
        needPadding = true;
    }

    uint8_t last_block[8] = {0};
    if (needPadding) {
        memcpy(last_block, data + full_blocks * 8, bytes_remain);

        switch (this->padding) {
            case Padding::Zeros: {
                break;
            }
            case Padding::ANSI_X923: {
                last_block[7] = 8 - bytes_remain;
                break;
            }
            case Padding::ISO10126: {
                for (size_t i = bytes_remain; i < 8 - bytes_remain - 1; ++i) {
                    last_block[i] = dist(gen);
                }
                last_block[7] = 8 - bytes_remain;
                break;
            }
            case Padding::PKCS7: {
                for (size_t i = bytes_remain; i < 8 - bytes_remain; ++i) {
                    last_block[i] = 8 - bytes_remain;
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    uint8_t meta_block[8] = {0};
    meta_block[0] = needPadding ? 8 - bytes_remain : 0;
    for (int i = 1; i < 8; ++i) {
        meta_block[i] = dist(gen);
    }

    auto *out = new uint8_t[blocks * 8]();
    *out_len = blocks * 8;

    switch (this->mode) {
        case Mode::ECB: {
            for (size_t i = 0; i < full_blocks; ++i) {
                cipher->encrypt(data + i * 8, key, out + i * 8);
            }

            if (needPadding) {
                cipher->encrypt(last_block, key, out + full_blocks * 8);
                cipher->encrypt(meta_block, key, out + (full_blocks + 1) * 8);
            } else {
                cipher->encrypt(meta_block, key, out + full_blocks * 8);
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
    return out;
}

uint8_t* CipherModule::decrypt(uint8_t *data, size_t len_bytes, size_t *out_len) const {
    size_t total_blocks = len_bytes / 8;

    uint8_t meta_block[8] = {0};
    cipher->decrypt(data + (total_blocks - 1) * 8, key, meta_block);
    size_t invaluable_bytes = meta_block[0];

    size_t out_bytes = (total_blocks - 1) * 8;
    out_bytes -= invaluable_bytes;
    *(out_len) = out_bytes;

    auto *out = new uint8_t[out_bytes]();

    switch (this->mode) {
        case Mode::ECB: {
            for (size_t i = 0; i < total_blocks - 2; ++i) {
                cipher->decrypt(data + i * 8, key, out + i * 8);
            }
            uint8_t last_block[8] = {0};
            cipher->decrypt(data + (total_blocks - 2) * 8, key, last_block);
            memcpy(out + (total_blocks - 2) * 8, last_block, 8 - invaluable_bytes);
            break;
        }
        default: {
            break;
        }
    }
    return out;
}
