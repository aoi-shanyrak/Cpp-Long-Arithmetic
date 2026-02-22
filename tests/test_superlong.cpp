#include "superlong.hpp"
#include <iostream>
#include <cassert>
#include <string>

int testsPassed = 0;
int testsFailed = 0;

#define TEST(name, condition) \
  do { \
    if (condition) { \
      std::cout << "✓ " << name << std::endl; \
      testsPassed++; \
    } else { \
      std::cout << "✗ " << name << std::endl; \
      testsFailed++; \
    } \
  } while(0)

static SuperLong absValue(const SuperLong& value) {
  if (value.isNegative()) {
    return SuperLong(0LL) - value;
  }
  return value;
}

static void checkDivModRelation(const std::string& aStr, const std::string& bStr) {
  SuperLong a(aStr);
  SuperLong b(bStr);
  SuperLong q = a / b;
  SuperLong r = a % b;
  std::string label = "DivMod relation (" + aStr + "," + bStr + ")";
  TEST(label, a == b * q + r);
  if (!r.isZero()) {
    std::string boundLabel = "Remainder bound (" + aStr + "," + bStr + ")";
    TEST(boundLabel, absValue(r) < absValue(b));
  }
}

// Construction tests
void testConstruction() {
  std::cout << "\n=== Construction Tests ===" << std::endl;
  
  // Default constructor
  SuperLong zero;
  TEST("Default constructor creates zero", zero.isZero());
  
  // String constructors (most reliable way)
  SuperLong num1("42");
  TEST("Constructor from positive string", num1.toString() == "42");
  
  SuperLong num2("-42");
  TEST("Constructor from negative string", num2.toString() == "-42");
  
  // String constructors
  SuperLong fromStr1("123456789");
  TEST("Constructor from positive string", fromStr1.toString() == "123456789");
  
  SuperLong fromStr2("-987654321");
  TEST("Constructor from negative string", fromStr2.toString() == "-987654321");
  
  SuperLong zero_str("0");
  TEST("Constructor from zero string", zero_str.isZero());
  
  SuperLong large_str("123456789012345678901234567890");
  TEST("Constructor from very large string", large_str.toString() == "123456789012345678901234567890");
  
  SuperLong neg_str("-999999999999999999999999999999");
  TEST("Constructor from very large negative string", neg_str.toString() == "-999999999999999999999999999999");
  
  // Copy constructor
  SuperLong copy(fromStr1);
  TEST("Copy constructor", copy == fromStr1);
  
  // Move constructor (using temporary)
  SuperLong moved = SuperLong("12345");
  TEST("Move constructor", moved.toString() == "12345");
  
  // Assignment operators
  SuperLong assigned;
  assigned = fromStr1;
  TEST("Copy assignment", assigned == fromStr1);
  
  SuperLong moveAssigned;
  moveAssigned = SuperLong("54321");
  TEST("Move assignment", moveAssigned.toString() == "54321");
  
  // int64_t constructors
  SuperLong pos64(100LL);
  TEST("int64_t constructor positive", pos64.toString() == "100");
  
  SuperLong neg64(-100LL);
  TEST("int64_t constructor negative", neg64.toString() == "-100");
  
  SuperLong maxLong(9223372036854775807LL);
  TEST("int64_t INT64_MAX", maxLong.toString() == "9223372036854775807");

  SuperLong minLong(static_cast<int64_t>(-9223372036854775807LL - 1LL));
  TEST("int64_t INT64_MIN", minLong.toString() == "-9223372036854775808");
}

// Addition tests
void testAddition() {
  std::cout << "\n=== Addition Tests ===" << std::endl;
  
  // Positive + Positive
  SuperLong a("100");
  SuperLong b("200");
  TEST("100 + 200 = 300", (a + b).toString() == "300");
  
  // Negative + Negative
  SuperLong c("-100");
  SuperLong d("-200");
  TEST("-100 + -200 = -300", (c + d).toString() == "-300");
  
  // Positive + Negative (positive larger)
  SuperLong e("500");
  SuperLong f("-200");
  TEST("500 + -200 = 300", (e + f).toString() == "300");
  
  // Positive + Negative (negative larger)
  SuperLong g("100");
  SuperLong h("-500");
  TEST("100 + -500 = -400", (g + h).toString() == "-400");
  
  // Zero tests
  SuperLong zero(0LL);
  TEST("0 + 100 = 100", (zero + a).toString() == "100");
  TEST("100 + 0 = 100", (a + zero).toString() == "100");
  
  // Large numbers
  SuperLong big1("999999999999999999999");
  SuperLong big2("111111111111111111111");
  SuperLong result = big1 + big2;
  TEST("Large number addition", result.toString() == "1111111111111111111110");
  
  // Additive inverse
  TEST("x + (-x) = 0", (a + c).isZero());
  
  // Using int64_t
  TEST("SuperLong + int64_t", (a + 50LL).toString() == "150");
}

