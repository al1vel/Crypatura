#include "../include/CipherModule.h"

#include <iostream>
#include <random>
#include <thread>
#include <fstream>

inline void increment_counter(uint8_t* block, size_t block_size, uint64_t value) {
    auto* low  = reinterpret_cast<uint64_t *>(block + block_size - 8);
    uint64_t old_low = *low;
    *low = old_low + value;

    if (*low < old_low) {
        auto* high = reinterpret_cast<uint64_t *>(block);
        (*high)++;
    }
}

CipherModule::CipherModule(ISymmetricCipher *cipher, size_t block_size, uint8_t *key, size_t key_len, Mode mode, Padding padding,
                           uint8_t *iv, std::initializer_list<std::any> additional) {
    this->cipher = cipher;
    this->blocksiz = block_size;
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
            cipher->encrypt(data + i * blocksiz, key, res + i * blocksiz);
        } else {
            cipher->decrypt(data + i * blocksiz, key, res + i * blocksiz);
        }
    }
}

void CipherModule::CBC_thread(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res) const {
    for (int i = index; i < total_blocks; i += threads_cnt) {
        cipher->decrypt(data + i * blocksiz, key, res + i * blocksiz);
        for (size_t part = 0; part < blocksiz / 8; ++part) {
            *(reinterpret_cast<uint64_t*>(res + i * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + (i - 1) * blocksiz + part * 8));
        }
    }
}

void CipherModule::CFB_thread(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res) const {
    for (int i = index; i < total_blocks; i += threads_cnt) {
        cipher->encrypt(data + (i - 1) * blocksiz, key, res + i * blocksiz);
        for (size_t part = 0; part < blocksiz / 8; ++part) {
            *(reinterpret_cast<uint64_t*>(res + i * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + i * blocksiz + part * 8));
        }
    }
}

void CipherModule::Delta_thread_enc(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res, int delta) const {
    for (int i = index; i < total_blocks; i += threads_cnt) {
        // uint64_t iv_val = *(reinterpret_cast<uint64_t*>(iv));
        // iv_val += (i + 1) * delta;

        //auto* iv_val = new uint8_t[blocksiz]();
        uint8_t iv_val[16] = {0};
        memcpy(iv_val, iv, blocksiz);
        increment_counter(iv_val, blocksiz, (i + 1) * delta);

        cipher->encrypt(iv_val, key, res + (i + 1) * blocksiz);
        for (size_t part = 0; part < blocksiz / 8; ++part) {
            *(reinterpret_cast<uint64_t*>(res + (i + 1) * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + i * blocksiz + part * 8));
        }
        //delete[] iv_val;
    }
}

void CipherModule::Delta_thread_dec(int index, int threads_cnt, int total_blocks, uint8_t *data, uint8_t *res, int delta) const {
    for (int i = index; i < total_blocks; i += threads_cnt) {
        // uint64_t iv_val = *(reinterpret_cast<uint64_t*>(iv));
        // iv_val += (i + 1) * delta;

        //auto* iv_val = new uint8_t[blocksiz]();
        uint8_t iv_val[16] = {0};
        memcpy(iv_val, iv, blocksiz);
        increment_counter(iv_val, blocksiz, (i + 1) * delta);

        cipher->encrypt(iv_val, key, res + i * blocksiz);
        for (size_t part = 0; part < blocksiz / 8; ++part) {
            *(reinterpret_cast<uint64_t*>(res + i * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + (i + 1) * blocksiz + part * 8));
        }
        //delete[] iv_val;
    }
}

