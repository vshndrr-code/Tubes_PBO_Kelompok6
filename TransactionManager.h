#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Akun.h"
#include "Transaksi.h"
#include "DatabaseManager.h"
#include "LogManager.h"
#include "Helper.h"

using namespace std;

class TransactionManager {
private:
    bool konfirmasiSebelumCetak() {
        char pilihan;
        string margin(14, ' ');
        cout << "\n" << margin << "===================================\n";
        cout << margin << " APAKAH ANDA YAKIN INGIN MELANJUTKAN\n";
        cout << margin << "       TRANSAKSI INI? (Y/N) : ";
        cin >> pilihan;
        if (pilihan == 'y' || pilihan == 'Y') return true;
        
        cout << "\n" << margin << " [!] TRANSAKSI TELAH DIBATALKAN.\n";
        return false;
    }

public:
    // POLYMORPHISM
    void prosesSetor(Akun& user, DatabaseManager& db, LogManager& log, vector<Akun>& listAkun) {
        system("CLS");
        long long jumlah;
        string margin(14, ' ');
        string promptMargin(15, ' ');
        
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "           SETORAN TUNAI           \n";
        cout << margin << "===================================\n\n";
        cout << margin << " KETIK 0 UNTUK KEMBALI KE MENU UTAMA\n\n";
        
        jumlah = dapatkanInput<long long>(promptMargin + "MASUKKAN NOMINAL SETORAN: Rp ");

        if (jumlah == 0) return;

        if (jumlah < 0 || jumlah % 50000 != 0) {
            cout << "\n" << promptMargin << "[!] GAGAL: HARUS KELIPATAN RP 50.000 DAN POSITIF!\n";
            return;
        }

        if (!konfirmasiSebelumCetak()) return;
        
        long long saldoSementara = user.getSaldo();
        
        // POLYMORPHISM & DYNAMIC MEMORY ALLOCATION
        Transaksi* tx = new Setor(jumlah); // Alokasi memory dinamis
        tx->eksekusi(saldoSementara);
        user.setSaldo(saldoSementara);
        delete tx; // Dealokasi memory 
        
        db.simpanDatabase(listAkun);
        log.catatLaporanKeuangan(db, user.getNoRek(), user.getNama(), "SETOR TUNAI", jumlah);
        
        system("CLS");
        cout << "\n" << promptMargin << "[!] SETORAN BERHASIL!\n\n";
        user.tampilkanData("SETOR TUNAI", jumlah);
    }

    // POLYMORPHISM
    void prosesTarik(Akun& user, DatabaseManager& db, LogManager& log, vector<Akun>& listAkun) {
        system("CLS");
        string margin(8, ' ');
        string promptMargin(15, ' ');
        cout << "\n";
        cout << margin << "====================================================\n";
        cout << margin << "                     BANK LOOPY                     \n";
        cout << margin << "                  PENARIKAN TUNAI                   \n";
        cout << margin << "====================================================\n\n";
        cout << margin << "  <--- [1] 50.000                [4] 1.000.000  --->\n\n\n";
        cout << margin << "  <--- [2] 200.000               [5] 5.000.000  --->\n\n\n";
        cout << margin << "  <--- [3] 500.000               [6] JUMLAH LAIN--->\n\n\n";
        cout << margin << "====================================================\n";
        
        int pilihan = dapatkanInputBatas<int>(margin + "PILIH OPSI (1-6) ATAU 0 UNTUK KEMBALI: ", 0, 6);

        if (pilihan == 0) return;

        long long jumlah = 0;
        if (pilihan == 1) {
            jumlah = 50000;
        } else if (pilihan == 2) {
            jumlah = 200000;
        } else if (pilihan == 3) {
            jumlah = 500000;
        } else if (pilihan == 4) {
            jumlah = 1000000;
        } else if (pilihan == 5) {
            jumlah = 5000000;
        } else if (pilihan == 6) {
            system("CLS");
            string m(14, ' ');
            cout << "\n\n";
            cout << m << "===================================\n";
            cout << m << "            BANK LOOPY             \n";
            cout << m << "          PENARIKAN TUNAI          \n";
            cout << m << "===================================\n\n";
            cout << m << " KETIK 0 UNTUK KEMBALI KE MENU UTAMA\n\n";
            jumlah = dapatkanInput<long long>(promptMargin + "MASUKKAN NOMINAL PENARIKAN: Rp ");
            if (jumlah == 0) return;
        }

        if (jumlah < 0 || jumlah % 50000 != 0) {
            cout << "\n" << promptMargin << "[!] GAGAL: HARUS KELIPATAN RP 50.000 DAN POSITIF!\n";
            return;
        }

        if (user.getSaldo() - jumlah < 50000) {
            cout << "\n" << promptMargin << "[!] GAGAL: SALDO MINIMAL HARUS RP 50.000!\n";
            return;
        }

        if (!konfirmasiSebelumCetak()) return;

        long long saldoSementara = user.getSaldo();
        
        // POLYMORPHISM & DYNAMIC MEMORY ALLOCATION
        Transaksi* tx = new Tarik(jumlah); // Alokasi memory dinamis
        tx->eksekusi(saldoSementara);
        user.setSaldo(saldoSementara);
        delete tx; // Dealokasi memory 

        db.simpanDatabase(listAkun);
        log.catatLaporanKeuangan(db, user.getNoRek(), user.getNama(), "TARIK TUNAI", jumlah);

        system("CLS");
        cout << "\n" << promptMargin << "[!] PENARIKAN BERHASIL!\n\n";
        user.tampilkanData("TARIK TUNAI", jumlah);
    }

