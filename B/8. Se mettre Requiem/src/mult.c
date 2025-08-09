#include "mult.h"

__uint128_t digits_a[MAX_LEN];
__uint128_t digits_b[MAX_LEN];
__uint128_t digits_result[MAX_LEN * 2];
__uint128_t length_a, length_b, length_result;
__uint128_t sign_a, sign_b, sign_result;

void inc(__uint128_t *n){
  __uint128_t mask = 1;

  increment_carry_loop:
  if (*n & mask){
    // Current bit is 1, flip to 0 and continue carry
    *n = *n ^ mask;
    mask = mask << 1;
    goto increment_carry_loop;
  }
  // Current bit is 0, flip to 1 and done
  *n = *n ^ mask;
}

void dec(__uint128_t *n) {
  __uint128_t mask = 1;

  decrement_borrow_loop:
  if (!(*n & mask)) {
    *n = *n ^ mask;
    mask = mask << 1;
    goto decrement_borrow_loop;
  }
  *n = *n ^ mask;
}

int cmp(__uint128_t a, __uint128_t b) {
  __uint128_t i = 127;
  
  loop:
  __uint128_t a_bit = (a >> i) & 1;
  __uint128_t b_bit = (b >> i) & 1;
  if (a_bit ^ b_bit) {
    if (a_bit) return 1;
    return ~0;
  }
  
  if (EQ(i, 0)) goto end;
  dec(&i);
  goto loop;

  end:
  return 0;
}


void add(__uint128_t a, __uint128_t b, __uint128_t *result) {
  *result = a ^ b;
  __uint128_t carry = a & b;
  
  add_loop:
  if (EQ(carry, 0)) goto add_end;
  
  carry = carry << 1;
  __uint128_t temp = *result;
  *result = *result ^ carry;
  carry = carry & temp;
  
  goto add_loop;
  
  add_end:
  return;
}

void sub(__uint128_t a, __uint128_t b, __uint128_t *result) {
  __uint128_t not_b = ~b;
  __uint128_t neg_b = 0;
  __uint128_t one = 1;
  add(not_b, one, &neg_b);
  add(a, neg_b, result);
}

