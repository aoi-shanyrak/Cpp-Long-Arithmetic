#include "superlong.hpp"

void SuperLong::removeLeadingZeros() {
  while (digits.size() > 1 and digits.back() == 0) {
    digits.pop_back();
  }
  if (digits.size() == 1 && digits[0] == 0) {
    sign = Sign::Positive;
  }
}