// Subtraction tests
void testSubtraction() {
  std::cout << "\n=== Subtraction Tests ===" << std::endl;
  
  SuperLong a("500");
  SuperLong b("200");
  TEST("500 - 200 = 300", (a - b).toString() == "300");
  
  SuperLong c("100");
  SuperLong d("500");
  TEST("100 - 500 = -400", (c - d).toString() == "-400");
  
  SuperLong e("-100");
  SuperLong f("-300");
  TEST("-100 - (-300) = 200", (e - f).toString() == "200");
  
  SuperLong g("-300");
  SuperLong h("-100");
  TEST("-300 - (-100) = -200", (g - h).toString() == "-200");
  
  // Self subtraction
  TEST("x - x = 0", (a - a).isZero());
  
  // Large numbers
  SuperLong big1("999999999999999999999");
  SuperLong big2("111111111111111111111");
  TEST("Large number subtraction", (big1 - big2).toString() == "888888888888888888888");
  
  // Using int64_t
  TEST("SuperLong - int64_t", (a - 100LL).toString() == "400");
}

// Multiplication tests
void testMultiplication() {
  std::cout << "\n=== Multiplication Tests ===" << std::endl;
  
  SuperLong a("12");
  SuperLong b("3");
  TEST("12 * 3 = 36", (a * b).toString() == "36");
  
  SuperLong c("-12");
  SuperLong d("3");
  TEST("-12 * 3 = -36", (c * d).toString() == "-36");
  
  SuperLong e("-12");
  SuperLong f("-3");
  TEST("-12 * -3 = 36", (e * f).toString() == "36");
  
  // Zero multiplication
  SuperLong zero(0LL);
  TEST("0 * 100 = 0", (zero * a).isZero());
  TEST("100 * 0 = 0", (a * zero).isZero());
  
  // One multiplication
  SuperLong one(1LL);
  TEST("1 * 123 = 123", (one * SuperLong("123")).toString() == "123");
  TEST("123 * 1 = 123", (SuperLong("123") * one).toString() == "123");
  
  // Larger numbers
  SuperLong big1("999");
  SuperLong big2("999");
  TEST("999 * 999 = 998001", (big1 * big2).toString() == "998001");
  
  // Very large numbers
  SuperLong huge1("123456789");
  SuperLong huge2("987654321");
  TEST("Large multiplication", (huge1 * huge2).toString() == "121932631112635269");
  
  // Using int64_t
  TEST("SuperLong * int64_t", (a * 4LL).toString() == "48");
}

// Division tests
void testDivision() {
  std::cout << "\n=== Division Tests ===" << std::endl;
  
  SuperLong a("100");
  SuperLong b("4");
  TEST("100 / 4 = 25", (a / b).toString() == "25");
  
  SuperLong c("100");
  SuperLong d("3");
  TEST("100 / 3 = 33", (c / d).toString() == "33");
  
  // Division with negatives
  SuperLong e("-100");
  SuperLong f("4");
  TEST("-100 / 4 = -25", (e / f).toString() == "-25");
  
  SuperLong g("100");
  SuperLong h("-4");
  TEST("100 / -4 = -25", (g / h).toString() == "-25");
  
  // Division resulting in zero
  SuperLong i("3");
  SuperLong j("10");
  TEST("3 / 10 = 0", (i / j).isZero());
  
  // Large number division
  SuperLong big1("999999999");
  SuperLong big2("999");
  TEST("999999999 / 999 = 1001001", (big1 / big2).toString() == "1001001");
  
  // Using int64_t
  TEST("SuperLong / int64_t", (a / 5LL).toString() == "20");
}

