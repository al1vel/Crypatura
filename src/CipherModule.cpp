#include "../include/CipherModule.h"

#include <iostream>
#include <random>
#include <thread>

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

int CipherModule::getThreadsCount(int default_value) const {
    if (additionalParams.empty())
        return default_value;

    const auto &param = *additionalParams.begin();
    if (param.type() == typeid(int)) {
        return std::any_cast<int>(param);
    }

    return default_value;
}

void CipherModule::ECB_thread(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res,
                              bool enc) const {
    for (int i = index; i < total_blocks; i += threads_cnt) {
        if (enc) {
            cipher->encrypt(data + i * 8, key, res + i * 8);
        } else {
            cipher->decrypt(data + i * 8, key, res + i * 8);
        }
    }
}

void CipherModule::CBC_thread(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res) const {
    for (int i = index; i < total_blocks; i += threads_cnt) {
        cipher->decrypt(data + i * 8, key, res + i * 8);
        *(reinterpret_cast<uint64_t*>(res + i * 8)) ^= *(reinterpret_cast<uint64_t*>(data + (i - 1) * 8));
    }
}

void CipherModule::CFB_thread(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res) const {
    for (int i = index; i < total_blocks; i += threads_cnt) {
        cipher->encrypt(data + (i - 1) * 8, key, res + i * 8);
        *(reinterpret_cast<uint64_t*>(res + i * 8)) ^= *(reinterpret_cast<uint64_t*>(data + i * 8));
    }
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
            // for (size_t i = 0; i < full_blocks; ++i) {
            //     cipher->encrypt(data + i * 8, key, out + i * 8);
            // }

            int threads_cnt = getThreadsCount(4);
            //printf("Threads count: %d\n", threads_cnt);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::ECB_thread, this, i, threads_cnt, full_blocks, data, out, true);
            }

            for (auto &t: threads) {
                t.join();
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
            if (iv == nullptr) {
                std::cerr << "Encrypt with CBC got Null IV" << std::endl;
                return nullptr;
            }

            *(reinterpret_cast<uint64_t *>(meta_block)) ^= *(reinterpret_cast<uint64_t *>(iv));
            cipher->encrypt(meta_block, key, out);

            for (size_t i = 0; i < full_blocks; ++i) {
                *(reinterpret_cast<uint64_t *>(data + i * 8)) ^= *(reinterpret_cast<uint64_t *>(out + i * 8));
                cipher->encrypt(data + i * 8, key, out + (i + 1) * 8);
            }

            if (needPadding) {
                *(reinterpret_cast<uint64_t *>(last_block)) ^= *(reinterpret_cast<uint64_t *>(out + full_blocks * 8));
                cipher->encrypt(last_block, key, out + (full_blocks + 1) * 8);
            }
            break;
        }
        case Mode::PCBC: {
            if (iv == nullptr) {
                std::cerr << "Encrypt with PCBC got Null IV" << std::endl;
                return nullptr;
            }

            uint64_t m = *(reinterpret_cast<uint64_t*>(meta_block));
            *(reinterpret_cast<uint64_t*>(meta_block)) ^= *(reinterpret_cast<uint64_t*>(iv));
            cipher->encrypt(meta_block, key, out);

            for (size_t i = 0; i < full_blocks; ++i) {
                uint64_t tmp = *(reinterpret_cast<uint64_t *>(data + i * 8));
                *(reinterpret_cast<uint64_t*>(data + i * 8)) ^= m;
                m = tmp;
                *(reinterpret_cast<uint64_t*>(data + i * 8)) ^= *(reinterpret_cast<uint64_t*>(out + i * 8));
                cipher->encrypt(data + i * 8, key, out + (i + 1) * 8);
            }

            if (needPadding) {
                *(reinterpret_cast<uint64_t *>(last_block)) ^= m;
                *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(out + full_blocks * 8));
                cipher->encrypt(last_block, key, out + (full_blocks + 1) * 8);
            }

            break;
        }
        case Mode::CFB: {
            if (iv == nullptr) {
                std::cerr << "Encrypt with CFB got Null IV" << std::endl;
                return nullptr;
            }

            cipher->encrypt(iv, key, out);
            *(reinterpret_cast<uint64_t*>(out)) ^= *(reinterpret_cast<uint64_t*>(meta_block));

            for (size_t i = 0; i < full_blocks; ++i) {
                cipher->encrypt(out + i * 8, key, out + (i + 1) * 8);
                *(reinterpret_cast<uint64_t*>(out + (i + 1) * 8)) ^= *(reinterpret_cast<uint64_t*>(data + i * 8));
            }

            if (needPadding) {
                cipher->encrypt(out + full_blocks * 8, key, out + (full_blocks + 1) * 8);
                *(reinterpret_cast<uint64_t*>(out + (full_blocks + 1) * 8)) ^= *(reinterpret_cast<uint64_t*>(data + full_blocks * 8));
            }

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

uint8_t *CipherModule::decrypt(uint8_t *data, size_t len_bytes, size_t *out_len) const {
    size_t total_blocks = len_bytes / 8;
    uint8_t meta_block[8] = {0};

    switch (this->mode) {
        case Mode::ECB: {
            cipher->decrypt(data + (total_blocks - 1) * 8, key, meta_block);
            size_t invaluable_bytes = meta_block[0];

            size_t out_bytes = (total_blocks - 1) * 8;
            out_bytes -= invaluable_bytes;
            *(out_len) = out_bytes;

            auto *out = new uint8_t[out_bytes]();

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::ECB_thread, this, i, threads_cnt, total_blocks - 2, data, out,
                                     false);
            }

            for (auto &t: threads) {
                t.join();
            }

            uint8_t last_block[8] = {0};
            cipher->decrypt(data + (total_blocks - 2) * 8, key, last_block);
            memcpy(out + (total_blocks - 2) * 8, last_block, 8 - invaluable_bytes);

            return out;
        }

        case Mode::CBC: {
            cipher->decrypt(data, key, meta_block);
            *(reinterpret_cast<uint64_t*>(meta_block)) ^= *(reinterpret_cast<uint64_t*>(iv));
            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * 8 - invaluable_bytes;
            *(out_len) = out_bytes;

            auto *out = new uint8_t[out_bytes]();

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::CBC_thread, this, i, threads_cnt, total_blocks - 2, data + 8, out);
            }

            for (auto &t: threads) {
                t.join();
            }

            uint8_t last_block[8] = {0};
            cipher->decrypt(data + (total_blocks - 1) * 8, key, last_block);
            *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 2) * 8));
            memcpy(out + (total_blocks - 2) * 8, last_block, 8 - invaluable_bytes);

            return out;
        }
        case Mode::PCBC: {
            cipher->decrypt(data, key, meta_block);
            *(reinterpret_cast<uint64_t*>(meta_block)) ^= *(reinterpret_cast<uint64_t*>(iv));
            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * 8 - invaluable_bytes;
            *(out_len) = out_bytes;

            auto *out = new uint8_t[out_bytes]();

            for (size_t i = 0; i < total_blocks - 2; ++i) {
                cipher->decrypt(data + (i + 1) * 8, key, out + i * 8);
                if (i == 0) {
                    *(reinterpret_cast<uint64_t*>(out + i * 8)) ^= *(reinterpret_cast<uint64_t*>(meta_block));
                } else {
                    *(reinterpret_cast<uint64_t*>(out + i * 8)) ^= *(reinterpret_cast<uint64_t*>(out + (i - 1) * 8));
                }
                *(reinterpret_cast<uint64_t*>(out + i * 8)) ^= *(reinterpret_cast<uint64_t*>(data + i * 8));
            }
            uint8_t last_block[8] = {0};
            cipher->decrypt(data + (total_blocks - 1) * 8, key, last_block);
            *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(out + (total_blocks - 3) * 8));
            *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 2) * 8));
            memcpy(out + (total_blocks - 2) * 8, last_block, 8 - invaluable_bytes);
            return out;
        }
        case Mode::CFB: {
            cipher->encrypt(iv, key, meta_block);
            *(reinterpret_cast<uint64_t*>(meta_block)) ^= *(reinterpret_cast<uint64_t*>(data));
            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * 8 - invaluable_bytes;
            *(out_len) = out_bytes;

            auto *out = new uint8_t[out_bytes]();

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::CFB_thread, this, i, threads_cnt, total_blocks - 2, data + 8, out);
            }
            for (auto &t: threads) {
                t.join();
            }

            uint8_t last_block[8] = {0};
            cipher->encrypt(data + (total_blocks - 2) * 8, key, last_block);
            *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 1) * 8));
            memcpy(out + (total_blocks - 2) * 8, last_block, 8 - invaluable_bytes);

            return out;
        }
        default: {
            return nullptr;
        }
    }
}
