#ifndef CRYPATURA_IROUNDFUNCTION_H
#define CRYPATURA_IROUNDFUNCTION_H
#include <cstdint>

class IRoundFunction {
    public:

    virtual void encryptBlock(uint8_t* arr, size_t len, uint8_t* r_key, size_t key_len, uint8_t* res) = 0;

    virtual ~IRoundFunction() = default;
};

#endif //CRYPATURA_IROUNDFUNCTION_H