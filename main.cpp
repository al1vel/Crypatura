#include <cstdint>
#include <iostream>



class EncAlg {
public:
    virtual void key_extension(uint8_t *init_key, size_t init_key_len, uint8_t **result, size_t rounds) = 0;



    virtual ~EncAlg() = default;
};

int main() {
    return 0;
}
