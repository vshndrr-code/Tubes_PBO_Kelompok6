#ifndef AKUN_H
#define AKUN_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

#include "Helper.h"

// CLASS & OBJECT
class Akun {
private:
    // ENCAPSULATION
    string nama;
    string nik; 
    string pin;
    long long saldo;
    string noRek;
    string telepon;

public:
    // CONSTRUCTOR & DESTRUCTOR
    Akun() : nama(""), nik(""), pin(""), saldo(0), noRek(""), telepon("") {}
    Akun(string n, string id, string p, long long s = 0, string nr = "", string tel = "") 
        : nama(n), nik(id), pin(p), saldo(s), noRek(nr), telepon(tel) {}
    ~Akun() {}

    // ENCAPSULATION (Getter/Setter)
    string getNama() const { return nama; }
    string getNik() const { return nik; }
    string getPin() const { return pin; }
    long long getSaldo() const { return saldo; }
    string getNoRek() const { return noRek; }
    string getTelepon() const { return telepon; }
    
    void setSaldo(long long s) { this->saldo = s; }
    void setPin(string p) { this->pin = p; }
    void setNoRek(string nr) { this->noRek = nr; }
    void setTelepon(string tel) { this->telepon = tel; }

    // OPERATOR OVERLOADING
    void operator += (long long jumlah) { this->saldo += jumlah; }
    void operator -= (long long jumlah) {
        if (this->saldo - jumlah >= 50000) this->saldo -= jumlah;
    }

    // METHOD OVERLOADING
    void tampilkanData() {
        string margin(15, ' ');
        cout << margin << left << setw(15) << "NAMA NASABAH" << ": " << nama << "\n";
        cout << margin << left << setw(15) << "NO. REKENING" << ": " << noRek << "\n";
    }

    // METHOD OVERLOADING
    void tampilkanData(bool denganSaldo) {
        tampilkanData();
        if (denganSaldo) {
            string margin(15, ' ');
            cout << margin << left << setw(15) << "TOTAL SALDO" << ": Rp " << formatRibuan(saldo) << "\n";
        }
    }

    // METHOD OVERLOADING
    void tampilkanData(string jenisTransaksi, long long nominal, string infoTambahan = "", long long admBank = 0) {
        string margin(14, ' ');
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "       STRUK RESMI TRANSAKSI       \n";
        cout << margin << "===================================\n";
        cout << margin << left << setw(12) << "NAMA" << ": " << nama << "\n";
        cout << margin << left << setw(12) << "NO REK" << ": " << noRek << "\n";
        cout << margin << left << setw(12) << "JENIS TX" << ": " << jenisTransaksi << "\n";
        if (!infoTambahan.empty()) {
            cout << margin << left << setw(12) << "DETAIL TX" << ": " << infoTambahan << "\n";
        }
        cout << margin << left << setw(12) << "NOMINAL" << ": Rp " << formatRibuan(nominal) << "\n";
        cout << margin << left << setw(12) << "ADM BANK" << ": Rp " << formatRibuan(admBank) << "\n";
        cout << margin << left << setw(12) << "TOTAL" << ": Rp " << formatRibuan(nominal + admBank) << "\n";
        cout << margin << left << setw(12) << "SISA SALDO" << ": Rp " << formatRibuan(saldo) << "\n";
        cout << margin << "===================================\n";
        cout << margin << "  TERIMA KASIH ATAS TRANSAKSI ANDA \n";
        cout << margin << "===================================\n";
    }
};

#endif
