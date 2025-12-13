#ifndef BIG_INT_H
#define BIG_INT_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <complex>
#include <cstdint>

#define BASE 1000000000
#define SMALL_BASE 10000

std::size_t num_length(uint64_t num);

class BigInt {
private:
    std::vector<uint64_t> digits;
    bool isNegative;

public:
    BigInt();
    explicit BigInt(long long value);
    explicit BigInt(const std::string &str);
    BigInt(const BigInt &other);
    BigInt(BigInt &&other) noexcept;
    ~BigInt();

    BigInt abs() const;
    void remove_leading_zeros();
    bool is_zero() const;
    bool isOdd() const;
    bool isEven() const;
    BigInt operator%(const BigInt &other) const;

    static BigInt randomBigIntBelow(const BigInt& limit);
    void random(const BigInt& left, const BigInt& right);
    static BigInt random_odd_with_len(size_t bit_len);

    BigInt &operator=(const BigInt &other);
    BigInt &operator=(BigInt &&other) noexcept;
    BigInt operator+(const BigInt &other) const;
    BigInt operator-(const BigInt &other) const;
    BigInt operator*(const BigInt &other) const;
    BigInt operator/(const BigInt &other) const;
    BigInt& operator+=(const BigInt &other);
    BigInt& operator-=(const BigInt &other);
    BigInt& operator*=(const BigInt &other);
    BigInt& operator/=(const BigInt &other);
    BigInt& operator++();
    BigInt& operator--();

    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;
    bool operator<(const BigInt &other) const;
    bool operator>(const BigInt &other) const;
    bool operator<=(const BigInt &other) const;
    bool operator>=(const BigInt &other) const;

    [[nodiscard]] BigInt mod_exp(const BigInt &exp, const BigInt &mod) const;
    [[nodiscard]] BigInt karatsuba_multiply(const BigInt &a) const;

    friend std::istream &operator>>(std::istream &is, BigInt &num);
    friend std::ostream &operator<<(std::ostream &os, const BigInt &num);
};

#endif //BIG_INT_H