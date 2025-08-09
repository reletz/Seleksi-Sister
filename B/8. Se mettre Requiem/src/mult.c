#include "mult.h"

#define NTT_PRIME 18446744069414584321ULL
#define NTT_GENERATOR 7

__uint128_t digits_a[MAX_LEN];
__uint128_t digits_b[MAX_LEN];
__uint128_t digits_result[MAX_LEN * 2];
__uint128_t length_a, length_b, length_result;
__uint128_t sign_a, sign_b, sign_result;

void inc(__uint128_t *n)
{
  __uint128_t mask = 1;

increment_carry_loop:
  if (*n & mask)
  {
    *n = *n ^ mask;
    mask = mask << 1;
    goto increment_carry_loop;
  }
  *n = *n ^ mask;
}

void dec(__uint128_t *n)
{
  __uint128_t mask = 1;

decrement_borrow_loop:
  if (!(*n & mask))
  {
    *n = *n ^ mask;
    mask = mask << 1;
    goto decrement_borrow_loop;
  }
  *n = *n ^ mask;
}

int cmp(__uint128_t a, __uint128_t b)
{
  __uint128_t i = 127;

loop:
  __uint128_t a_bit = (a >> i) & 1;
  __uint128_t b_bit = (b >> i) & 1;
  if (a_bit ^ b_bit)
  {
    if (a_bit)
      return 1;
    return ~0;
  }

  if (EQ(i, 0))
    goto end;
  dec(&i);
  goto loop;

end:
  return 0;
}

void add(__uint128_t a, __uint128_t b, __uint128_t *result)
{
  *result = a ^ b;
  __uint128_t carry = a & b;

add_loop:
  if (EQ(carry, 0))
    goto add_end;

  carry = carry << 1;
  __uint128_t temp = *result;
  *result = *result ^ carry;
  carry = carry & temp;

  goto add_loop;

add_end:
  return;
}

void sub(__uint128_t a, __uint128_t b, __uint128_t *result)
{
  __uint128_t not_b = ~b;
  __uint128_t neg_b = 0;
  __uint128_t one = 1;
  add(not_b, one, &neg_b);
  add(a, neg_b, result);
}

void mul(__uint128_t a, __uint128_t b, __uint128_t *result)
{
  *result = 0;

mul_loop:
  if (EQ(a, 0))
    goto mul_end;

  if (a & 1)
  {
    __uint128_t temp;
    add(*result, b, &temp);
    *result = temp;
  }

  a = a >> 1;
  b = b << 1;
  goto mul_loop;

mul_end:
  return;
}

__uint128_t add_f(__uint128_t a, __uint128_t b)
{
  __uint128_t result;
  add(a, b, &result);
  return result;
}

__uint128_t sub_f(__uint128_t a, __uint128_t b)
{
  __uint128_t result;
  sub(a, b, &result);
  return result;
}

__uint128_t mul_f(__uint128_t a, __uint128_t b)
{
  __uint128_t result;
  mul(a, b, &result);
  return result;
}

void init_bigint(__uint128_t *digits, __uint128_t *length)
{
  __uint128_t i = 0;

init_loop:
  if (EQ(i, MAX_LEN))
    goto init_done;
  digits[i] = 0;
  inc(&i);
  goto init_loop;

init_done:
  *length = 1;
}

void mul_bigint_10(__uint128_t *digits, __uint128_t *length)
{
  __uint128_t carry = 0;
  __uint128_t i = 0;

multiply_loop:
  if (EQ(i, *length))
    goto handle_final_carry;

  __uint128_t temp_product;
  mul(digits[i], 10, &temp_product);

  __uint128_t temp_sum;
  add(temp_product, carry, &temp_sum);

  carry = temp_sum >> BASE_BITS;
  digits[i] = temp_sum & MASK;

  inc(&i);
  goto multiply_loop;

handle_final_carry:
  if (NE(carry, 0))
  {
    digits[*length] = carry;
    inc(length);
  }
}

void parse_string_to_bigint(const char *str, __uint128_t *digits, __uint128_t *length)
{
  init_bigint(digits, length);

  __uint128_t i = 0;

parse_loop:
  if (EQ(str[i], 0))
    goto parse_done;

  if (IS_DIGIT(str[i]))
  {
    mul_bigint_10(digits, length);

    __uint128_t digit = str[i] & 0x0F;
    add_with_carry_propagation(digits, length, digit, 0);
  }

  inc(&i);
  goto parse_loop;

parse_done:
  return;
}

