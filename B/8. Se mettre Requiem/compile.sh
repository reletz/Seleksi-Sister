#!/bin/bash

# Menentukan nama file sumber dan nama output
SOURCE_FILE="src/mult.c"
OUTPUT_NAME="bin/mult"

# Perintah untuk kompilasi
gcc -std=gnu99 -O2 -g -Wall -o "$OUTPUT_NAME" "$SOURCE_FILE"

# Periksa apakah kompilasi berhasil
if [ $? -eq 0 ]; then
  echo "Compile berhasil! Program '$OUTPUT_NAME' telah dibuat."
  echo "Untuk menjalankan, ketik: ./$OUTPUT_NAME"
else
  echo "Compile gagal."
fi