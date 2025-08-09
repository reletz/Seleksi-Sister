#ifndef __MULT_H__
#define __MULT_H__

#include <stdio.h>

#define EQ(A, B) (!((A) ^ (B)))
#define NE(A, B) (!EQ(A, B))
#define LT(A, B) (EQ(cmp((A), (B)), ~0))
#define GT(A, B) (EQ(cmp((A), (B)), 1))
#define LE(A, B) (!GT(A, B))
#define GE(A, B) (!LT(A, B))

#define IS_DIGIT(C) (EQ((C) & 0xF0, 0x30) & LE((C) & 0x0F, 9))

#define MAX_LEN 55
#define BASE_BITS 63
#define BASE ((__uint128_t)1 << BASE_BITS)
#define MASK 0x7FFFFFFFFFFFFFFFULL
#define MAX_INPUT_DIGITS 1002 

// 128-bit functions (keep existing)
int cmp(__uint128_t a, __uint128_t b);
void inc(__uint128_t *n);
void dec(__uint128_t *n);
void add(__uint128_t a, __uint128_t b, __uint128_t *result);
void sub(__uint128_t a, __uint128_t b, __uint128_t *result);
void mul(__uint128_t a, __uint128_t b, __uint128_t *result);

__uint128_t add_f(__uint128_t a, __uint128_t b);
__uint128_t sub_f(__uint128_t a, __uint128_t b);
__uint128_t mul_f(__uint128_t a, __uint128_t b);

// Big integer functions (now using __uint128_t)
void init_bigint(__uint128_t *digits, __uint128_t *length);
void mul_bigint_10(__uint128_t *digits, __uint128_t *length) ;
void parse_string_to_bigint(const char *str, __uint128_t *digits, __uint128_t *length);
void print_bigint(__uint128_t *digits, __uint128_t length, const char *name);
void bigint_multiply_ab();

#endif