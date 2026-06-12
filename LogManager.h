#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "DatabaseManager.h"
#include "Helper.h"

using namespace std;

class LogManager {
public:
    // FILE HANDLING
    void catatLaporanKeuangan(DatabaseManager& db, string noRek, string nama, string jenisTx, long long nominal, string detail = "-", long long admBank = 0) {
        ofstream file(db.dapatkanPathLaporan(), ios::app); 
        if (file.is_open()) {
            file << "NoRek: " << noRek << "\n";
            file << "Nama: " << nama << "\n";
            file << "Transaksi: " << jenisTx << "\n";
            file << "Nominal: Rp " << formatRibuan(nominal) << "\n";
            file << "Adm Bank: Rp " << formatRibuan(admBank) << "\n";
            file << "Total: Rp " << formatRibuan(nominal + admBank) << "\n";
            file << "Detail: " << detail << "\n";
            file << "-----------------------------------------\n";
            file.close();
        }
    }

    // FILE HANDLING
    void lihatLaporanKeuangan(DatabaseManager& db, string targetNoRek) {
        system("CLS");
        string margin(14, ' ');
        string promptMargin(15, ' ');
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "      MUTASI REKENING PRIBADI      \n";
        cout << margin << "===================================\n\n";

        ifstream file(db.dapatkanPathLaporan());
        
        if (file.is_open()) {
            string line;
            bool ketemu = false;
            struct TransaksiRecord {
                string noRek;
                string nama;
                string jenisTx;
                string nominal;
                string admBank;
                string total;
                string detail;
            } record;

            while (getline(file, line)) {
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    string key = line.substr(0, pos);
                    string val = line.substr(pos + 1);

                    // Trim key
                    size_t fk = key.find_first_not_of(" \t\r\n");
                    if (fk != string::npos) {
                        size_t lk = key.find_last_not_of(" \t\r\n");
                        key = key.substr(fk, lk - fk + 1);
                    } else {
                        key = "";
                    }

                    // Trim val
                    size_t fv = val.find_first_not_of(" \t\r\n");
                    if (fv != string::npos) {
                        size_t lv = val.find_last_not_of(" \t\r\n");
                        val = val.substr(fv, lv - fv + 1);
                    } else {
                        val = "";
                    }

                    if (key == "NoRek") {
                        record.noRek = val;
                    } else if (key == "Nama") {
                        record.nama = val;
                    } else if (key == "Transaksi") {
                        record.jenisTx = val;
                    } else if (key == "Nominal") {
                        record.nominal = val;
                    } else if (key == "Adm Bank") {
                        record.admBank = val;
                    } else if (key == "Total") {
                        record.total = val;
                    } else if (key == "Detail") {
                        record.detail = val;
                    }
                } else if (line.rfind("-----------------------------------------", 0) == 0) {
                    if (record.noRek == targetNoRek) {
                        if (record.admBank.empty()) record.admBank = "Rp 0";
                        if (record.total.empty()) record.total = record.nominal;

                        cout << promptMargin << left << setw(12) << "Nama" << " : " << record.nama << "\n";
                        cout << promptMargin << left << setw(12) << "Transaksi" << " : " << record.jenisTx << "\n";
                        cout << promptMargin << left << setw(12) << "Nominal" << " : " << formatRpString(record.nominal) << "\n";
                        cout << promptMargin << left << setw(12) << "Adm Bank" << " : " << formatRpString(record.admBank) << "\n";
                        cout << promptMargin << left << setw(12) << "Total" << " : " << formatRpString(record.total) << "\n";
                        cout << promptMargin << left << setw(12) << "Detail" << " : " << record.detail << "\n";
                        cout << promptMargin << "-----------------------------------------\n";
                        ketemu = true;
                    }
                    record = TransaksiRecord();
                }
            }
            file.close();
            if (!ketemu) {
                cout << promptMargin << "[ Belum ada riwayat transaksi pada Rekening Anda ]\n";
            }
        } else {
            cout << promptMargin << "[ Belum ada data mutasi keuangan terdaftar ]\n";
        }
        cout << "\n" << margin << "===================================\n";
    }
};

#endif
