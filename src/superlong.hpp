#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

using n256 = uint8_t;
using n256plus = uint32_t;

enum class Sign { Positive, Negative };

class SuperLong {
 public:
  SuperLong();
  SuperLong(int64_t num);
  SuperLong(const std::string& str);
  SuperLong(const SuperLong& other);
  SuperLong(SuperLong&& other) noexcept;
  ~SuperLong() = default;

  SuperLong& operator=(const SuperLong& other);
  SuperLong& operator=(SuperLong&& other) noexcept;

  SuperLong operator+(const SuperLong& other) const;
  SuperLong operator-(const SuperLong& other) const;
  SuperLong operator*(const SuperLong& other) const;
  SuperLong operator/(const SuperLong& other) const;
  SuperLong operator%(const SuperLong& other) const;

  SuperLong operator+(int64_t other) const;
  SuperLong operator-(int64_t other) const;
  SuperLong operator*(int64_t other) const;
  SuperLong operator/(int64_t other) const;
  SuperLong operator%(int64_t other) const;

  bool operator==(const SuperLong& other) const;
  bool operator!=(const SuperLong& other) const;
  bool operator<(const SuperLong& other) const;
  bool operator<=(const SuperLong& other) const;
  bool operator>(const SuperLong& other) const;
  bool operator>=(const SuperLong& other) const;

  void negate();

  bool isZero() const;
  bool isNegative() const;
  bool isPositive() const;

  std::string toString() const;

 private:
  Sign sign;
  std::vector<n256> digits;

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