uint8_t *CipherModule::encrypt(uint8_t *data, size_t len_bytes, size_t *out_len) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    bool needPadding = false;

    size_t full_blocks = len_bytes / blocksiz;
    size_t blocks = len_bytes / blocksiz + 1;
    size_t bytes_remain = len_bytes % blocksiz;
    if (bytes_remain > 0) {
        ++blocks;
        needPadding = true;
    }

    //auto* last_block = new uint8_t[blocksiz]();
    uint8_t last_block[16] = {0};
    if (needPadding) {
        for (size_t k = 0; k < 8; ++k) {
            printf("%c", *(data + full_blocks * blocksiz + k));
        }
        std::cout << std::endl;
        memcpy(last_block, data + full_blocks * blocksiz, bytes_remain);

        switch (this->padding) {
            case Padding::Zeros: {
                break;
            }
            case Padding::ANSI_X923: {
                last_block[blocksiz - 1] = blocksiz - bytes_remain;
                break;
            }
            case Padding::ISO10126: {
                for (size_t i = bytes_remain; i < blocksiz - 1; ++i) {
                    last_block[i] = dist(gen);
                }
                last_block[blocksiz - 1] = blocksiz - bytes_remain;
                break;
            }
            case Padding::PKCS7: {
                for (size_t i = bytes_remain; i < blocksiz; ++i) {
                    last_block[i] = blocksiz - bytes_remain;
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    //auto* meta_block = new uint8_t[blocksiz]();
    uint8_t meta_block[16] = {0};
    meta_block[0] = needPadding ? blocksiz - bytes_remain : 0;
    for (size_t i = 1; i < blocksiz; ++i) {
        //meta_block[i] = dist(gen);
        meta_block[i] = 1;
    }

    auto *out = new uint8_t[blocks * blocksiz]();
    *out_len = blocks * blocksiz;

    switch (this->mode) {
        case Mode::ECB: {
            std::cout << "Encrypt with ECB" << std::endl;
            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::ECB_thread, this, i, threads_cnt, full_blocks, data, out, true);
            }
            for (auto &t: threads) {
                t.join();
            }
            if (needPadding) {
                cipher->encrypt(last_block, key, out + full_blocks * blocksiz);
                cipher->encrypt(meta_block, key, out + (full_blocks + 1) * blocksiz);
            } else {
                cipher->encrypt(meta_block, key, out + full_blocks * blocksiz);
            }
            break;
        }
        case Mode::CBC: {
            std::cout << "Encrypt with CBC" << std::endl;
            if (iv == nullptr) {
                std::cerr << "Encrypt with CBC got Null IV" << std::endl;
                return nullptr;
            }

            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t *>(meta_block + part * 8)) ^= *(reinterpret_cast<uint64_t *>(iv + part * 8));
            }
            cipher->encrypt(meta_block, key, out);

            for (size_t i = 0; i < full_blocks; ++i) {
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t *>(data + i * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t *>(out + i * blocksiz + part * 8));
                }
                cipher->encrypt(data + i * blocksiz, key, out + (i + 1) * blocksiz);
            }

            if (needPadding) {
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t *>(last_block + part * 8)) ^= *(reinterpret_cast<uint64_t *>(out + full_blocks * blocksiz + part * 8));
                }
                cipher->encrypt(last_block, key, out + (full_blocks + 1) * blocksiz);
            }
            break;
        }
        case Mode::PCBC: {
            std::cout << "Encrypt with PCBC" << std::endl;
            if (iv == nullptr) {
                std::cerr << "Encrypt with PCBC got Null IV" << std::endl;
                return nullptr;
            }

            uint64_t m = *(reinterpret_cast<uint64_t*>(meta_block));
            *(reinterpret_cast<uint64_t*>(meta_block)) ^= *(reinterpret_cast<uint64_t*>(iv));
            cipher->encrypt(meta_block, key, out);

            for (size_t i = 0; i < full_blocks; ++i) {
                uint64_t tmp = *(reinterpret_cast<uint64_t *>(data + i * blocksiz));
                *(reinterpret_cast<uint64_t*>(data + i * blocksiz)) ^= m;
                m = tmp;
                *(reinterpret_cast<uint64_t*>(data + i * blocksiz)) ^= *(reinterpret_cast<uint64_t*>(out + i * blocksiz));
                cipher->encrypt(data + i * blocksiz, key, out + (i + 1) * blocksiz);
            }

            if (needPadding) {
                *(reinterpret_cast<uint64_t *>(last_block)) ^= m;
                *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(out + full_blocks * blocksiz));
                cipher->encrypt(last_block, key, out + (full_blocks + 1) * blocksiz);
            }
            break;
        }
        case Mode::CFB: {
            std::cout << "Encrypt with CFB" << std::endl;
            if (iv == nullptr) {
                std::cerr << "Encrypt with CFB got Null IV" << std::endl;
                return nullptr;
            }

            cipher->encrypt(iv, key, out);
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(out + part * 8)) ^= *(reinterpret_cast<uint64_t*>(meta_block + part * 8));
            }

            for (size_t i = 0; i < full_blocks; ++i) {
                cipher->encrypt(out + i * blocksiz, key, out + (i + 1) * blocksiz);
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(out + (i + 1) * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + i * blocksiz + part * 8));
                }
            }
            if (needPadding) {
                cipher->encrypt(out + full_blocks * blocksiz, key, out + (full_blocks + 1) * blocksiz);
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(out + (full_blocks + 1) * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + full_blocks * blocksiz + part * 8));
                }
            }
            break;
        }
        case Mode::OFB: {
            std::cout << "Encrypt with OFB" << std::endl;
            if (iv == nullptr) {
                std::cerr << "Encrypt with OFB got Null IV" << std::endl;
                return nullptr;
            }

            cipher->encrypt(iv, key, out);

            //uint64_t Ek = *(reinterpret_cast<uint64_t*>(out));
            //auto* Ek = new uint8_t[blocksiz]();
            uint8_t Ek[16] = {0};
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(Ek + part * 8)) = *(reinterpret_cast<uint64_t*>(out + part * 8));
            }

            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(out + part * 8)) ^= *(reinterpret_cast<uint64_t*>(meta_block + part * 8));
            }

            for (size_t i = 0; i < full_blocks; ++i) {
                cipher->encrypt(Ek, key, out + (i + 1) * blocksiz);
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(Ek + part * 8)) = *(reinterpret_cast<uint64_t*>(out + (i + 1) * blocksiz + part * 8));
                }
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(out + (i + 1) * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + i * blocksiz + part * 8));
                }
            }
            if (needPadding) {
                cipher->encrypt(Ek, key, out + (full_blocks + 1) * blocksiz);
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(out + (full_blocks + 1) * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + full_blocks * blocksiz + part * 8));
                }
            }
            //delete[] Ek;
            break;
        }
        case Mode::CTR: {
            std::cout << "Encrypt with CTR" << std::endl;
            if (iv == nullptr) {
                std::cerr << "Encrypt with CTR got Null IV" << std::endl;
                return nullptr;
            }

            cipher->encrypt(iv, key, out);
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(out + part * 8)) ^= *(reinterpret_cast<uint64_t*>(meta_block + part * 8));
            }

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::Delta_thread_enc, this, i, threads_cnt, full_blocks, data, out, 1);
            }
            for (auto &t: threads) {
                t.join();
            }

            if (needPadding) {
                // uint64_t iv_val = *(reinterpret_cast<uint64_t*>(iv));
                // iv_val += full_blocks + 1;
                //auto* iv_val = new uint8_t[blocksiz]();
                uint8_t iv_val[16] = {0};
                memcpy(iv_val, iv, blocksiz);
                increment_counter(iv_val, blocksiz, full_blocks + 1);

                cipher->encrypt(iv_val, key, out + (full_blocks + 1) * blocksiz);
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(out + (full_blocks + 1) * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(last_block + part * 8));
                }
                //delete[] iv_val;
            }
            break;
        }
        case Mode::RandomDelta: {
            std::cout << "Encrypt with RD" << std::endl;
            if (iv == nullptr) {
                std::cerr << "Encrypt with RD got Null IV" << std::endl;
                return nullptr;
            }
            int delta = 4;

            cipher->encrypt(iv, key, out);
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(out + part * 8)) ^= *(reinterpret_cast<uint64_t*>(meta_block + part * 8));
            }

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::Delta_thread_enc, this, i, threads_cnt, full_blocks, data, out, delta);
            }
            for (auto &t: threads) {
                t.join();
            }

            if (needPadding) {
                // uint64_t iv_val = *(reinterpret_cast<uint64_t*>(iv));
                // iv_val += (full_blocks + 1) * delta;

                //auto* iv_val = new uint8_t[blocksiz]();
                uint8_t iv_val[16] = {0};
                memcpy(iv_val, iv, blocksiz);
                increment_counter(iv_val, blocksiz, (full_blocks + 1) * delta);

                cipher->encrypt(iv_val, key, out + (full_blocks + 1) * blocksiz);
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(out + (full_blocks + 1) * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(last_block + part * 8));
                }
                //delete[] iv_val;
            }
            break;
        }
        default: {
            std::cout << "Default case in encrypt modes" << std::endl;
            break;
        }
    }
    // delete[] last_block;
    // delete[] meta_block;
    return out;
}

