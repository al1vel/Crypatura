#ifndef CRYPATURA_DESKEYEXTENSION_H
#define CRYPATURA_DESKEYEXTENSION_H
#include "../Interfaces/IKeyExtension.h"


class DESKeyExtension : public IKeyExtension {
public:
    void key_extension(uint8_t *init_key, size_t init_key_len, uint8_t **result, size_t rounds) override;
};


#endif //CRYPATURA_DESKEYEXTENSION_H