void print_bigint(__uint128_t *digits, __uint128_t length, const char *name)
{
  printf("%s: [", name);
  __uint128_t i = 0;

print_loop:
  if (EQ(i, length))
    goto print_done;

  printf("%llu", (unsigned long long)digits[i]);

  __uint128_t next_i;
  add(i, 1, &next_i);
  if (NE(next_i, length))
  {
    printf(", ");
  }

  inc(&i);
  goto print_loop;

print_done:
  printf("], length=%llu\n", (unsigned long long)length);
}

void add_with_carry_propagation(__uint128_t *result_digits, __uint128_t *result_length,
                                __uint128_t value, __uint128_t start_pos)
{
  __uint128_t carry = value;
  __uint128_t pos = start_pos;

carry_loop:
  if (EQ(carry, 0))
    goto carry_done;

  if (GE(pos, *result_length))
  {
    if (GE(pos, MAX_LEN * 2))
      goto carry_done;

    *result_length = pos + 1;
    result_digits[pos] = 0;
  }

  __uint128_t sum;
  add(result_digits[pos], carry, &sum);

  carry = sum >> BASE_BITS;
  result_digits[pos] = sum & MASK;

  inc(&pos);
  goto carry_loop;

carry_done:
  return;
}

void normalize_result_length(__uint128_t *result_digits, __uint128_t *result_length)
{
  __uint128_t i = *result_length;

normalize_loop:
  if (EQ(i, 1))
    goto normalize_done;

  dec(&i);
  if (NE(result_digits[i], 0))
  {
    inc(&i);
    goto normalize_done;
  }

  goto normalize_loop;

normalize_done:
  *result_length = i;
}

void bigint_multiply_ab()
{
  __uint128_t i = 0;
init_result_loop:
  if (EQ(i, MAX_LEN * 2))
    goto init_result_done;
  digits_result[i] = 0;
  inc(&i);
  goto init_result_loop;
init_result_done:

  length_result = 1;

  __uint128_t i_a = 0;

outer_loop:
  if (EQ(i_a, length_a))
    goto multiply_done;

  __uint128_t i_b = 0;

inner_loop:
  if (EQ(i_b, length_b))
    goto inner_done;

  __uint128_t product;
  mul(digits_a[i_a], digits_b[i_b], &product);

  __uint128_t pos;
  add(i_a, i_b, &pos);

  add_with_carry_propagation(digits_result, &length_result, product, pos);

  inc(&i_b);
  goto inner_loop;

inner_done:
  inc(&i_a);
  goto outer_loop;

multiply_done:
  normalize_result_length(digits_result, &length_result);
}

int bigint_is_zero(__uint128_t *digits, __uint128_t length)
{
  return (EQ(length, 1) & EQ(digits[0], 0));
}

__uint128_t div_bigint_10(__uint128_t *digits, __uint128_t *length)
{
  __uint128_t remainder = 0;
  __uint128_t i = *length;

main_div_loop:
  if (EQ(i, 0))
    goto main_div_done;
  dec(&i);

  __uint128_t temp = remainder;
  temp = temp << BASE_BITS;
  add(temp, digits[i], &temp);

  remainder = div_128_by_mod(&temp, 10);
  digits[i] = temp;

  goto main_div_loop;

main_div_done:
  __uint128_t len_minus_1;
  sub(*length, 1, &len_minus_1);
  if (GT(*length, 1) & EQ(digits[len_minus_1], 0))
  {
    dec(length);
  }

  return remainder;
}

__uint128_t div_128_by_mod(__uint128_t *num, __uint128_t mod)
{
  __uint128_t quotient = 0;
  __uint128_t remainder = 0;
  __uint128_t i = 128;

div128_loop_mod:
  if (EQ(i, 0))
    goto div128_done_mod;
  dec(&i);

  remainder = remainder << 1;
  __uint128_t bit = (*num >> i) & 1;
  remainder = remainder | bit;

  if (GE(remainder, mod))
  {
    sub(remainder, mod, &remainder);
    __uint128_t one = 1;
    quotient = quotient | (one << i);
  }
  goto div128_loop_mod;

div128_done_mod:
  *num = quotient;
  return remainder;
}