uint8_t *CipherModule::decrypt(uint8_t *data, size_t len_bytes, size_t *out_len) const {
    size_t total_blocks = len_bytes / blocksiz;
    //auto* meta_block = new uint8_t[blocksiz]();
    uint8_t meta_block[16] = {0};

    switch (this->mode) {
        case Mode::ECB: {
            std::cout << "Decrypt with ECB" << std::endl;
            cipher->decrypt(data + (total_blocks - 1) * blocksiz, key, meta_block);
            size_t invaluable_bytes = meta_block[0];

            size_t out_bytes = (total_blocks - 1) * blocksiz;
            out_bytes -= invaluable_bytes;
            *(out_len) = out_bytes;
            auto *out = new uint8_t[out_bytes]();

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::ECB_thread, this, i, threads_cnt, total_blocks - 2, data, out, false);
            }
            for (auto &t: threads) {
                t.join();
            }

            //auto* last_block = new uint8_t[blocksiz]();
            uint8_t last_block[16] = {0};
            cipher->decrypt(data + (total_blocks - 2) * blocksiz, key, last_block);
            memcpy(out + (total_blocks - 2) * blocksiz, last_block, blocksiz - invaluable_bytes);

            // delete[] meta_block;
            // delete[] last_block;
            return out;
        }

        case Mode::CBC: {
            std::cout << "Decrypt with CBC" << std::endl;
            cipher->decrypt(data, key, meta_block);

            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(meta_block + part * 8)) ^= *(reinterpret_cast<uint64_t*>(iv + part * 8));
            }

            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * blocksiz - invaluable_bytes;
            *(out_len) = out_bytes;
            auto *out = new uint8_t[out_bytes]();

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::CBC_thread, this, i, threads_cnt, total_blocks - 2, data + blocksiz, out);
            }
            for (auto &t: threads) {
                t.join();
            }

            //auto* last_block = new uint8_t[blocksiz]();
            uint8_t last_block[16] = {0};
            cipher->decrypt(data + (total_blocks - 1) * blocksiz, key, last_block);
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(last_block + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 2) * blocksiz + part * 8));
            }

            memcpy(out + (total_blocks - 2) * blocksiz, last_block, blocksiz - invaluable_bytes);

            // delete[] meta_block;
            // delete[] last_block;
            return out;
        }

        case Mode::PCBC: {
            std::cout << "Decrypt with PCBC" << std::endl;
            cipher->decrypt(data, key, meta_block);
            *(reinterpret_cast<uint64_t*>(meta_block)) ^= *(reinterpret_cast<uint64_t*>(iv));
            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * blocksiz - invaluable_bytes;
            *(out_len) = out_bytes;
            auto *out = new uint8_t[out_bytes]();

            for (size_t i = 0; i < total_blocks - 2; ++i) {
                cipher->decrypt(data + (i + 1) * blocksiz, key, out + i * blocksiz);
                if (i == 0) {
                    *(reinterpret_cast<uint64_t*>(out + i * blocksiz)) ^= *(reinterpret_cast<uint64_t*>(meta_block));
                } else {
                    *(reinterpret_cast<uint64_t*>(out + i * blocksiz)) ^= *(reinterpret_cast<uint64_t*>(out + (i - 1) * blocksiz));
                }
                *(reinterpret_cast<uint64_t*>(out + i * blocksiz)) ^= *(reinterpret_cast<uint64_t*>(data + i * blocksiz));
            }
            //auto *last_block = new uint8_t[blocksiz]();
            uint8_t last_block[16] = {0};
            cipher->decrypt(data + (total_blocks - 1) * blocksiz, key, last_block);
            *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(out + (total_blocks - 3) * blocksiz));
            *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 2) * blocksiz));
            memcpy(out + (total_blocks - 2) * blocksiz, last_block, blocksiz - invaluable_bytes);

            // delete[] meta_block;
            // delete[] last_block;
            return out;
        }

        case Mode::CFB: {
            std::cout << "Decrypt with CFB" << std::endl;
            cipher->encrypt(iv, key, meta_block);
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(meta_block + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + part * 8));
            }
            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * blocksiz - invaluable_bytes;
            *(out_len) = out_bytes;
            auto *out = new uint8_t[out_bytes]();

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::CFB_thread, this, i, threads_cnt, total_blocks - 1, data + blocksiz, out);
            }
            for (auto &t: threads) {
                t.join();
            }

            //auto *last_block = new uint8_t[blocksiz]();
            uint8_t last_block[16] = {0};
            cipher->encrypt(data + (total_blocks - 2) * blocksiz, key, last_block);
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(last_block + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 1) * blocksiz + part * 8));
            }
            memcpy(out + (total_blocks - 2) * blocksiz, last_block, blocksiz - invaluable_bytes);

            // delete[] meta_block;
            // delete[] last_block;
            return out;
        }

        case Mode::OFB: {
            std::cout << "Decrypt with OFB" << std::endl;
            cipher->encrypt(iv, key, meta_block);

            //uint64_t Ek = *(reinterpret_cast<uint64_t*>(meta_block));
            //auto* Ek = new uint8_t[blocksiz]();
            uint8_t Ek[16] = {0};
            memcpy(Ek, meta_block, blocksiz);

            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(meta_block + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + part * 8));
            }
            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * blocksiz - invaluable_bytes;
            *(out_len) = out_bytes;
            auto *out = new uint8_t[out_bytes]();

            for (size_t i = 0; i < total_blocks - 2; ++i) {
                cipher->encrypt(Ek, key, out + i * blocksiz);
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(Ek + part * 8)) = *(reinterpret_cast<uint64_t*>(out + i * blocksiz + part * 8));
                }
                for (size_t part = 0; part < blocksiz / 8; ++part) {
                    *(reinterpret_cast<uint64_t*>(out + i * blocksiz + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + (i + 1) * blocksiz + part * 8));
                }
            }

            //auto* last_block = new uint8_t[blocksiz]();
            uint8_t last_block[16] = {0};
            cipher->encrypt(Ek, key, last_block);
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(last_block + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 1) * blocksiz + part * 8));
            }
            memcpy(out + (total_blocks - 2) * blocksiz, last_block, blocksiz - invaluable_bytes);

            // delete[] meta_block;
            // delete[] last_block;
            // delete[] Ek;
            return out;
        }

        case Mode::CTR: {
            std::cout << "Decrypt with CTR" << std::endl;
            cipher->encrypt(iv, key, meta_block);
            for (size_t part = 0; part < blocksiz / 8; ++part) {
                *(reinterpret_cast<uint64_t*>(meta_block + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + part * 8));
            }
            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * blocksiz - invaluable_bytes;
            *(out_len) = out_bytes;
            auto *out = new uint8_t[out_bytes]();

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::Delta_thread_dec, this, i, threads_cnt, total_blocks - 2, data, out, 1);
            }
            for (auto &t: threads) {
                t.join();
            }

            //auto *last_block = new uint8_t[blocksiz]();
            // uint64_t iv_val = *(reinterpret_cast<uint64_t*>(iv));
            // iv_val += total_blocks - 1;
            //uint8_t last_block[16] = {0};
            //auto* iv_val = new uint8_t[blocksiz]();
            // uint8_t iv_val[16] = {0};
            // memcpy(iv_val, iv, blocksiz);
            // increment_counter(iv_val, blocksiz, total_blocks - 1);
            //
            // cipher->encrypt(iv_val, key, last_block);
            // for (size_t part = 0; part < blocksiz / 8; ++part) {
            //     *(reinterpret_cast<uint64_t*>(last_block + part * 8)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 1) * blocksiz + part * 8));
            // }
            // memcpy(out + (total_blocks - 1) * blocksiz, last_block, blocksiz - invaluable_bytes);

            // delete[] meta_block;
            // delete[] last_block;
            // delete[] iv_val;
            return out;
        }

        case Mode::RandomDelta: {
            std::cout << "Decrypt with RD" << std::endl;
            int delta = 4;
            cipher->encrypt(iv, key, meta_block);
            *(reinterpret_cast<uint64_t*>(meta_block)) ^= *(reinterpret_cast<uint64_t*>(data));
            size_t invaluable_bytes = meta_block[0];
            size_t out_bytes = (total_blocks - 1) * blocksiz - invaluable_bytes;
            *(out_len) = out_bytes;
            auto *out = new uint8_t[out_bytes]();

            int threads_cnt = getThreadsCount(4);
            std::vector<std::thread> threads;
            for (int i = 0; i < threads_cnt; ++i) {
                threads.emplace_back(&CipherModule::Delta_thread_dec, this, i, threads_cnt, total_blocks - 2, data, out, delta);
            }
            for (auto &t: threads) {
                t.join();
            }

            //auto *last_block = new uint8_t[blocksiz]();
            // uint8_t last_block[16] = {0};
            // uint64_t iv_val = *(reinterpret_cast<uint64_t*>(iv));
            // iv_val += (total_blocks - 1) * delta;
            // cipher->encrypt(reinterpret_cast<uint8_t*>(&iv_val), key, last_block);
            // *(reinterpret_cast<uint64_t*>(last_block)) ^= *(reinterpret_cast<uint64_t*>(data + (total_blocks - 1) * blocksiz));
            // memcpy(out + (total_blocks - 1) * blocksiz, last_block, blocksiz - invaluable_bytes);

            // delete[] meta_block;
            // delete[] last_block;
            return out;
        }
        default: {
            return nullptr;
        }
    }
}

