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

    void do_festel_net(uint8_t *block);
};


#endif //CRYPATURA_FESTELNET_H