__uint128_t bigint_mod(__uint128_t *digits, __uint128_t length, __uint128_t mod)
{
  __uint128_t temp_digits[MAX_LEN * 2];
  __uint128_t i_copy = 0;
copy_loop_mod:
  if (EQ(i_copy, length))
    goto copy_done_mod;
  temp_digits[i_copy] = digits[i_copy];
  inc(&i_copy);
  goto copy_loop_mod;
copy_done_mod:

  __uint128_t remainder = 0;
  __uint128_t i = length;

main_div_loop_mod:
  if (EQ(i, 0))
    goto main_div_done_mod;
  dec(&i);

  __uint128_t temp = remainder;
  temp = temp << BASE_BITS;
  add(temp, temp_digits[i], &temp);

  remainder = div_128_by_mod(&temp, mod);

  goto main_div_loop_mod;

main_div_done_mod:
  return remainder;
}

__uint128_t power(__uint128_t base, __uint128_t exp, __uint128_t mod)
{
  __uint128_t res = 1;
  __uint128_t temp_base = base;
  base = div_128_by_mod(&temp_base, mod);

power_loop:
  if (EQ(exp, 0))
    goto power_done;

  if (exp & 1)
  {
    __uint128_t temp_res;
    mul(res, base, &temp_res);
    res = div_128_by_mod(&temp_res, mod);
  }

  exp >>= 1;
  temp_base = base;
  mul(temp_base, temp_base, &temp_base);
  base = div_128_by_mod(&temp_base, mod);

  goto power_loop;

power_done:
  return res;
}

__uint128_t inverse(__uint128_t n, __uint128_t mod)
{
  __uint128_t exp;
  sub(mod, 2, &exp);
  return power(n, exp, mod);
}

void swap_elements(__uint128_t *a, __uint128_t *b)
{
  __uint128_t temp = *a;
  *a = *b;
  *b = temp;
}

void bit_reversal_permutation(__uint128_t *a, __uint128_t n)
{
  __uint128_t i = 1;
  __uint128_t j = 0;

main_permute_loop:
  if (GE(i, n))
    goto main_permute_done;

  __uint128_t bit = n >> 1;

inner_bit_loop:
  if (j & bit)
  {
    j = j ^ bit;
  }
  else
  {
    j = j | bit;
    goto inner_bit_done;
  }
  bit = bit >> 1;
  goto inner_bit_loop;
inner_bit_done:

  if (LT(i, j))
  {
    swap_elements(&a[i], &a[j]);
  }

  inc(&i);
  goto main_permute_loop;

main_permute_done:
  return;
}

void print_bigint_base10(__uint128_t *digits, __uint128_t length)
{
  char decimal_str[2000002];
  __uint128_t str_len = 0;

  __uint128_t temp_digits[MAX_LEN * 2];
  __uint128_t temp_length = length;
  __uint128_t i = 0;

  if (bigint_is_zero(digits, length))
  {
    printf("Result (base 10): 0\n");
    return;
  }

copy_loop:
  if (EQ(i, length))
    goto copy_done;
  temp_digits[i] = digits[i];
  inc(&i);
  goto copy_loop;
copy_done:

conversion_loop:
  if (bigint_is_zero(temp_digits, temp_length))
    goto conversion_done;

  __uint128_t remainder = div_bigint_10(temp_digits, &temp_length);
  decimal_str[str_len] = (char)(remainder) + '0';
  inc(&str_len);
  goto conversion_loop;

conversion_done:

  printf("Result (base 10): ");
  __uint128_t print_idx = str_len;

print_reverse_loop:
  if (EQ(print_idx, 0))
    goto print_reverse_done;
  dec(&print_idx);
  printf("%c", decimal_str[print_idx]);
  goto print_reverse_loop;

print_reverse_done:
  printf("\n");
}

