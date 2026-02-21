#pragma once

#include <string>
#include <vector>
#include <cstdint>

using n256 = uint8_t;

enum class Sign { Positive, Negative };

class SuperLong {
 private:
  std::vector<n256> digits;
  Sign sign;

  void removeLeadingZeros() {
    while (digits.size() > 1 and digits.back() == 0) {
      digits.pop_back();
    }
    if (digits.size() == 1 && digits[0] == 0) {
      sign = Sign::Positive;
    }
  }

  static SuperLong add(const SuperLong& a, const SuperLong& b);
  static SuperLong subtract(const SuperLong& a, const SuperLong& b);
  static SuperLong multiply(const SuperLong& a, const SuperLong& b);
  static SuperLong divide(const SuperLong& a, const SuperLong& b);

 public:
  SuperLong() : sign(Sign::Positive), digits(0) {}
  SuperLong(const std::string& str);
  SuperLong(const SuperLong& other);
  SuperLong(SuperLong&& other) noexcept;
  ~SuperLong();

  SuperLong& operator=(const SuperLong& other);
  SuperLong& operator=(SuperLong&& other) noexcept;

  SuperLong operator+(const SuperLong& other) const;
  SuperLong operator-(const SuperLong& other) const;
  SuperLong operator*(const SuperLong& other) const;
  SuperLong operator/(const SuperLong& other) const;

  bool operator==(const SuperLong& other) const;
  bool operator!=(const SuperLong& other) const;
  bool operator<(const SuperLong& other) const;
  bool operator<=(const SuperLong& other) const;
  bool operator>(const SuperLong& other) const;
  bool operator>=(const SuperLong& other) const;

  bool isZero() const { return digits.size() == 1 && digits[0] == 0; }
  bool isNegative() const { return sign == Sign::Negative; }
  bool isPositive() const { return sign == Sign::Positive; }

  std::string toString() const;
};
