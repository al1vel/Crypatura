#include "CipherModule.h"

CipherModule::CipherModule(ISymmetricCipher *cipher, uint8_t *key, size_t key_len, Mode mode, Padding padding, uint8_t *iv, std::initializer_list<std::any> additional) {
    this->cipher = cipher;
    this->key = key;
    this->key_len = key_len;
    this->padding = padding;
    this->iv = iv;
    this->mode = mode;
    additionalParams = additional;
}

void CipherModule::encrypt(uint8_t *data, size_t len, uint8_t *out) {
    switch (this->mode) {

    }
}

