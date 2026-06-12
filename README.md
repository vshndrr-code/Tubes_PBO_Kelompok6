# 🏦 Bank Loopy - ATM Banking System

> **Tugas Besar Praktik Pemrograman Berorientasi Objek (PBO)**  
> **Kelompok 6**

Bank Loopy adalah aplikasi **Automatic Teller Machine (ATM)** berbasis console yang dibangun menggunakan bahasa **C++** dengan menerapkan konsep **Pemrograman Berorientasi Objek (OOP)** secara komprehensif.

---

## 📋 Daftar Anggota Kelompok

| No  | Nama                     | NIM       |
| --- | ------------------------ | --------- |
| 1   | Yazri Khoiri Matondang   | 251402016 |
| 2   | Muhammad Vasha Nadar     | 251402019 |
| 3   | Dian Indriani            | 251402040 |
| 4   | Indah Ayu Gemilang       | 251402087 |
| 5   | Keizya Azalea Azka       | 251402131 |
| 6   | ⁠Quinsha Ilmi Azzahra     | 251402137 |

---

## ✨ Fitur Aplikasi

| Fitur                        | Keterangan                                                                    |
| ---------------------------- | ----------------------------------------------------------------------------- |
| 🔐 **Login**                 | Autentikasi nasabah dengan **nomor rekening + PIN 6 digit**, maksimal 3x   |
| 📝 **Registrasi**            | Pendaftaran akun baru (NIK 16 digit, nama, telepon, PIN)                     |
| 💰 **Cek Saldo**             | Menampilkan sisa saldo rekening nasabah                                       |
| 💵 **Setor Tunai**           | Menyetorkan uang tunai (kelipatan **Rp 50.000**)                             |
| 🏧 **Tarik Tunai**           | Menarik uang tunai dengan nominal **preset** atau **kustom** (kelipatan Rp 50.000, saldo minimal Rp 50.000) |
| 🔄 **Transfer Dana**         | Transfer ke **Bank Loopy (sesama)** atau **bank lain** (BCA, Mandiri, BRI, BNI, CIMB Niaga, Bank Sumut, Danamon) dengan biaya admin **Rp 2.500** untuk transfer antar bank |
| 📊 **Mutasi Rekening**       | Riwayat transaksi nasabah berdasarkan nomor rekening                          |
| 🔑 **Ubah PIN**              | Mengganti PIN nasabah dengan verifikasi PIN lama + konfirmasi PIN baru        |
| ℹ️ **Informasi Akun**        | Menampilkan detail data diri nasabah                                          |
| 🧾 **Struk Transaksi**       | Cetak struk otomatis setelah setiap transaksi sukses                          |

---

## 🧱 Konsep OOP yang Diterapkan

### 1. 🏛️ **Class & Object**
Setiap entitas dalam program direpresentasikan sebagai class:
- `Akun` — data nasabah
- `SistemBank` — controller utama ATM
- `AuthManager` — manajemen autentikasi
- `DatabaseManager` — manajemen penyimpanan data
- `LogManager` — pencatatan log transaksi
- `TransactionManager` — pemrosesan transaksi
- `Transaksi`, `Setor`, `Tarik` — hierarki transaksi

### 2. 🔒 **Encapsulation**
Data dalam class `Akun` bersifat **private** dan hanya dapat diakses melalui **getter/setter**:
```cpp
// Akun.h
private:
    string nama;
    string nik;
    string pin;
    long long saldo;
    string noRek;
    string telepon;

public:
    string getNama() const;
    void setSaldo(long long s);
    void setPin(string p);
    // ...
```

### 3. 🧬 **Inheritance**
Class `Setor` dan `Tarik` mewarisi class abstract `Transaksi`:
```cpp
class Transaksi {                    // Base class (abstract)
    virtual void eksekusi(long long&) = 0;
};

class Setor : public Transaksi { };  // Derived class
class Tarik : public Transaksi { };  // Derived class
```

### 4. 🔄 **Polymorphism**
Method `eksekusi()` dipanggil secara polimorfik melalui pointer base class dengan alokasi memori dinamis:
```cpp
// TransactionManager.h
Transaksi* tx = new Setor(jumlah);
tx->eksekusi(saldoSementara);  // Memanggil method override Setor
delete tx;
```

### 5. 🎭 **Abstraction**
Class `Transaksi` merupakan **abstract class** dengan **pure virtual function**:
```cpp
class Transaksi {
protected:
    long long jumlah;
public:
    virtual void eksekusi(long long &saldo) = 0;  // Pure virtual
    virtual ~Transaksi() {}
};
```

### 6. ➕ **Operator Overloading**
Operator `+=` dan `-=` di-overload pada class `Akun` untuk manipulasi saldo:
```cpp
void operator += (long long jumlah);  // Setor
void operator -= (long long jumlah);  // Tarik / Transfer (dengan proteksi saldo minimal)
```

### 7. 🔧 **Method Overloading**
Method `tampilkanData()` memiliki 3 versi berbeda:
```cpp
void tampilkanData();                                           // Tanpa parameter
void tampilkanData(bool denganSaldo);                           // Dengan opsi saldo
void tampilkanData(string jenisTx, long long nominal,           // Cetak struk
                    string infoTambahan = "", long long admBank = 0);
```

