import subprocess
import argparse
import requests
import time
import sys
import re

CONNECTION_NAME = "enp0s3"
DNS_SERVER = "192.168.55.1"
TARGET_DOMAIN = "ngantuk.tech"
TARGET_URL = f"http://{TARGET_DOMAIN}"

def run_command(command):
    print(f"Executing: {command}")
    try:
        result = subprocess.run(
            command,
            shell=True,
            check=True,
            capture_output=True,
            text=True
        )
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error saat menjalankan perintah.")
        print(f"   Perintah: {e.cmd}")
        print(f"   Return Code: {e.returncode}")
        print(f"   Error Output: {e.stderr.strip()}")
        return False
    except FileNotFoundError:
        print(f"Error: Perintah 'nmcli' tidak ditemukan. Pastikan NetworkManager terinstal.")
        return False

def get_ip():
    try:
        result = subprocess.run(['ip', '-4', 'addr', 'show', CONNECTION_NAME],capture_output=True, text = True, check = True)
        match = re.search(r'inet (\d{1,3}\.\d{1,3}.\d{1,3}.\d{1,3})', result.stdout)
        if match:
            ip_addr = match.group(1)
            print(f"Your IP now is {ip_addr}")
        else:
            print("Cannot retrieve your ip")
    except Exception as e:
        print(f"Error: {e}")

def set_static(ip_address):
    print("\nMengatur jaringan ke mode STATIS...")
    cmd = (
        f'nmcli con mod "{CONNECTION_NAME}" '
	f'ipv4.method manual '
        f'ipv4.addresses {ip_address}/24 '
        f'ipv4.dns "{DNS_SERVER}" '
    )
    if not run_command(cmd):
    	return False
    print(f"Berhasil mengatur IP statis ke {ip_address}")
    return True

def set_dhcp():
    print("\nMengatur jaringan ke mode DHCP...")
    command = f'nmcli con mod "{CONNECTION_NAME}" ipv4.method auto'
    if not run_command(command):
        return False
    get_ip()
    return True

def restart_network():
    print("\nMerestart koneksi jaringan...")
    command = f'nmcli con down "{CONNECTION_NAME}" && nmcli con up "{CONNECTION_NAME}"'
    if not run_command(command):
        print("Gagal merestart jaringan. Mungkin perlu beberapa saat.")
        return False
    
    print("... Menunggu jaringan stabil ...")
    time.sleep(5)
    print("Jaringan berhasil direstart.")
    return True

def test_connection():
    print(f"\nMenguji koneksi ke {TARGET_URL}...")
    try:
        response = requests.get(TARGET_URL, timeout=10)
        response.raise_for_status()
        print("KONEKSI BERHASIL!")
        print("--- Isi Website ---")
        print(response.text.strip())
        print("-------------------")
    except requests.exceptions.RequestException as e:
        print(f"KONEKSI GAGAL!")
        print(f"   Error: {e}")
        print("   Pastikan semua VM (DNS, Proxy, Web Server) berjalan dengan benar.")

def main():
    parser = argparse.ArgumentParser(description="Client untuk mengonfigurasi jaringan dan menguji koneksi.")
    subparsers = parser.add_subparsers(dest="command", required=True, help="Pilih mode: 'dhcp' atau 'static'")

    subparsers.add_parser("dhcp", help="Atur jaringan untuk menggunakan DHCP.")

    static_parser = subparsers.add_parser("static", help="Atur jaringan untuk menggunakan IP statis.")
    static_parser.add_argument("--ip", type=str, required=True, help="Alamat IP statis yang akan digunakan (contoh: 192.168.55.3)")

    args = parser.parse_args()

    config_success = False
    if args.command == "dhcp":
        config_success = set_dhcp()
    elif args.command == "static":
        config_success = set_static(args.ip)

    if config_success:
        if restart_network():
            test_connection()
    else:
        print("\nKonfigurasi jaringan gagal, pengujian koneksi dibatalkan.")

if __name__ == "__main__":
    if subprocess.os.getuid() != 0:
        print("Skrip ini perlu dijalankan dengan hak akses root. Coba jalankan dengan 'sudo python3 client.py ...'")
        sys.exit(1)
    main()
