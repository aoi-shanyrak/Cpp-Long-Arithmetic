#pragma once

#include <cstdint>
#include <string>
#include <vector>

using n256 = uint8_t;
using n256plus = uint32_t;


enum class Sign { Positive, Negative };


class SuperLong {
 private:
  std::vector<n256> digits;
  Sign sign;

  void removeLeadingZeros();

  static SuperLong add(const SuperLong& a, const SuperLong& b);
  static SuperLong subtract(const SuperLong& a, const SuperLong& b);
  static SuperLong multiply(const SuperLong& a, const SuperLong& b);
  static SuperLong divide(const SuperLong& a, const SuperLong& b);

 public:
  SuperLong() : sign(Sign::Positive), digits(0) {}
  SuperLong(n256 num) : sign(Sign::Positive), digits(1, num) {}
  SuperLong(uint32_t num);
  SuperLong(int64_t num);
  SuperLong(const std::string& str);
  SuperLong(const SuperLong& other) : sign(other.sign), digits(other.digits) {}
  SuperLong(SuperLong&& other) noexcept : sign(other.sign), digits(std::move(other.digits)) {}
  ~SuperLong() = default;

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

  void negate() { sign = (sign == Sign::Positive) ? Sign::Negative : Sign::Positive; }

  bool isZero() const { return digits.size() == 1 && digits[0] == 0; }
  bool isNegative() const { return sign == Sign::Negative; }
  bool isPositive() const { return sign == Sign::Positive; }

  std::string toString() const;
};