void mul(__uint128_t a, __uint128_t b, __uint128_t *result) {
  *result = 0;
  
  mul_loop:
  if (EQ(a, 0)) goto mul_end;
  
  if (a & 1) {
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

__uint128_t add_f(__uint128_t a, __uint128_t b) {
  __uint128_t result;
  add(a, b, &result);
  return result;
}

__uint128_t sub_f(__uint128_t a, __uint128_t b) {
  __uint128_t result;
  sub(a, b, &result);
  return result;
}

__uint128_t mul_f(__uint128_t a, __uint128_t b) {
  __uint128_t result;
  mul(a, b, &result);
  return result;
}

void init_bigint(__uint128_t *digits, __uint128_t *length) {
  __uint128_t i = 0;
  
  init_loop:
  if (EQ(i, MAX_LEN)) goto init_done;
  digits[i] = 0;
  inc(&i);
  goto init_loop;
  
  init_done:
  *length = 1;
}

void mul_bigint_10(__uint128_t *digits, __uint128_t *length) {
  __uint128_t carry = 0;
  __uint128_t i = 0;
  
  multiply_loop:
  if (EQ(i, *length)) goto handle_final_carry;
  
  __uint128_t temp_product;
  mul(digits[i], 10, &temp_product);
  
  __uint128_t temp_sum;
  add(temp_product, carry, &temp_sum);
  
  carry = temp_sum >> BASE_BITS;
  digits[i] = temp_sum & MASK;
  
  inc(&i);
  goto multiply_loop;
  
  handle_final_carry:
  if (NE(carry, 0)) {
    digits[*length] = carry;
    inc(length);
  }
}

void parse_string_to_bigint(const char *str, __uint128_t *digits, __uint128_t *length) {
  init_bigint(digits, length);
  
  __uint128_t i = 0;
  
  parse_loop:
  if (EQ(str[i], 0)) goto parse_done;
  
  if (IS_DIGIT(str[i])) {
    mul_bigint_10(digits, length);
    
    __uint128_t digit = str[i] & 0x0F;
    __uint128_t temp;
    add(digits[0], digit, &temp);
    digits[0] = temp;
  }
  
  inc(&i);
  goto parse_loop;
  
  parse_done:
  return;
}

void print_bigint(__uint128_t *digits, __uint128_t length, const char *name) {
  printf("%s: [", name);
  __uint128_t i = 0;
  
  print_loop:
  if (EQ(i, length)) goto print_done;
  
  printf("%llu", (unsigned long long)digits[i]);
  
  __uint128_t next_i;
  add(i, 1, &next_i);
  if (NE(next_i, length)) {
    printf(", ");
  }
  
  inc(&i);
  goto print_loop;
  
  print_done:
  printf("], length=%llu\n", (unsigned long long)length);
}

void add_with_carry_propagation(__uint128_t *result_digits, __uint128_t *result_length, 
                                __uint128_t value, __uint128_t start_pos) {
  __uint128_t carry = value;
  __uint128_t pos = start_pos;
  
  carry_loop:
  if (EQ(carry, 0)) goto carry_done;
  
  __uint128_t sum;
  add(result_digits[pos], carry, &sum);
  
  carry = sum >> BASE_BITS;
  result_digits[pos] = sum & MASK;
  
  // Update length if we extended beyond current length
  __uint128_t new_pos;
  add(pos, 1, &new_pos);
  if (GE(new_pos, *result_length)) {
      *result_length = new_pos;
  }
  
  inc(&pos);
  goto carry_loop;
  
  carry_done:
  return;
}

void normalize_result_length(__uint128_t *result_digits, __uint128_t *result_length) {
  // Remove leading zeros, but keep at least 1 digit
  __uint128_t i = *result_length;
  
  normalize_loop:
  if (EQ(i, 1)) goto normalize_done;  // Keep at least 1 digit
  
  dec(&i);
  if (NE(result_digits[i], 0)) {
      inc(&i);  // Found non-zero, this is correct length
      goto normalize_done;
  }
  
  goto normalize_loop;
  
  normalize_done:
  *result_length = i;
}

void bigint_multiply_ab() {
  // Initialize result array to all zeros
  __uint128_t i = 0;
  init_result_loop:
  if (EQ(i, MAX_LEN * 2)) goto init_result_done;
  digits_result[i] = 0;
  inc(&i);
  goto init_result_loop;
  init_result_done:
  
  length_result = 1;
  
  // Main multiplication: for each digit in A
  __uint128_t i_a = 0;
  
  outer_loop:
  if (EQ(i_a, length_a)) goto multiply_done;
  
  // For each digit in B
  __uint128_t i_b = 0;
  
  inner_loop:
  if (EQ(i_b, length_b)) goto inner_done;
  
  // Multiply digits_a[i_a] × digits_b[i_b]
  __uint128_t product;
  mul(digits_a[i_a], digits_b[i_b], &product);
  
  // Add to result[i_a + i_b] with carry propagation
  __uint128_t pos;
  add(i_a, i_b, &pos);
  
  add_with_carry_propagation(digits_result, &length_result, product, pos);
  
  inc(&i_b);
  goto inner_loop;
  
  inner_done:
  inc(&i_a);
  goto outer_loop;
  
  multiply_done:
  // Clean up result (remove leading zeros)
  normalize_result_length(digits_result, &length_result);
}

int bigint_is_zero(__uint128_t *digits, __uint128_t length) {
  if (EQ(length, 1) & EQ(digits[0], 0)) {
    return 1;
  }
  return 0;
}

// Fungsi untuk membagi __uint128_t dengan 10 menggunakan bitwise long division
// Mengembalikan sisa dan menyimpan hasil bagi kembali ke *num
__uint128_t div_128_by_10(__uint128_t *num) {
  __uint128_t quotient = 0;
  __uint128_t remainder = 0;
  __uint128_t i = 128;

  div128_loop:
  if (EQ(i, 0)) goto div128_done;
  dec(&i);

  remainder = remainder << 1;
  __uint128_t bit = (*num >> i) & 1;
  remainder = remainder | bit;

  if (GE(remainder, 10)) {
    sub(remainder, 10, &remainder);
    __uint128_t one = 1;
    quotient = quotient | (one << i);
  }
  goto div128_loop;

  div128_done:
  *num = quotient;
  return remainder;
}


// Fungsi pembagian bigint dengan 10
__uint128_t bigint_div_10(__uint128_t *digits, __uint128_t *length) {
  __uint128_t remainder = 0;
  __uint128_t i = *length;
  
  main_div_loop_fast:
  if (EQ(i, 0)) goto main_div_done_fast;
  dec(&i);
  
  // Buat angka 128-bit dari sisa sebelumnya dan digit saat ini
  __uint128_t temp = remainder;
  temp = temp << BASE_BITS;
  add(temp, digits[i], &temp);
  
  // Bagi angka 128-bit ini dengan 10
  remainder = div_128_by_10(&temp);
  digits[i] = temp;
  
  goto main_div_loop_fast;
  
  main_div_done_fast:
  // Hapus nol di depan (normalisasi)
  __uint128_t len_minus_1;
  sub(*length, 1, &len_minus_1);
  if (GT(*length, 1) & EQ(digits[len_minus_1], 0)) {
    dec(length);
  }
  
  return remainder;
}


void print_bigint_base10(__uint128_t *digits, __uint128_t length) {
  // 55 * log10(2^63) kira-kira 1045
  char decimal_str[2091]; // gatau cok td segfault, pasang gedean aja. harusnya 2091 (+ null terminator) jg cukup
  __uint128_t str_len = 0;

  __uint128_t temp_digits[MAX_LEN * 2];
  __uint128_t temp_length = length;
  __uint128_t i = 0;

  // Cek jika angka adalah nol
  if (bigint_is_zero(digits, length)) {
    printf("Result (base 10): 0\n");
    return;
  }

  // Salin digits ke array sementara agar tidak merusak hasil asli
  copy_loop_fast:
  if (EQ(i, length)) goto copy_done_fast;
  temp_digits[i] = digits[i];
  inc(&i);
  goto copy_loop_fast;
  copy_done_fast:

  // Loop utama konversi: terus bagi dengan 10 sampai hasilnya 0
  conversion_loop_fast:
  if (bigint_is_zero(temp_digits, temp_length)) goto conversion_done_fast;
  
  __uint128_t remainder = bigint_div_10(temp_digits, &temp_length);
  decimal_str[str_len] = (char)(remainder) + '0';
  inc(&str_len);
  goto conversion_loop_fast;

  conversion_done_fast:
  
  // Cetak string desimal yang dihasilkan (secara terbalik)
  printf("Result (base 10): ");
  __uint128_t print_idx = str_len;

  print_reverse_loop_fast:
  if (EQ(print_idx, 0)) goto print_reverse_done_fast;
  dec(&print_idx);
  printf("%c", decimal_str[print_idx]);
  goto print_reverse_loop_fast;
  
  print_reverse_done_fast:
  printf("\n");
}

// Fungsi untuk membaca input besar dengan aman
void read_large_input(const char* prompt, char* buffer) {
  printf("%s", prompt);

  __uint128_t i = 0;
  int ch;

  input_loop:
  ch = getchar();
  // Berhenti jika bertemu newline, EOF, atau buffer hampir penuh
  if (EQ(ch, '\n') | EQ(ch, EOF) | GE(i, MAX_INPUT_DIGITS - 1)) {
    goto input_done;
  }

  buffer[i] = (char)ch;
  inc(&i);
  goto input_loop;

  input_done:
  buffer[i] = '\0';
}

int main() {
  char input_a[MAX_INPUT_DIGITS];
  char input_b[MAX_INPUT_DIGITS];

  // Gunakan fungsi pembaca input yang baru
  read_large_input("Masukkan bilangan pertama: ", input_a);
  read_large_input("Masukkan bilangan kedua: ", input_b);

  printf("\nMemulai perkalian...\n\n");

  // Parse string input menjadi format bigint
  parse_string_to_bigint(input_a, digits_a, &length_a);
  parse_string_to_bigint(input_b, digits_b, &length_b);

  // Lakukan perkalian
  bigint_multiply_ab();

  // Cetak hasilnya dalam basis 10
  print_bigint_base10(digits_result, length_result);

  return 0;
}