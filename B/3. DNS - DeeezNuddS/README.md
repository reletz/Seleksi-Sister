# DNS - DeeezNuddS

Imma be honest that I'm speedrunning these after I woke up from my sleep.

## Sistem Operasi

Semua virtual machine dalam proyek ini menggunakan **Debian 13** dengan instalasi minimal, tanpa _desktop environment_ atau _window manager_.

Download di sini: https://www.debian.org/download

---

## Langkah-Langkah Setup

> **Asumsi:** Semua dependensi sudah terinstal di setiap VM sesuai daftar di bawah. Jaringan VirtualBox diatur ke Internal Network untuk semua VM.

### VM 1: DNS & DHCP Server (IP: 192.168.55.1)

1. Salin file konfigurasi:
   - `interfaces` ke `/etc/network/`
   - `named.conf.local` dan `named.conf.options` ke `/etc/bind/`
   - `db.ngantuk.tech` ke `/etc/bind/`
   - `dhcpd.conf` ke `/etc/dhcp/`
   - `isc-dhcp-server` ke `/etc/default/`
2. Restart semua service:
   ```sh
   sudo systemctl restart networking
   sudo systemctl restart bind9
   sudo systemctl restart isc-dhcp-server
   ```

### VM 2: HTTP Server (IP: 192.168.55.2)

1. Salin file konfigurasi:
   - `interfaces` ke `/etc/network/`
   - `ports.conf` ke `/etc/apache2/`
   - `index.html` ke `/var/www/html/`
2. Restart service:
   ```sh
   sudo systemctl restart networking
   sudo systemctl restart apache2
   ```

### VM 4: Reverse Proxy & Firewall (IP: 192.168.55.4)

1. Salin file konfigurasi:
   - `interfaces` ke `/etc/network/`
   - `proxy.py` ke home directory (`~/`)
2. Jalankan perintah di `firewall_rules.txt` untuk konfigurasi UFW.
3. Restart jaringan:
   ```sh
   sudo systemctl restart networking
   ```
4. Jalankan proxy:
   ```sh
   sudo python3 ~/proxy.py
   ```

### VM 3: Client

1. Salin skrip `client.py` ke home directory (`~/`).
2. Pastikan NetworkManager mengelola koneksi (file `/etc/network/interfaces` hanya berisi konfigurasi `lo`).
3. Nyalakan terlebih dahulu jaringan. Contoh:
```sh
nmcli con up enp0s3
```
4. Jalankan skrip untuk konfigurasi jaringan dan tes:
   - Mode DHCP:
     ```sh
     sudo python3 client.py dhcp
     ```
   - Mode Statis:
     ```sh
     sudo python3 client.py static --ip 192.168.55.3
     ```

---

## Rincian dan Dependensi Tiap VM

Konfigurasi tiap VM dapat dilihat pada folder terkait. Video juga telah diupload.

### VM 1: DNS & DHCP Server

- **Dependensi:**
  - `bind9`: Perangkat lunak standar industri untuk server DNS.
  - `isc-dhcp-server`: Perangkat lunak untuk server DHCP.
  - `sudo`: Untuk manajemen hak akses.

### VM 2: HTTP Server

- **Dependensi:**
  - `apache2`: Perangkat lunak web server untuk menyajikan halaman.
  - `sudo`: Untuk manajemen hak akses.

### VM 3: Client

- **Dependensi:**
  - `python3` & `python3-requests`: Untuk menjalankan skrip client.
  - `network-manager`: Menyediakan tool `nmcli` yang digunakan skrip untuk otomasi jaringan.
  - `dnsutils`: Menyediakan tool `dig` untuk diagnostik DNS.
  - `sudo`: Untuk manajemen hak akses.

### VM 4: Reverse Proxy & Firewall

- **Dependensi:**
  - `python3` & `python3-pip`: Untuk menjalankan skrip reverse proxy.
  - `flask`: Framework Python yang digunakan untuk membuat program reverse proxy.
  - `ufw`: Perangkat lunak Uncomplicated Firewall untuk mengelola aturan keamanan.
  - `sudo`: Untuk manajemen hak akses.

---

## Bonus yang Dikerjakan

- **DHCP:** Mengonfigurasi server DHCP untuk memberikan alamat IP dan konfigurasi DNS secara otomatis kepada client (VM 3). Skrip client dibuat agar dapat mengatur mode jaringannya sendiri antara DHCP (otomatis) atau IP Statis (manual).
- **OS Tanpa GUI:** Seluruh implementasi menggunakan instalasi Debian server minimal (CLI-only) untuk efisiensi sumber daya.
