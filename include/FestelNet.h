#ifndef CRYPATURA_FESTELNET_H
#define CRYPATURA_FESTELNET_H

#include "Interfaces/IRoundFunction.h"
#include "Interfaces/IKeyExtension.h"

class FestelNet {
private:
    IRoundFunction *round_F;
    IKeyExtension *key_extenser;

public:
    FestelNet(IRoundFunction *round_F, IKeyExtension *key_extenser);

    ~FestelNet();

    void do_festel_net(uint8_t *block, uint8_t *key, uint8_t *out, size_t key_len, size_t rounds_cnt, size_t r_key_size, size_t block_size) const;

    void do_festel_net_reverse(uint8_t *block, uint8_t *key, uint8_t *out, size_t key_len, size_t rounds_cnt, size_t r_key_size, size_t block_size) const;
};


#endif //CRYPATURA_FESTELNET_H