### 8. 📐 **Template**
Fungsi template untuk validasi input dengan tipe data fleksibel:
```cpp
template <typename T>
T dapatkanInput(const string& prompt);

template <typename T>
T dapatkanInputBatas(const string& prompt, T minVal, T maxVal);
```

### 9. 💾 **File Handling**
Data nasabah dan log transaksi disimpan dalam **file teks**:
- `output/database_bank.txt` — database akun nasabah
- `output/laporan_keuangan.txt` — log / mutasi transaksi

### 10. 📦 **Composition**
Class `SistemBank` memiliki objek anggota (composition) dari class lain:
```cpp
class SistemBank {
private:
    DatabaseManager db;
    LogManager logManager;
    AuthManager auth;
    TransactionManager txManager;
    // ...
};
```

### 11. 🗂️ **Vector (STL)**
Data akun nasabah disimpan dalam **`vector<Akun>`** yang memungkinkan pengelolaan data dinamis.

### 12. 🧹 **Dynamic Memory Allocation**
Alokasi dan dealokasi memori dinamis untuk objek transaksi:
```cpp
Transaksi* tx = new Setor(jumlah);  // Alokasi
// ...
delete tx;                           // Dealokasi
```

---

## 🗂️ Struktur File

```
📁 tubes pbo/
├── main.cpp                      # Entry point program
├── SistemBank.h                  # Class utama (controller ATM)
├── Akun.h                        # Class nasabah
├── AuthManager.h                 # Class autentikasi (login, registrasi, ubah PIN)
├── DatabaseManager.h             # Class manajemen database (file I/O)
├── LogManager.h                  # Class pencatatan log transaksi
├── TransactionManager.h          # Class pemrosesan transaksi
├── Transaksi.h                   # Abstract class transaksi + derived class Setor/Tarik
├── Helper.h                      # Fungsi utilitas (template input, format rupiah)
├── output/
│   ├── database_bank.txt         # Database akun nasabah
│   └── laporan_keuangan.txt      # Log transaksi keuangan
└── README.md                     # Dokumentasi proyek
```

---

## ⚙️ Cara Menjalankan

### Prasyarat
- Compiler C++ yang mendukung **C++11** atau lebih baru (contoh: MinGW g++, MSVC, GCC, Clang)

### Langkah-langkah

1. **Clone repositori ini:**
   ```bash
   git clone https://github.com/vshndrr-code/Tubes_PBO_Kelompok6.git
   cd Tubes_PBO_Kelompok6
   ```

2. **Kompilasi program:**
   ```bash
   g++ main.cpp -o bank_loopy.exe
   ```
   > **Catatan:** Program ini menggunakan header `<conio.h>` yang spesifik untuk Windows. Untuk sistem operasi lain, diperlukan penyesuaian.

3. **Jalankan program:**
   ```bash
   ./bank_loopy.exe
   ```

---

## 🖥️ Tampilan Program

### Menu Selamat Datang
```
===================================
            BANK LOOPY
       SELAMAT DATANG DI ATM
===================================

  [1] LOGIN (SUDAH PUNYA AKUN)
  [2] REGISTRASI (BUAT AKUN BARU)
  [3] KELUAR APLIKASI

===================================
  PILIH OPSI (1-3):
```

### Menu Utama
```
=================================================================
                           MENU UTAMA
=================================================================

  <--- [1] PENARIKAN TUNAI              [5] MUTASI REKENING  --->

  <--- [2] SETORAN TUNAI                [6] UBAH PIN         --->

  <--- [3] CEK SALDO                    [7] INFORMASI AKUN   --->

  <--- [4] TRANSFER DANA                [8] LOGOUT / KELUAR  --->
```

---

## 📌 Aturan Bisnis

| Aturan                              | Nilai              |
| ----------------------------------- | ------------------ |
| 💳 **Digit PIN**                    | 6 digit angka      |
| 🆔 **Digit NIK**                    | 16 digit angka     |
| 🔢 **Digit Nomor Rekening**         | 10 digit (generated)|
| 📞 **Minimal digit telepon**        | 11 digit           |
| 🔐 **Maksimal percobaan login**     | 3 kali             |
| 💵 **Kelipatan setor/tarik**        | Rp 50.000          |
| 💰 **Saldo minimal**                | Rp 50.000          |
| 🏦 **Biaya admin transfer antar bank**| Rp 2.500          |

---

## 🛠️ Tech Stack

- **Bahasa:** C++ (C++11 / C++14)
- **Penyimpanan:** File teks (`.txt`)
- **Platform:** Windows (menggunakan `conio.h`, `COLOR`)

---

## 📄 Lisensi

Proyek ini dibuat untuk memenuhi tugas **Praktik Pemrograman Berorientasi Objek** dan bersifat **open-source** untuk keperluan pembelajaran.

---

<p align="center">
  <b>Bank Loopy</b> — <i>Kelompok 6 | Tubes PBO</i>
