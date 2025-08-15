# DNS - DeeezNuddS

Imma be honest that I'm speedrunning these after I woke up from my sleep.

## Sistem Operasi

Semua virtual machine dalam proyek ini menggunakan **Debian 13** dengan instalasi minimal, tanpa _desktop environment_ atau _window manager_.

Download di sini: https://www.debian.org/download

---

## Rincian dan Dependensi Tiap VM

Konfigurasi tiap VM dapat dilihat pada folder.
Video juga telah diupload.

### VM 1: DNS & DHCP Server

- **Dependensi:**
  - `bind9`: Perangkat lunak standar industri untuk server DNS.
  - `isc-dhcp-server`: Perangkat lunak untuk server DHCP.
  - `sudo`: Untuk manajemen hak akses.

### VM 2: HTTP Server

- **Peran:** Server tujuan yang menyimpan dan menyajikan konten website sederhana.
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
