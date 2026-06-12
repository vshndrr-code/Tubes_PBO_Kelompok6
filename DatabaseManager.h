#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Akun.h"

using namespace std;

class DatabaseManager {
public:
    // FILE HANDLING
    string dapatkanPathDatabase() {
        ifstream file("output/database_bank.txt");
        if (file.is_open()) {
            file.close();
            return "output/database_bank.txt";
        }
        
        ifstream fileRoot("database_bank.txt");
        if (fileRoot.is_open()) {
            fileRoot.close();
            return "database_bank.txt";
        }

        ofstream testFile("output/test_write.tmp");
        if (testFile.is_open()) {
            testFile.close();
            remove("output/test_write.tmp");
            return "output/database_bank.txt";
        }

        return "database_bank.txt";
    }

    // FILE HANDLING
    string dapatkanPathLaporan() {
        ifstream file("output/laporan_keuangan.txt");
        if (file.is_open()) {
            file.close();
            return "output/laporan_keuangan.txt";
        }
        
        ifstream fileRoot("laporan_keuangan.txt");
        if (fileRoot.is_open()) {
            fileRoot.close();
            return "laporan_keuangan.txt";
        }

        ofstream testFile("output/test_write.tmp");
        if (testFile.is_open()) {
            testFile.close();
            remove("output/test_write.tmp");
            return "output/laporan_keuangan.txt";
        }

        return "laporan_keuangan.txt";
    }

    // FILE HANDLING
    void muatDatabase(vector<Akun>& listAkun) {
        listAkun.clear();
        ifstream file(dapatkanPathDatabase());
        if (file.is_open()) {
            string line;
            string nama = "", nik = "", pin = "", noRek = "", telepon = "";
            long long saldo = 0;
            
            while (getline(file, line)) {
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    string key = line.substr(0, pos);
                    string val = line.substr(pos + 1);
                    
                    // Trim key spaces
                    size_t fk = key.find_first_not_of(" \t\r\n");
                    if (fk != string::npos) {
                        size_t lk = key.find_last_not_of(" \t\r\n");
                        key = key.substr(fk, lk - fk + 1);
                    } else {
                        key = "";
                    }

                    // Trim val spaces
                    size_t fv = val.find_first_not_of(" \t\r\n");
                    if (fv != string::npos) {
                        size_t lv = val.find_last_not_of(" \t\r\n");
                        val = val.substr(fv, lv - fv + 1);
                    } else {
                        val = "";
                    }

                    if (key == "NIK") {
                        nik = val;
                    } else if (key == "PIN") {
                        pin = val;
                    } else if (key == "NoRek") {
                        noRek = val;
                    } else if (key == "Telepon") {
                        telepon = val;
                    } else if (key == "Saldo") {
                        // Hilangkan "Rp" jika ada
                        size_t rpPos = val.find("Rp");
                        if (rpPos != string::npos) {
                            val = val.substr(rpPos + 2);
                        }
                        // Trim val lagi setelah hilangkan "Rp"
                        size_t fvs = val.find_first_not_of(" \t\r\n");
                        if (fvs != string::npos) {
                            size_t lvs = val.find_last_not_of(" \t\r\n");
                            val = val.substr(fvs, lvs - fvs + 1);
                        } else {
                            val = "";
                        }
                        try {
                            saldo = stoll(val);
                        } catch (...) {
                            saldo = 0;
                        }
                    } else if (key == "Nama") {
                        nama = val;
                    }
                } else if (line.rfind("-----------------------------------------", 0) == 0) {
                    if (!nik.empty()) {
                        listAkun.push_back(Akun(nama, nik, pin, saldo, noRek, telepon));
                    }
                    nama = ""; nik = ""; pin = ""; saldo = 0; noRek = ""; telepon = "";
                }
            }
            file.close();
        }
    }

    // FILE HANDLING
    void simpanDatabase(const vector<Akun>& listAkun) {
        ofstream file(dapatkanPathDatabase());
        if (file.is_open()) {
            for (const auto& acc : listAkun) {
                file << "NoRek: " << acc.getNoRek() << "\n";
                file << "NIK: " << acc.getNik() << "\n";
                file << "PIN: " << acc.getPin() << "\n";
                file << "Saldo: Rp " << acc.getSaldo() << "\n";
                file << "Nama: " << acc.getNama() << "\n";
                file << "Telepon: " << acc.getTelepon() << "\n";
                file << "-----------------------------------------\n";
            }
            file.close();
        }
    }
};

#endif
