#ifndef CRYPATURA_IKEYEXTENSION_H
#define CRYPATURA_IKEYEXTENSION_H
#include <cstdint>
#include <cstddef>

class IKeyExtension {
public:
    virtual void key_extension(uint8_t *init_key, size_t init_key_len, uint8_t *result) = 0;

    virtual ~IKeyExtension() = default;
};

#endif //CRYPATURA_IKEYEXTENSION_H
