#include "superlong.hpp"

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

using n256plus = uint32_t;

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