    void prosesTransfer(Akun& user, vector<Akun>& listAkun, DatabaseManager& db, LogManager& log) {
        long long jumlah;
        string rekTujuan;
        int pilihanBank;
        string namaBank = "LOOPY";
        string margin(14, ' ');
        string promptMargin(15, ' ');
        string namaTujuan = "";

        system("CLS");
        cout << "\n\n";
        cout << margin << "===================================\n";
        cout << margin << "            BANK LOOPY             \n";
        cout << margin << "         PILIH BANK TUJUAN         \n";
        cout << margin << "===================================\n\n";
        cout << margin << " [1] BANK LOOPY (SESAMA) [5] BANK BCA\n";
        cout << margin << " [2] BANK MANDIRI        [6] BANK CIMB NIAGA\n";
        cout << margin << " [3] BANK BRI            [7] BANK SUMUT\n";
        cout << margin << " [4] BANK BNI            [8] BANK DANAMON\n\n";
        cout << margin << "===================================\n";
        cout << margin << " KETIK 0 UNTUK KEMBALI KE MENU UTAMA\n";
        
        pilihanBank = dapatkanInputBatas<int>(promptMargin + "PILIH (1-8): ", 0, 8);

        if (pilihanBank == 0) return;

        switch (pilihanBank) {
            case 1: namaBank = "LOOPY"; break;
            case 2: namaBank = "MANDIRI"; break;
            case 3: namaBank = "BRI"; break;
            case 4: namaBank = "BNI"; break;
            case 5: namaBank = "BCA"; break;
            case 6: namaBank = "CIMB NIAGA"; break;
            case 7: namaBank = "BANK SUMUT"; break;
            case 8: namaBank = "DANAMON"; break;
            default:
                cout << "\n" << promptMargin << "[!] PILIHAN BANK TIDAK VALID!\n";
                return;
        }

        cout << "\n" << promptMargin << "MASUKKAN REKENING TUJUAN  : ";
        cin >> rekTujuan;
        if (rekTujuan == "0") return;

        if (pilihanBank == 1) {
            if (rekTujuan == user.getNoRek()) {
                cout << "\n" << promptMargin << "[!] GAGAL: TIDAK BISA TRANSFER KE REKENING SENDIRI!\n";
                return;
            }
            bool found = false;
            for (size_t i = 0; i < listAkun.size(); i++) {
                if (listAkun[i].getNoRek() == rekTujuan) {
                    namaTujuan = listAkun[i].getNama();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "\n" << promptMargin << "[!] GAGAL: REKENING TUJUAN TIDAK DITEMUKAN!\n";
                return;
            }
            cout << promptMargin << "NAMA PENERIMA             : " << namaTujuan << "\n";
        }

        cout << promptMargin << "MASUKKAN NOMINAL TRANSFER : Rp ";
        cin >> jumlah;

        if (jumlah <= 0) {
            cout << "\n" << promptMargin << "[!] GAGAL: NOMINAL TIDAK VALID!\n";
            return;
        }

        long long totalDeduction = jumlah;
        long long biayaAdmin = 0;
        if (pilihanBank != 1) {
            biayaAdmin = 2500;
            totalDeduction += biayaAdmin;
        }

        if (user.getSaldo() - totalDeduction < 50000) {
            if (biayaAdmin > 0) {
                cout << "\n" << promptMargin << "[!] GAGAL: SALDO MINIMAL HARUS RP 50.000\n";
                cout << promptMargin << "    (DIBUTUHKAN RP " << formatRibuan(totalDeduction) << " TERMASUK ADMIN Rp 2.500)!\n";
            } else {
                cout << "\n" << promptMargin << "[!] GAGAL: SALDO MINIMAL HARUS RP 50.000!\n";
            }
            return;
        }

        // Tampilkan Ringkasan
        cout << "\n" << promptMargin << "RINGKASAN TRANSFER:\n";
        cout << promptMargin << "BANK TUJUAN : " << namaBank << "\n";
        cout << promptMargin << "REK TUJUAN  : " << rekTujuan << "\n";
        if (pilihanBank == 1) {
            cout << promptMargin << "NAMA        : " << namaTujuan << "\n";
        }
        cout << promptMargin << "NOMINAL     : Rp " << formatRibuan(jumlah) << "\n";
        if (biayaAdmin > 0) {
            cout << promptMargin << "BIAYA ADMIN : Rp " << formatRibuan(biayaAdmin) << "\n";
            cout << promptMargin << "TOTAL       : Rp " << formatRibuan(totalDeduction) << "\n";
        }

        if (!konfirmasiSebelumCetak()) return;

        user -= totalDeduction; 

        if (pilihanBank == 1) {
            for (size_t i = 0; i < listAkun.size(); i++) {
                if (listAkun[i].getNoRek() == rekTujuan) {
                    listAkun[i] += jumlah; 
                    log.catatLaporanKeuangan(db, rekTujuan, listAkun[i].getNama(), "TRANSFER MASUK", jumlah, "Dari No.Rek: " + user.getNoRek());
                    break;
                }
            }
        }

        db.simpanDatabase(listAkun);
        
        string detailLog = "Ke Bank " + namaBank + " (" + rekTujuan + ")";
        if (pilihanBank == 1) {
            detailLog += " a.n. " + namaTujuan;
        }
        log.catatLaporanKeuangan(db, user.getNoRek(), user.getNama(), "TRANSFER KELUAR", jumlah, detailLog, biayaAdmin);

        system("CLS");
        cout << "\n" << promptMargin << "[!] TRANSFER BERHASIL!\n\n";
        
        string infoStruk = "Ke " + namaBank + " (" + rekTujuan + ")";
        if (pilihanBank == 1) {
            infoStruk += "\n" + margin + "A.N.        : " + namaTujuan;
        }
        user.tampilkanData("TRANSFER TUNAI", jumlah, infoStruk, biayaAdmin);
    }
};

#endif
