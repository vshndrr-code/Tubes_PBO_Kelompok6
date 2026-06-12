#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "Akun.h"
#include "DatabaseManager.h"
#include "Helper.h"

using namespace std;

// ENCAPSULATION (Public methods)
class AuthManager {
public:
    string inputPIN(string prompt) {
        string password = "";
        char ch;
        cout << prompt;
        while ((ch = (char)_getch()) != 13) {
            if (ch == 8) {
                if (password.length() > 0) {
                    password.pop_back();
                    cout << "\b \b";
                }
            } else if (isdigit(ch) && password.length() < 6) {
                password.push_back(ch);
                cout << "*";
            }
        }
        cout << endl;
        return password;
    }

    string inputNIK(string prompt) {
        string nik = "";
        char ch;
        cout << prompt;
        while ((ch = (char)_getch()) != 13) {
            if (ch == 8) {
                if (nik.length() > 0) {
                    nik.pop_back();
                    cout << "\b \b";
                }
            } else if (isdigit(ch) && nik.length() < 16) {
                nik.push_back(ch);
                cout << ch;
            }
        }
        cout << endl;
        return nik;
    }

    int loginProses(vector<Akun>& listAkun, int& currentUserIndex) {
        system("CLS");
        string inputID, inputPINStr;
        string margin(14, ' ');
        string promptMargin(15, ' ');
        
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "        AUTENTIKASI NASABAH        \n";
        cout << margin << "===================================\n\n";
        cout << margin << " KETIK 0 LALU ENTER UNTUK KEMBALI\n\n";
        cout << promptMargin << "MASUKKAN NO. REKENING : ";
        cin >> inputID;

        if (inputID == "0") {
            return -1; 
        }

        inputPINStr = inputPIN(promptMargin + "MASUKKAN PIN (6D)     : ");

        for (size_t i = 0; i < listAkun.size(); i++) {
            if (listAkun[i].getNoRek() == inputID && listAkun[i].getPin() == inputPINStr) {
                currentUserIndex = (int)i;
                return 1; 
            }
        }
        return 0; 
    }

    int registrasiProses(vector<Akun>& listAkun, DatabaseManager& db) {
        string nama, nik, pinBaru, konfirmasiPin;
        string margin(14, ' ');
        string promptMargin(15, ' ');
        
        system("CLS");
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "   REGISTRASI NASABAH BANK BARU    \n";
        cout << margin << "===================================\n\n";
        cout << margin << " KETIK 0 LALU ENTER UNTUK KEMBALI\n\n";

        while (true) {
            nik = inputNIK(promptMargin + "MASUKKAN NIK BARU (16D): ");
            if (nik == "0") {
                return -1;
            }
            if (nik.length() != 16) {
                cout << promptMargin << "[!] NIK HARUS BERJUMLAH 16 DIGIT ANGKA!\n\n";
                continue;
            }
            
            bool sudahAda = false;
            for (const auto& acc : listAkun) {
                if (acc.getNik() == nik) {
                    sudahAda = true;
                    break;
                }
            }
            if (sudahAda) {
                cout << promptMargin << "[!] NIK TERSEBUT SUDAH TERDAFTAR!\n\n";
                continue;
            }
            break;
        }

        cout << promptMargin << "MASUKKAN NAMA ANDA     : ";
        cin.ignore();
        getline(cin, nama);

        string telepon;
        while (true) {
            cout << promptMargin << "MASUKKAN NO. TELEPON   : ";
            cin >> telepon;
            if (telepon == "0") return -1;
            bool validTel = (telepon.length() >= 11);
            for (char c : telepon) {
                if (!isdigit(c)) validTel = false;
            }
            if (!validTel) {
                cout << promptMargin << "[!] NOMOR TELEPON MINIMAL 11 DIGIT ANGKA!\n\n";
                continue;
            }
            break;
        }
        
        while (true) {
            pinBaru = inputPIN(promptMargin + "BUAT PIN 6 ANGKA       : ");
            if (pinBaru.length() != 6) {
                cout << promptMargin << "[!] PIN HARUS BERJUMLAH 6 DIGIT ANGKA!\n\n";
                continue;
            }
            konfirmasiPin = inputPIN(promptMargin + "KONFIRMASI PIN ANDA    : ");
            if (pinBaru == konfirmasiPin) {
                break;
            } else {
                cout << promptMargin << "[!] PIN TIDAK COCOK! SILAKAN ULANGI.\n\n";
            }
        }

        // Generasi nomor rekening 10 digit yang unik
        string norek = "";
        bool unik = false;
        while (!unik) {
            norek = "";
            for (int i = 0; i < 10; i++) {
                if (i == 0) norek += to_string(rand() % 9 + 1);
                else norek += to_string(rand() % 10);
            }
            unik = true;
            for (const auto& acc : listAkun) {
                if (acc.getNoRek() == norek) {
                    unik = false;
                    break;
                }
            }
        }
        
        listAkun.push_back(Akun(nama, nik, pinBaru, 0, norek, telepon)); 
        db.simpanDatabase(listAkun);
        cout << "\n" << promptMargin << "[!] REGISTRASI BERHASIL!\n";
        cout << promptMargin << "NOMOR REKENING ANDA : " << norek << "\n";
        return 1;
    }

    void ubahPIN(Akun& user, DatabaseManager& db, const vector<Akun>& listAkun) {
        system("CLS");
        string margin(14, ' ');
        string promptMargin(15, ' ');
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "             UBAH PIN              \n";
        cout << margin << "===================================\n\n";
        
        string pinLama = inputPIN(promptMargin + "MASUKKAN PIN LAMA      : ");
        if (pinLama != user.getPin()) {
            cout << "\n" << promptMargin << "[!] PIN LAMA SALAH!\n";
            return;
        }

        string pinBaru, konfirmasiPin;
        while (true) {
            pinBaru = inputPIN(promptMargin + "MASUKKAN PIN BARU (6D) : ");
            if (pinBaru.length() != 6) {
                cout << promptMargin << "[!] PIN HARUS BERJUMLAH 6 DIGIT ANGKA!\n\n";
                continue;
            }
            if (pinBaru == pinLama) {
                cout << promptMargin << "[!] PIN BARU TIDAK BOLEH SAMA DENGAN PIN LAMA!\n\n";
                continue;
            }
            konfirmasiPin = inputPIN(promptMargin + "KONFIRMASI PIN BARU    : ");
            if (pinBaru == konfirmasiPin) {
                break;
            } else {
                cout << promptMargin << "[!] PIN TIDAK COCOK! SILAKAN ULANGI.\n\n";
            }
        }

        user.setPin(pinBaru);
        db.simpanDatabase(listAkun);
        cout << "\n" << promptMargin << "[!] PIN BERHASIL DIUBAH!\n";
    }
};

#endif
