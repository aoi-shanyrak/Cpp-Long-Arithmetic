#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

using n256 = uint8_t;
using n256plus = uint32_t;


enum class Sign { Positive, Negative };


class SuperLong {
 public:
  SuperLong() : sign(Sign::Positive), digits(0) {}
  SuperLong(int64_t num);
  SuperLong(const std::string& str);
  SuperLong(const SuperLong& other) : sign(other.sign), digits(other.digits) {}
  SuperLong(SuperLong&& other) noexcept : sign(other.sign), digits(std::move(other.digits)) {}
  ~SuperLong() = default;

  SuperLong& operator=(const SuperLong& other);
  SuperLong& operator=(SuperLong&& other) noexcept;

  SuperLong operator+(const SuperLong& other) const { return add(*this, other); }
  SuperLong operator-(const SuperLong& other) const { return subtract(*this, other); }
  SuperLong operator*(const SuperLong& other) const { return multiply(*this, other); }
  SuperLong operator/(const SuperLong& other) const { return divide_quo_rem(*this, other).first; }
  SuperLong operator%(const SuperLong& other) const { return divide_quo_rem(*this, other).second; }

  SuperLong operator+(int64_t other) const { return add(*this, SuperLong(other)); }
  SuperLong operator-(int64_t other) const { return subtract(*this, SuperLong(other)); }
  SuperLong operator*(int64_t other) const { return multiply(*this, SuperLong(other)); }
  SuperLong operator/(int64_t other) const { return divide_quo_rem(*this, SuperLong(other)).first; }
  SuperLong operator%(int64_t other) const { return divide_quo_rem(*this, SuperLong(other)).second; }

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

 private:
  std::vector<n256> digits;
  Sign sign;

  SuperLong(n256 num) : sign(Sign::Positive), digits(1, num) {}
  SuperLong(uint64_t num);

  void removeLeadingZeros();

  void initFromUint64(uint64_t num);

  static int abscmp(const SuperLong& a, const SuperLong& b);

  static SuperLong add(const SuperLong& a, const SuperLong& b);
  static SuperLong addAbs(const SuperLong& a, const SuperLong& b);

  static SuperLong subtract(const SuperLong& a, const SuperLong& b);
  static SuperLong subtractAbs(const SuperLong& a, const SuperLong& b);

  static SuperLong multiply(const SuperLong& a, const SuperLong& b);
  static SuperLong multiply_simple(const SuperLong& a, const SuperLong& b);
  static SuperLong multiply_karatsuba(const SuperLong& a, const SuperLong& b);

  static std::pair<SuperLong, SuperLong> divide_quo_rem(const SuperLong& a, const SuperLong& b);

  SuperLong multi256n(size_t shift) const;
  SuperLong divid256n(size_t shift) const;
};
