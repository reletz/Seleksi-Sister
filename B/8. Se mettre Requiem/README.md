# Se mettre Requiem - Big Integer Multiplication

A high-performance big integer multiplication program written in C with strict language constraints.

**Special thanks to *https://cp4space.hatsya.com/2021/09/01/an-efficient-prime-for-number-theoretic-transforms/* for the prime number**

## Project Structure

```
.
├── bin/
│   ├── mult          # Linux executable
│   └── mult.exe      # Windows executable
├── src/
│   ├── mult.c        # Main implementation
│   └── mult.h        # Header file with declarations
├── compile.bat       # Windows compilation script
├── compile.sh        # Linux compilation script
└── README.md         # This file
```

## Problem Statement

Create a C program that takes two positive integers as input (via stdin) and outputs their multiplication result (via stdout). The implementation must follow strict constraints:

**Allowed Operations:**

- All bitwise operators: `<<`, `!`, `~`, `>>`, `&`, `|`, `^`
- `goto` keyword
- `if` keyword
- Static primitive arrays (no malloc)
- `printf` and `scanf` from stdio.h only
- All data types including `__uint128_t`
- Function declarations and macro definitions

**Forbidden:**

- All arithmetic operators (`+`, `-`, `*`, `/`, `%`)
- Any library except stdio.h
- Dynamic memory allocation
- Loop keywords (`for`, `while`, `do-while`)

## Compilation

### Windows

```bash
# Using the batch script
compile.bat

# Manual compilation
gcc -std=gnu99 -O2 -g -Wall -o bin\mult.exe src\mult.c
```

### Linux/Unix

```bash
# Using the shell script
chmod +x compile.sh
./compile.sh

# Manual compilation
gcc -std=gnu99 -O2 -g -Wall -o bin/mult src/mult.c
```

### Compilation Flags Explained

- `-std=gnu99`: Use C99 standard with GNU extensions (for `__uint128_t`)
- `-O2`: Enable optimization level 2 for better performance
- `-g`: Include debugging information
- `-Wall`: Enable most warning messages

## Usage

### Windows

```bash
bin\mult.exe
```

### Linux/Unix

```bash
./bin/mult
```

**Example Session:**

```
Masukkan bilangan pertama: 123456789012345678901234567890
Masukkan bilangan kedua: 987654321098765432109876543210

Memulai perkalian...

Result (base 10): 121932631137021795226185032733622923332237463801111263526900
```

## Implementation

This solution implements a complete big integer arithmetic system using only bitwise operations and goto statements:

### Core Features

- **Custom arithmetic**: Addition, subtraction, multiplication using bitwise operations
- **Big integer representation**: Base-2^63 number system for efficient computation
- **NTT multiplication**: Number Theoretic Transform for O(N log N) complexity
- **String parsing**: Convert decimal strings to internal representation
- **Result formatting**: Convert back to decimal for output

### Performance Targets

- **Subproblem 1**: Handle numbers in range -2^32 < x < 2^32
- **Subproblem 2**: Handle numbers in range -10^1000 < x < 10^1000
- **Subproblem 3**: Handle numbers in range -10^1000000 < x < 10^1000000

### Algorithm Complexity

- **Subproblem 2**: O(N²) traditional multiplication
- **Subproblem 3**: O(N log N) using Number Theoretic Transform

## Technical Details

### Architecture

- **Base representation**: 2^63 for maximum efficiency with 128-bit arithmetic
- **NTT prime**: 18446744069414584321 (optimized for 128-bit operations)
- **Memory management**: Static arrays with compile-time bounds
- **Control flow**: Goto-based loops for performance and constraint compliance

### Key Components

- `mult.h`: Function declarations and macros
- `mult.c`: Complete implementation with bitwise arithmetic
- Custom comparison, increment/decrement operations
- Big integer parsing and formatting
- NTT-based fast multiplication

## Constraints Compliance

This implementation strictly adheres to all problem constraints:

- No arithmetic operators (implemented via bitwise operations)
- No forbidden keywords (uses goto for all loops)
- Only stdio.h used
- Static arrays only
- Handles all required number ranges
- Meets performance requirements

## Performance

- **Small numbers** (< 2^32): Near-instant
- **Medium numbers** (< 10^1000): Sub-second using O(N²) algorithm
- **Large numbers** (< 10^1000000): Sub-second using O(N log N) NTT

Helped by Gemini and Claude :D
