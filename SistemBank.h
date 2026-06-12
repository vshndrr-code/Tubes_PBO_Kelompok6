#ifndef SISTEM_BANK_H
#define SISTEM_BANK_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include <ctime>
#include <cstdlib>

#include "Akun.h"
#include "DatabaseManager.h"
#include "LogManager.h"
#include "AuthManager.h"
#include "TransactionManager.h"
#include "Helper.h"

using namespace std;

// COMPOSITION & CLASS & OBJECT
class SistemBank {
private:
    // VECTOR
    vector<Akun> listAkun;
    int currentUserIndex;

    DatabaseManager db;
    LogManager logManager;
    AuthManager auth;
    TransactionManager txManager;

public:
    // CONSTRUCTOR & DESTRUCTOR
    SistemBank() : currentUserIndex(-1) {
        srand(time(0));
        db.muatDatabase(listAkun);
    }

    ~SistemBank() {
        listAkun.clear();
    }

    void setConsoleTheme() {
        system("COLOR 1F");
    }

    int menuSelamatDatang() {
        system("CLS");
        string margin(14, ' ');
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "       SELAMAT DATANG DI ATM       \n";
        cout << margin << "===================================\n\n";
        cout << margin << "  [1] LOGIN (SUDAH PUNYA AKUN)\n";
        cout << margin << "  [2] REGISTRASI (BUAT AKUN BARU)\n";
        cout << margin << "  [3] KELUAR APLIKASI\n\n";
        cout << margin << "===================================\n";
        return dapatkanInputBatas<int>(margin + "  PILIH OPSI (1-3): ", 1, 3);
    }

    void cekSaldo() {
        system("CLS");
        string margin(14, ' ');
        string promptMargin(15, ' ');
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "             CEK SALDO             \n";
        cout << margin << "===================================\n\n";
        cout << promptMargin << left << setw(13) << "NO. REKENING" << ": " << listAkun[currentUserIndex].getNoRek() << "\n";
        cout << promptMargin << left << setw(13) << "SISA SALDO" << ": Rp " << formatRibuan(listAkun[currentUserIndex].getSaldo()) << "\n";
        cout << margin << "===================================\n";
    }

    void lihatInformasiAkun() {
        system("CLS");
        string margin(14, ' ');
        string promptMargin(15, ' ');
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "          INFORMASI AKUN           \n";
        cout << margin << "===================================\n\n";
        cout << promptMargin << left << setw(13) << "NAMA NASABAH" << ": " << listAkun[currentUserIndex].getNama() << "\n";
        cout << promptMargin << left << setw(13) << "NO. REKENING" << ": " << listAkun[currentUserIndex].getNoRek() << "\n";
        cout << promptMargin << left << setw(13) << "NIK / ID" << ": " << listAkun[currentUserIndex].getNik() << "\n";
        cout << promptMargin << left << setw(13) << "NO. TELEPON" << ": " << listAkun[currentUserIndex].getTelepon() << "\n";
        cout << margin << "===================================\n";
    }

    void menuUtama() {
        system("CLS");
        string margin(8, ' ');
        cout << "\n";
        cout << margin << "=================================================================\n";
        cout << margin << "                           MENU UTAMA                            \n";
        cout << margin << "=================================================================\n\n";
        cout << margin << "  <--- [1] PENARIKAN TUNAI              [5] MUTASI REKENING  --->\n\n\n";
        cout << margin << "  <--- [2] SETORAN TUNAI                [6] UBAH PIN         --->\n\n\n";
        cout << margin << "  <--- [3] CEK SALDO                    [7] INFORMASI AKUN   --->\n\n\n";
        cout << margin << "  <--- [4] TRANSFER DANA                [8] LOGOUT / KELUAR  --->\n\n\n";
    }

    void jalankan() {
        while (true) {
            int statusAwal = menuSelamatDatang();

            if (statusAwal == 1) {
                int pointerPercobaan = 0;
                bool loginSukses = false;
                bool ditekanKembali = false;

                while (pointerPercobaan < 3) {
                    int hasilLogin = auth.loginProses(listAkun, currentUserIndex);
                    
                    if (hasilLogin == -1) {
                        ditekanKembali = true; 
                        break;
                    } else if (hasilLogin == 1) {
                        loginSukses = true;
                        break;
                    } else {
                        pointerPercobaan++;
                        string promptMargin(15, ' ');
                        cout << "\n" << promptMargin << "[!] PIN / ID SALAH! SISA PERCOBAAN: " << (3 - pointerPercobaan) << "\n";
                        jedaLayar();
                    }
                }

                if (ditekanKembali) continue; 

                if (!loginSukses) {
                    string promptMargin(15, ' ');
                    cout << "\n" << promptMargin << "[!] AKSES DIBLOKIR SEMENTARA. KEMBALI KE MENU UTAMA.\n";
                    jedaLayar();
                    continue; 
                }

                int pilihan;
                do {
                    menuUtama();
                    string inputMargin(8, ' ');
                    pilihan = dapatkanInputBatas<int>(inputMargin + "PILIH MENU (1-8): ", 1, 8);

                    switch (pilihan) {
                        case 1: 
                            txManager.prosesTarik(listAkun[currentUserIndex], db, logManager, listAkun); 
                            jedaLayar(); 
                            break;
                        case 2: 
                            txManager.prosesSetor(listAkun[currentUserIndex], db, logManager, listAkun); 
                            jedaLayar(); 
                            break;
                        case 3: 
                            cekSaldo();
                            jedaLayar(); 
                            break;
                        case 4: 
                            txManager.prosesTransfer(listAkun[currentUserIndex], listAkun, db, logManager); 
                            jedaLayar(); 
                            break;
                        case 5: 
                            logManager.lihatLaporanKeuangan(db, listAkun[currentUserIndex].getNoRek()); 
                            jedaLayar(); 
                            break; 
                        case 6: 
                            auth.ubahPIN(listAkun[currentUserIndex], db, listAkun);
                            jedaLayar();
                            break;
                        case 7: 
                            lihatInformasiAkun();
                            jedaLayar();
                            break;
                        case 8: 
                            currentUserIndex = -1; // logout
                            break;
                        default:
                            string promptMargin(15, ' ');
                            cout << "\n" << promptMargin << "[!] PILIHAN MENU TIDAK VALID!\n";
                            jedaLayar();
                            break;
                    }
                } while (pilihan != 8);

            } else if (statusAwal == 2) {
                int hasilRegis = auth.registrasiProses(listAkun, db);
                if (hasilRegis == -1) {
                    continue; 
                }
                jedaLayar();
            } else if (statusAwal == 3) {
                system("CLS");
                string margin(14, ' ');
                cout << "\n\n\n" << margin << "TERIMA KASIH TELAH MENGGUNAKAN BANK LOOPY\n\n\n";
                break;
            } else {
                string promptMargin(15, ' ');
                cout << "\n" << promptMargin << "[!] PILIHAN TIDAK VALID!\n";
                jedaLayar();
            }
        }
    }
};

#endif
