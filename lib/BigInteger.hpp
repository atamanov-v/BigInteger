#pragma once

#include <string>

/**
 * Arbitrary-precision math
 */
namespace apmath {

/**
 * @brief BigInteger class implements work with arbitrary-precision arithmetic.
 *
 * BigInteger is a simple class for working wtih arbitrary-precision integer arithmetic.
 * BigInteger supports addition, subtraction and comparison.
 */
class BigInteger {
public:
    /**
     * @brief BigInteger constructor
     * @param numberString - string representation of
     * a decimal integer number in format [+-][000]123456789
     */
    explicit BigInteger(const std::string& numberString);
    BigInteger(const BigInteger& other) = default;
    BigInteger& operator=(const BigInteger& other) = default;
    BigInteger(BigInteger&& other) noexcept = default;
    BigInteger& operator=(BigInteger&& other) noexcept = default;
    ~BigInteger() = default;

    BigInteger& operator+=(const BigInteger& rhs);

    friend inline BigInteger operator+(BigInteger lhs, const BigInteger& rhs) {
        lhs += rhs;
        return lhs;
    }

    BigInteger& operator-=(const BigInteger& other);

    friend inline BigInteger operator-(BigInteger lhs, const BigInteger& rhs) {
        lhs -= rhs;
        return lhs;
    }

    BigInteger& operator++() {
        *this += BigInteger("1");
        return *this;
    }
    BigInteger operator++(int) {
        BigInteger tmp(*this);
        operator++();
        return tmp;
    }

    BigInteger& operator--() {
        *this -= BigInteger("1");
        return *this;
    }
    BigInteger operator--(int) {
        BigInteger tmp(*this);
        operator--();
        return tmp;
    }

    BigInteger operator+() const {
        return *this;
    }

    BigInteger operator-() const {
        BigInteger result(*this);
        result.changeSign();
        return result;
    }

    friend inline bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
        return lhs.mSign == rhs.mSign && lhs.mNumberString == rhs.mNumberString;
    }
    friend inline bool operator!=(const BigInteger& lhs, const BigInteger& rhs) { return !(lhs == rhs); }

    friend bool operator< (const BigInteger& lhs, const BigInteger& rhs) {
        if (lhs.mSign != rhs.mSign) {
            return lhs.mSign == Minus;
        }
        int stringsDiff = compareNumberStrings(lhs.mNumberString, rhs.mNumberString);
        return lhs.mSign == Plus ? stringsDiff < 0 : stringsDiff > 0 ;
    }
    friend inline bool operator> (const BigInteger& lhs, const BigInteger& rhs){ return rhs < lhs; }
    friend inline bool operator<=(const BigInteger& lhs, const BigInteger& rhs){ return !(lhs > rhs); }
    friend inline bool operator>=(const BigInteger& lhs, const BigInteger& rhs){ return !(lhs < rhs); }

    /**
     * @brief toString - creates string representation of the decimal integer number
     * @return string representation
     */
    inline std::string toString() const {
        return mSign == Plus ? mNumberString : "-" + mNumberString;
    }

private:
    enum Sign {
        Minus = -1,
        Plus = 1
    };

    /**
     * @brief mNumberString - string representation of the absolute value of
     * the decimal integer number without leading zeros
     */
    std::string mNumberString;
    /**
     * @brief mSign - number's sign
     */
    Sign mSign;

    /**
     * @brief changeSign - changes number's sign
     */
    void changeSign();
    /**
     * @brief parseSign - parses number's sign in input string
     */
    void parseSign();
    /**
     * @brief skipZeros - erases leading zeros in constructor and after subtraction
     */
    void skipZeros();
    /**
     * @brief add - implements addition of the absolute values
     * @param rhs - BigInteger to add
     */
    void add(const BigInteger& rhs);
    /**
     * @brief sub - implements subtraction of the absolute values
     * @param rhs - BigInteger to subtract
     */
    void sub(const BigInteger& rhs);
    /**
     * @brief compareNumberStrings - compares the absolute values of the numbers represented by strings
     * lhs, rhs - string representations of the absolute values of the numbers to compare
     * @return Negative value if lhs is less than rhs.
     * Zero if lhs and rhs compare equal.
     * Positive value if lhs is greater than rhs.
     */
    static int compareNumberStrings(const std::string& lhs, const std::string& rhs);
};

} // namespace apmath
