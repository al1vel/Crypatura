#ifndef CRYPATURA_DEALKEYEXTENSION_H
#define CRYPATURA_DEALKEYEXTENSION_H

#include "../Interfaces/IKeyExtension.h"

class DEALKeyExtension : public IKeyExtension {
public:
    void key_extension(uint8_t *init_key, size_t init_key_len, uint8_t *result) override;
};


#endif //CRYPATURA_DEALKEYEXTENSION_H