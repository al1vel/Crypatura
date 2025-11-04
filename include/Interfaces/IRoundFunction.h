#ifndef CRYPATURA_IROUNDFUNCTION_H
#define CRYPATURA_IROUNDFUNCTION_H
#include <cstdint>

class IRoundFunction {
    public:

    virtual void do_round_func(uint8_t* arr, uint8_t* r_key, uint8_t* res) = 0;

    virtual ~IRoundFunction() = default;
};

#endif //CRYPATURA_IROUNDFUNCTION_H