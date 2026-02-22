#include "superlong.hpp"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>


SuperLong::SuperLong(const std::string& str) {
  if (str.empty()) {
    throw std::invalid_argument("Input string cannot be empty");
  }

  size_t start = 0;
  if (str[0] == '-') {
    sign = Sign::Negative;
    start = 1;
  } else {
    sign = Sign::Positive;
    if (str[0] == '+') {
      start = 1;
    }
  }
  std::string strc = str.substr(start);

  if (strc == "0") {
    digits.push_back(0);
    sign = Sign::Positive;
    return;
  }

  while (!strc.empty()) {
    n256 carry = 0;
    std::string new_str;

    for (char c : strc) {
      n256plus cur = (c - '0') + carry * 10;
      n256 digit_quo = static_cast<n256>(cur / 256);
      carry = static_cast<n256>(cur % 256);

      if (!new_str.empty() or digit_quo != 0) {
        new_str += (digit_quo + '0');
      }
    }
    digits.push_back(carry);
    strc = new_str;
  }
}


void SuperLong::initFromUint64(uint64_t num) {
  if (num == 0) {
    digits.push_back(0);
    return;
  }
  while (num > 0) {
    digits.push_back(static_cast<n256>(num % 256));
    num /= 256;
  }
}


SuperLong::SuperLong(uint64_t num) : sign(Sign::Positive) {
  initFromUint64(num);
}


SuperLong::SuperLong(int64_t num) {
  if (num == INT64_MIN) {
    sign = Sign::Negative;
    digits = std::vector<n256> {0, 0, 0, 0, 0, 0, 0, 128};  // 2^63 in little-endian
    return;
  }
  if (num < 0) {
    sign = Sign::Negative;
    num = -num;
    initFromUint64(static_cast<uint64_t>(num));
  } else {
    sign = Sign::Positive;
    initFromUint64(static_cast<uint64_t>(num));
  }
}


SuperLong& SuperLong::operator=(const SuperLong& other) {
  if (this != &other) {
    sign = other.sign;
    digits = other.digits;
  }
  return *this;
}


SuperLong& SuperLong::operator=(SuperLong&& other) noexcept {
  if (this != &other) {
    sign = other.sign;
    digits = std::move(other.digits);
  }
  return *this;
}


std::string SuperLong::toString() const {
  if (isZero()) {
    return "0";
  }
  std::string result;
 
  SuperLong temp = *this;
  temp.sign = Sign::Positive;

  while (!temp.isZero()) {
    auto [quotient, remainder] = divide_quo_rem(temp, SuperLong(static_cast<n256>(10)));
    temp = quotient;
    result += static_cast<char>(remainder.digits[0] + '0');
  }
  if (sign == Sign::Negative) {
    result += '-';
  }
  std::reverse(result.begin(), result.end());
  return result;
}