// Modulo tests
void testModulo() {
  std::cout << "\n=== Modulo Tests ===" << std::endl;
  
  SuperLong a("100");
  SuperLong b("3");
  TEST("100 % 3 = 1", (a % b).toString() == "1");
  
  SuperLong c("100");
  SuperLong d("4");
  TEST("100 % 4 = 0", (c % d).isZero());
  
  SuperLong e("17");
  SuperLong f("5");
  TEST("17 % 5 = 2", (e % f).toString() == "2");
  
  // Modulo with negatives
  SuperLong g("-17");
  SuperLong h("5");
  TEST("-17 % 5 = -2", (g % h).toString() == "-2");
  
  // Using int64_t
  TEST("SuperLong % int64_t", (a % 7LL).toString() == "2");
}

// Algebraic properties and invariants
void testAlgebraProperties() {
  std::cout << "\n=== Algebraic Properties ===" << std::endl;

  SuperLong a("12345678901234567890");
  SuperLong b("-9876543210987654321");
  SuperLong c("11111111111111111111");

  TEST("(a + b) - b = a", ((a + b) - b) == a);
  TEST("(a - b) + b = a", ((a - b) + b) == a);

  TEST("Addition commutativity", (a + c) == (c + a));
  TEST("Multiplication commutativity", (a * c) == (c * a));

  TEST("Addition associativity", ((a + b) + c) == (a + (b + c)));
  TEST("Multiplication distributivity", (a * (b + c)) == ((a * b) + (a * c)));

  SuperLong d("-1234567");
  SuperLong e("8901");
  TEST("Exact division after multiply", (d * e) / d == e);

  checkDivModRelation("12345678901234567890", "987654321");
  checkDivModRelation("-12345678901234567890", "987654321");
  checkDivModRelation("12345678901234567890", "-987654321");
}

// Comparison tests
void testComparison() {
  std::cout << "\n=== Comparison Tests ===" << std::endl;
  
  SuperLong a("100");
  SuperLong b("200");
  SuperLong c("100");
  
  // Equality
  TEST("100 == 100", a == c);
  TEST("100 != 200", a != b);
  TEST("NOT (100 == 200)", !(a == b));
  
  // Less than
  TEST("100 < 200", a < b);
  TEST("NOT (200 < 100)", !(b < a));
  TEST("NOT (100 < 100)", !(a < c));
  
  // Less than or equal
  TEST("100 <= 200", a <= b);
  TEST("100 <= 100", a <= c);
  TEST("NOT (200 <= 100)", !(b <= a));
  
  // Greater than
  TEST("200 > 100", b > a);
  TEST("NOT (100 > 200)", !(a > b));
  TEST("NOT (100 > 100)", !(a > c));
  
  // Greater than or equal
  TEST("200 >= 100", b >= a);
  TEST("100 >= 100", a >= c);
  TEST("NOT (100 >= 200)", !(a >= b));
  
  // Negative comparisons
  SuperLong neg1("-100");
  SuperLong neg2("-200");
  TEST("-100 > -200", neg1 > neg2);
  TEST("-200 < -100", neg2 < neg1);
  TEST("-100 == -100", neg1 == SuperLong("-100"));
  
  // Mixed sign comparisons
  TEST("-100 < 0 < 100", neg1 < SuperLong(0LL) && SuperLong(0LL) < a);
  
  // Large number comparisons
  SuperLong big1("999999999999999999999");
  SuperLong big2("999999999999999999998");
  TEST("big1 > big2", big1 > big2);
}

// Negation tests
void testNegation() {
  std::cout << "\n=== Negation Tests ===" << std::endl;
  
  SuperLong pos("100");
  pos.negate();
  TEST("negate(100) = -100", pos.toString() == "-100");
  
  SuperLong neg("-200");
  neg.negate();
  TEST("negate(-200) = 200", neg.toString() == "200");
  
  SuperLong zero(0LL);
  zero.negate();
  TEST("negate(0) = 0", zero.isZero());
}

// Sign checking tests
void testSignChecks() {
  std::cout << "\n=== Sign Checking Tests ===" << std::endl;
  
  SuperLong pos("100");
  TEST("100 is positive", pos.isPositive());
  TEST("NOT (100 is negative)", !pos.isNegative());
  TEST("NOT (100 is zero)", !pos.isZero());
  
  SuperLong neg("-100");
  TEST("-100 is negative", neg.isNegative());
  TEST("NOT (-100 is positive)", !neg.isPositive());
  
  SuperLong zero(0LL);
  TEST("0 is zero", zero.isZero());
  TEST("0 is positive", zero.isPositive());
  TEST("NOT (0 is negative)", !zero.isNegative());
}