void CipherModule::encrypt_file(const std::string &inputPath, const std::string &outputPath) const {
    std::ifstream in(inputPath, std::ios::binary);
    std::ofstream out(outputPath, std::ios::binary);

    constexpr size_t BLOCK_SIZE = 16384;
    uint8_t buffer[BLOCK_SIZE];

    while (in) {
        in.read(reinterpret_cast<char*>(buffer), BLOCK_SIZE);
        std::streamsize bytes_read = in.gcount();

        if (bytes_read > 0) {
            size_t out_len = 0;
            uint8_t* enc = encrypt(buffer, bytes_read, &out_len);

            out.write(reinterpret_cast<char*>(enc), out_len);
            delete[] enc;
        }
    }
}

void CipherModule::decrypt_file(const std::string &inputPath, const std::string &outputPath, size_t block_size) const {
    std::ifstream in(inputPath, std::ios::binary);
    std::ofstream out(outputPath, std::ios::binary);

    size_t BLOCK_SIZE = 16384 + block_size;
    uint8_t* buffer = new uint8_t[BLOCK_SIZE]();

    while (in) {
        in.read(reinterpret_cast<char*>(buffer), BLOCK_SIZE);
        std::streamsize bytes_read = in.gcount();

        if (bytes_read > 0) {
            size_t out_len = 0;
            uint8_t* dec = decrypt(buffer, bytes_read, &out_len);

            out.write(reinterpret_cast<char*>(dec), out_len);
            delete[] dec;
        }
    }
    delete[] buffer;
}
