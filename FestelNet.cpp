#include "FestelNet.h"

FestelNet::FestelNet(IRoundFunction *round_F, IKeyExtension *key_extenser) {
    this->round_F = round_F;
    this->key_extenser = key_extenser;
}

FestelNet::~FestelNet() {
    delete this->round_F;
    delete this->key_extenser;
}


void FestelNet::do_festel_net(uint8_t *block) {

}