// Edge cases
void testEdgeCases() {
  std::cout << "\n=== Edge Cases ===" << std::endl;
  
  // Division by zero
  SuperLong a("100");
  SuperLong zero(0LL);
  try {
    SuperLong result = a / zero;
    TEST("Division by zero throws exception", false);
  } catch (const std::invalid_argument&) {
    TEST("Division by zero throws exception", true);
  }
  
  // Modulo with zero
  try {
    SuperLong result = a % zero;
    TEST("Modulo by zero throws exception", false);
  } catch (const std::invalid_argument&) {
    TEST("Modulo by zero throws exception", true);
  }
  
  // Empty string construction
  try {
    SuperLong result("");
    TEST("Empty string throws exception", false);
  } catch (const std::invalid_argument&) {
    TEST("Empty string throws exception", true);
  }

  // Sign-only strings
  try {
    SuperLong result("+");
    TEST("Plus-only string throws exception", false);
  } catch (const std::invalid_argument&) {
    TEST("Plus-only string throws exception", true);
  }

  try {
    SuperLong result("-");
    TEST("Minus-only string throws exception", false);
  } catch (const std::invalid_argument&) {
    TEST("Minus-only string throws exception", true);
  }

  // Non-digit characters
  try {
    SuperLong result("12a34");
    TEST("Non-digit string throws exception", false);
  } catch (const std::invalid_argument&) {
    TEST("Non-digit string throws exception", true);
  }
  
  // Very large number operations
  SuperLong big1("999999999999999999999999999999");
  SuperLong big2("111111111111111111111111111111");
  SuperLong sum = big1 + big2;
  TEST("Very large addition", sum.toString() == "1111111111111111111111111111110");
  
  SuperLong product = SuperLong("1000000") * SuperLong("1000000");
  TEST("Large multiplication (1M * 1M)", product.toString() == "1000000000000");
}

// Fibonacci test (functional test)
void testFibonacci() {
  std::cout << "\n=== Functional Tests ===" << std::endl;
  
  // Calculate Fibonacci numbers
  SuperLong fib_prev(0LL);
  SuperLong fib_curr(1LL);
  
  for (int i = 0; i < 9; i++) {
    SuperLong fib_next = fib_prev + fib_curr;
    fib_prev = fib_curr;
    fib_curr = fib_next;
  }
  
  TEST("Fibonacci sequence works (10th: 55)", fib_curr.toString() == "55");
  
  // Calculate larger Fibonacci
  SuperLong f_prev(0LL);
  SuperLong f_curr(1LL);
  
  for (int i = 0; i < 49; i++) {
    SuperLong f_next = f_prev + f_curr;
    f_prev = f_curr;
    f_curr = f_next;
  }
  
  TEST("Large Fibonacci (50th number)", f_curr.toString() == "12586269025");
}

// String parsing edge cases
void testStringParsing() {
  std::cout << "\n=== String Parsing Tests ===" << std::endl;
  
  SuperLong pos_with_plus("+123");
  TEST("String with explicit plus sign", pos_with_plus.toString() == "123");
  
  SuperLong minus_sign("-456");
  TEST("String with minus sign", minus_sign.toString() == "-456");
  
  SuperLong leading_zeros("00123");
  TEST("String with leading zeros", leading_zeros.toString() == "123");

  SuperLong all_zeros("0000");
  TEST("All zeros string normalizes to zero", all_zeros.toString() == "0" && all_zeros.isZero());

  SuperLong negative_zero("-0");
  TEST("Negative zero normalizes to zero", negative_zero.toString() == "0" && negative_zero.isZero());
}

int main() {
  std::cout << "========================================" << std::endl;
  std::cout << "    SuperLong Comprehensive Test Suite" << std::endl;
  std::cout << "========================================" << std::endl;
  
  testConstruction();
  testAddition();
  testSubtraction();
  testMultiplication();
  testDivision();
  testModulo();
  testAlgebraProperties();
  testComparison();
  testNegation();
  testSignChecks();
  testEdgeCases();
  testFibonacci();
  testStringParsing();
  
  std::cout << "\n========================================" << std::endl;
  std::cout << "Results: " << testsPassed << " passed, " << testsFailed << " failed" << std::endl;
  std::cout << "========================================" << std::endl;
  
  return testsFailed > 0 ? 1 : 0;
}
