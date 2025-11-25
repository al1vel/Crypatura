#ifndef CRYPATURA_DEALROUNDFUNCTION_H
#define CRYPATURA_DEALROUNDFUNCTION_H

#include "../Interfaces/IRoundFunction.h"

class DEALRoundFunction : public IRoundFunction {
public:
    void do_round_func(uint8_t *arr, uint8_t *r_key, uint8_t *res) override;
};


#endif //CRYPATURA_DEALROUNDFUNCTION_H