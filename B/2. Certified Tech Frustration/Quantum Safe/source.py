import numpy as np
import random
import os
flag = b"HTB{contoh_flag_saja}" 
r = (5, 5, 5) 

def encrypt_flag(flag_text, pubkey_matrix, r_vector):
  """
  Mengenkripsi setiap karakter dari flag menggunakan kunci publik dan vektor r.
  
  Args:
    flag_text (bytes): Teks flag yang akan dienkripsi.
    pubkey_matrix (np.ndarray): Matriks kunci publik sebagai array NumPy.
    r_vector (np.ndarray): Vektor r sebagai array NumPy.
    
  Returns:
    list: Sebuah list berisi vektor-vektor hasil enkripsi.
  """
  encrypted_vectors = []
  
  # Iterasi melalui setiap byte (karakter) di dalam flag
  for char_code in flag_text:
    # Membuat vektor v dengan nilai acak, mirip seperti di kode Sage
    # ord(c) sudah digantikan dengan char_code
    v_plain = np.array([
      char_code,
      random.randint(0, 100),
      random.randint(0, 100)
    ])
    
    # Operasi utama: v * pubkey + r
    # Di NumPy, perkalian matriks menggunakan operator '@'
    v_encrypted = v_plain @ pubkey_matrix + r_vector
    
    encrypted_vectors.append(v_encrypted)
    
  return encrypted_vectors

if __name__ == "__main__":
  # Definisikan pubkey sebagai NumPy array
  pubkey = np.array([
    [47, -77, -85],
    [-49, 78, 50],
    [57, -78, 99]
  ], dtype=int)

  # Definisikan r sebagai NumPy array
  r_np = np.array(r, dtype=int)
  
  # Enkripsi flag
  encrypted_output = encrypt_flag(flag, pubkey, r_np)
  
  # Cetak hasilnya, satu vektor per baris
  print("Hasil Enkripsi:")
  for v in encrypted_output:
    # Mencetak dengan format yang mirip seperti output Sage
    print(f"({v[0]}, {v[1]}, {v[2]})")

