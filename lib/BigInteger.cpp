#include "BigInteger.hpp"
#include <stdexcept>

namespace {
// Add less absolute value to greater. Uses classical algorithm with carry digit.
// Single digit addition performs as:
// - converting lhs, rhs digits from char to int
// - computing digit sum: lhsDigit + rhsDigit + carry
// - updating carry digit
// - converting digit sum from int to char
void sumString(std::string& greater, const std::string& less) {
    auto rhsCRIt = less.crbegin();
    auto rhsCREnd = less.crend();
    auto lhsREnd = greater.rend();

    char carry = 0;
    for (auto lhsRIt = greater.rbegin(); lhsRIt != lhsREnd; ++lhsRIt) {
        bool lessEndReached {rhsCRIt == rhsCREnd};
        auto digitSum = *lhsRIt - '0' + carry;

        if (lessEndReached) {
            if (carry == 0) break;
        }
        else {
            digitSum += *rhsCRIt - '0';
            ++rhsCRIt;
        }

        carry = digitSum / 10;
        digitSum %= 10;
        *lhsRIt = '0' + digitSum;
    }

    if (carry) {
        greater.insert(0, 1, '0' + carry);
    }
}

// Subtract less absolute value from greater. Uses classical algorithm with borrow digit.
// Single digit subtraction looks similar to single digit addition.
void subtractString(std::string& greater, const std::string& less) {
    auto rhsCRIt = less.crbegin();
    auto rhsCREnd = less.crend();
    auto lhsREnd = greater.rend();

    char borrow = 0;
    for (auto lhsRIt = greater.rbegin(); lhsRIt != lhsREnd; ++lhsRIt) {
        bool lessEndReached {rhsCRIt == rhsCREnd};
        auto digitSub = *lhsRIt - '0' + borrow;

        if (lessEndReached) {
            if (borrow == 0) break;
        }
        else {
            digitSub -= *rhsCRIt - '0';
            ++rhsCRIt;
        }

        if (digitSub < 0) {
            digitSub += 10;
            borrow = -1;
        }
        else {
            borrow = 0;
        }
        *lhsRIt = '0' + digitSub;
    }
}

size_t countLeadingZeros(const std::string& number) {
    size_t count = 0;
    for ( ; count < number.size() - 1; ++count) {
        if (number.at(count) != '0') {
            break;
        }
    }
    return count;
}
}

namespace apmath {

BigInteger::BigInteger(const std::string &numberString)
    : mNumberString(numberString),
      mSign(Plus)
{
    parseSign();
    for (const auto& digit: mNumberString) {
        if (digit < '0' || digit > '9') {
            throw std::runtime_error("BigInteger format error: usage [+-][000]123456789");
        }
    }
    skipZeros();
}

BigInteger& BigInteger::operator+=(const BigInteger &rhs) {
    if (mSign == rhs.mSign) {
        add(rhs);
    }
    else {
        sub(rhs);
    }
    return *this;
}
// Uses: a - b = -(-a + b)
BigInteger& BigInteger::operator-=(const BigInteger &other) {
    changeSign();
    *this += other;
    changeSign();
    return *this;
}

void BigInteger::changeSign() {
    if (mNumberString != "0") {
        mSign = static_cast<Sign>(-(static_cast<int>(mSign)));
    }
}

void BigInteger::parseSign() {
    if (mNumberString.at(0) == '-') {
        mNumberString.erase(0, 1);
        mSign = Minus;
    }
    else if (mNumberString.at(0) == '+') {
        mNumberString.erase(0, 1);
    }
}
void BigInteger::skipZeros() {
    mNumberString.erase(0, countLeadingZeros(mNumberString));
    // Zero has plus sign
    if (mNumberString == "0") {
        mSign = Plus;
    }
}
// Compare absolute values of numbers and add less to greater
void BigInteger::add(const BigInteger &rhs) {
    if (compareNumberStrings(mNumberString, rhs.mNumberString) >= 0) {
        sumString(mNumberString, rhs.mNumberString);
    }
    else {
        std::string result = rhs.mNumberString;
        sumString(result, mNumberString);
        mNumberString = result;
    }
}
// Compare absolute values of numbers and subtract less from greater
void BigInteger::sub(const BigInteger &rhs) {
    if (compareNumberStrings(mNumberString, rhs.mNumberString) >= 0) {
        subtractString(mNumberString, rhs.mNumberString);
    }
    else {
        std::string result {rhs.mNumberString};
        subtractString(result, mNumberString);
        mNumberString = result;
        mSign = rhs.mSign;
    }
    skipZeros();
}

int BigInteger::compareNumberStrings(const std::string& lhs, const std::string& rhs) {
    if (lhs.size() != rhs.size()) {
        return lhs.size() - rhs.size();
    }
    return lhs.compare(rhs);
}

} // namespace apmath
