#ifndef CRYPATURA_DESKEYEXTENSION_H
#define CRYPATURA_DESKEYEXTENSION_H
#include "../Interfaces/IKeyExtension.h"

const int PC_1[56] = {
    57, 49, 41, 33, 25, 17, 9,
1, 58, 50, 42, 34, 26, 18,
10, 2, 59, 51, 43, 35, 27,
19, 11, 3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
7, 62, 54, 46, 38, 30, 22,
14, 6, 61, 53, 45, 37, 29,
21, 13, 5, 28, 20, 12, 4

};

class DESKeyExtension : public IKeyExtension {
public:
    void key_extension(uint8_t *init_key, size_t init_key_len, uint8_t **result, size_t rounds) override;
};


#endif //CRYPATURA_DESKEYEXTENSION_H