void ntt(__uint128_t *a, __uint128_t n, int invert_flag)
{
  __uint128_t M = NTT_PRIME;
  __uint128_t G = NTT_GENERATOR;
  __uint128_t i, j, len, wlen, w, u, v;
  bit_reversal_permutation(a, n);
  __uint128_t mod_minus_1;
  sub(M, 1, &mod_minus_1);

  len = 2;
main_ntt_loop:
  if (GT(len, n))
    goto main_ntt_done;

  __uint128_t ang = mod_minus_1;
  __uint128_t temp_len = len;
  div_128_by_mod(&ang, temp_len);
  wlen = power(G, ang, M);
  if (invert_flag)
    wlen = inverse(wlen, M);

  i = 0;
block_loop:
  if (GE(i, n))
    goto block_done;
  w = 1;
  j = 0;
butterfly_loop:
  if (GE(j, len >> 1))
    goto butterfly_done;
  __uint128_t idx1, idx2;
  add(i, j, &idx1);
  add(idx1, (len >> 1), &idx2);
  u = a[idx1];
  mul(a[idx2], w, &v);
  v = div_128_by_mod(&v, M);

  add(u, v, &a[idx1]);
  if (GE(a[idx1], M))
    sub(a[idx1], M, &a[idx1]);

  if (LT(u, v))
  {
    __uint128_t diff;
    sub(v, u, &diff);
    sub(M, diff, &a[idx2]);
  }
  else
  {
    sub(u, v, &a[idx2]);
  }

  mul(w, wlen, &w);
  w = div_128_by_mod(&w, M);
  inc(&j);
  goto butterfly_loop;
butterfly_done:
  add(i, len, &i);
  goto block_loop;
block_done:
  len <<= 1;
  goto main_ntt_loop;
main_ntt_done:

  if (invert_flag)
  {
    __uint128_t n_inv = inverse(n, M);
    i = 0;
  normalize_loop:
    if (EQ(i, n))
      goto normalize_done;
    mul(a[i], n_inv, &a[i]);
    a[i] = div_128_by_mod(&a[i], M);
    inc(&i);
    goto normalize_loop;
  normalize_done:;
  }
}

void multiply_ntt()
{
  __uint128_t M = NTT_PRIME;
  __uint128_t i;
  __uint128_t n = 1;
  __uint128_t combined_len;
  add(length_a, length_b, &combined_len);
size_loop:
  if (LT(n, combined_len))
  {
    n <<= 1;
    goto size_loop;
  }

  static __uint128_t fa[131072];
  static __uint128_t fb[131072];

  i = 0;
pad_a_loop:
  if (GE(i, n))
    goto pad_a_done;
  int temp = LT(i, length_a);
  if (temp) fa[i] = digits_a[i];
  if (!temp) fa[i] = 0;
  inc(&i);
  goto pad_a_loop;
pad_a_done:
  i = 0;
pad_b_loop:
  if (GE(i, n))
    goto pad_b_done;
  temp = LT(i, length_b);
  if (temp) fb[i] = digits_b[i];
  if (!temp) fb[i] = 0;
  inc(&i);
  goto pad_b_loop;
pad_b_done:

  ntt(fa, n, 0);
  ntt(fb, n, 0);

  i = 0;
pointwise_loop:
  if (EQ(i, n))
    goto pointwise_done;

  mul(fa[i], fb[i], &fa[i]);
  fa[i] = div_128_by_mod(&fa[i], M);

  inc(&i);
  goto pointwise_loop;
pointwise_done:

  ntt(fa, n, 1);

  __uint128_t carry = 0;
  i = 0;
final_carry_loop:
  if (GE(i, n))
    goto final_carry_done;

  __uint128_t current_val;
  add(fa[i], carry, &current_val);

  __uint128_t temp_for_division = current_val;
  __uint128_t remainder = div_128_by_mod(&temp_for_division, BASE);

  digits_result[i] = remainder;
  carry = temp_for_division;

  inc(&i);
  goto final_carry_loop;
final_carry_done:

  length_result = n;
  normalize_result_length(digits_result, &length_result);
}

void read_large_input(const char *prompt, char *buffer)
{
  printf("%s", prompt);

  __uint128_t i = 0;
  int ch;

input_loop:
  ch = getchar();
  __uint128_t limit = MAX_INPUT_DIGITS;
  dec(&limit);
  if (EQ(ch, '\n') | EQ(ch, EOF) | GE(i, limit))
  {
    goto input_done;
  }

  buffer[i] = (char)ch;
  inc(&i);
  goto input_loop;

input_done:
  buffer[i] = '\0';
}

int main()
{
  char input_a[MAX_INPUT_DIGITS];
  char input_b[MAX_INPUT_DIGITS];

  read_large_input("Masukkan bilangan pertama: ", input_a);
  read_large_input("Masukkan bilangan kedua: ", input_b);

  printf("\nMemulai perkalian...\n\n");

  parse_string_to_bigint(input_a, digits_a, &length_a);
  parse_string_to_bigint(input_b, digits_b, &length_b);

  bigint_multiply_ab();

  print_bigint_base10(digits_result, length_result);

  return 0;
}