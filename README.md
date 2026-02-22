# SuperLong - Arbitrary Precision Arithmetic Library

A C++17 library for performing arithmetic operations on arbitrarily large integers, without any limitation from the system's native integer size.

## Features

- **Support for arbitrarily large integers**: No limitation on the number of digits
- **Basic arithmetic operations**: Addition, subtraction, multiplication, division, and modulo
- **Comparison operators**: support for ==, !=, <, <=, >, >=
- **Multiple input formats**: Support for int64_t and string inputs
- **Optimized multiplication**: Karatsuba algorithm for large numbers, fallback to simple multiplication for smaller numbers
- **Sign handling**: support for negative numbers

## Building

```bash
make test  
```

>[!NOTE]
> `superlong.hpp` - Main header file with class definition

## Internal Representation

The library stores numbers in a base-256 format (using uint8_t digits) in little-endian byte order. This allows efficient storage and manipulation of arbitrarily large integers.
