import numpy as np

def solve_crypto_challenge():
  """
  Mendekripsi flag dari serangkaian vektor terenkripsi dengan menemukan
  vektor offset 'r' dan membalikkan operasi matriks.
  """
  
  # Kunci publik yang diberikan dalam soal
  pubkey = np.array([
    [47, -77, -85],
    [-49, 78, 50],
    [57, -78, 99]
  ], dtype=float) # Gunakan float untuk perhitungan invers

  # Daftar vektor terenkripsi yang diberikan
  encrypted_vectors = np.array([
    [-981, 1395, -1668], [6934, -10059, 4270], [3871, -5475, 3976],
    [4462, -7368, -8954], [2794, -4413, -3461], [5175, -7518, 3201],
    [3102, -5051, -5457], [7255, -10884, -266], [5694, -8016, 6237],
    [4160, -6038, 2582], [4940, -7069, 3770], [3185, -5158, -4939],
    [7669, -11686, -2231], [5601, -9013, -7971], [5600, -8355, 575],
    [1739, -2838, -3037], [2572, -4120, -3788], [8055, -11985, 1137],
    [7088, -10247, 5141], [8384, -12679, -1381], [-785, 1095, -1841],
    [4250, -6762, -5242], [3716, -5364, 2126], [5673, -7968, 6741],
    [5877, -9190, -4803], [5639, -8865, -5356], [1980, -3230, -3366],
    [6183, -9334, -1002], [2575, -4068, -2828], [7521, -11374, -1137],
    [5639, -8551, -1501], [4194, -6039, 3213], [2072, -3025, 383],
    [2444, -3699, -502], [6313, -9653, -2447], [4502, -7090, -4435],
    [-421, 894, 2912], [4667, -7142, -2266], [4228, -6616, -3749],
    [6258, -9719, -4407], [6044, -9561, -6463], [266, -423, -637],
    [3849, -6223, -5988], [5809, -9021, -4115], [4794, -7128, 918],
    [6340, -9442, 892], [5322, -8614, -8334]
  ], dtype=float)

  # === Langkah 1: Estimasi vektor 'r' ===
  # Logikanya: v_enkripsi = (v_plain * pubkey) + r
  # Rata-rata(v_enkripsi) = (Rata-rata(v_plain) * pubkey) + r
  # Jadi, r = Rata-rata(v_enkripsi) - (Rata-rata(v_plain) * pubkey)

  # Rata-rata dari angka acak antara 0 dan 100 adalah 50.
  avg_random = 50
  # Rata-rata dari nilai ASCII untuk karakter yang bisa dicetak adalah sekitar 80.
  avg_char_ord = 80
  
  # Buat vektor plaintext rata-rata
  avg_plain_vector = np.array([avg_char_ord, avg_random, avg_random])
  
  # Hitung rata-rata dari semua vektor terenkripsi
  avg_encrypted_vector = np.mean(encrypted_vectors, axis=0)
  
  # Estimasi 'r' dengan membulatkannya ke integer terdekat
  estimated_r = np.round(avg_encrypted_vector - (avg_plain_vector @ pubkey))
  print(f"[*] Vektor 'r' yang berhasil diestimasi: {estimated_r.astype(int)}\n")

  # === Langkah 2: Dekripsi setiap vektor ===
  # v_plain = (v_enkripsi - r) * invers(pubkey)
  
  try:
    # Hitung invers dari matriks kunci publik
    pubkey_inverse = np.linalg.inv(pubkey)
  except np.linalg.LinAlgError:
    print("[!] Error: Matriks kunci publik tidak bisa diinvers.")
    return

  decrypted_flag = ""
  print("Proses Dekripsi:")
  for i, v_enc in enumerate(encrypted_vectors):
    # Kurangi vektor terenkripsi dengan 'r' yang sudah kita temukan
    v_shifted = v_enc - estimated_r
    
    # Kalikan dengan invers untuk mendapatkan vektor plaintext asli
    v_plain = v_shifted @ pubkey_inverse
    
    # Komponen pertama dari v_plain adalah nilai ASCII dari karakter flag
    char_code = int(round(v_plain[0]))
    
    # Konversi kode ASCII kembali menjadi karakter
    decrypted_flag += chr(char_code)
    
    # (Opsional) Tampilkan hasil per karakter untuk verifikasi
    # print(f"  Vektor {i+1}: Ditemukan char '{chr(char_code)}' (ASCII: {char_code}), Noise: {round(v_plain[1], 1)}, {round(v_plain[2], 1)}")

  return decrypted_flag

if __name__ == "__main__":
  flag = solve_crypto_challenge()
  if flag:
    print("\n[+] Flag berhasil didekripsi!")
    print(f"    {flag}")

