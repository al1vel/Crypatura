#ifndef CRYPATURA_AESKEYEXTENSION_H
#define CRYPATURA_AESKEYEXTENSION_H

#include "../Interfaces/IKeyExtension.h"

class AESKeyExtension : public IKeyExtension {
private:
    size_t block_len = 0;
    uint8_t *S_box;

public:
    AESKeyExtension(size_t block_len, uint8_t *S_box_ptr) : block_len(block_len), S_box(S_box_ptr) {
    }

    void key_extension(uint8_t *init_key, size_t init_key_len, uint8_t *result) override;
};


#endif //CRYPATURA_AESKEYEXTENSION_H
