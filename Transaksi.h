#ifndef TRANSAKSI_H
#define TRANSAKSI_H

// ABSTRACTION (Abstract Class)
class Transaksi {
protected:
    long long jumlah;

public:
    // CONSTRUCTOR & DESTRUCTOR
    Transaksi(long long j) : jumlah(j) {}
    
    virtual void eksekusi(long long &saldo) = 0; // Pure Virtual Function
    
    virtual ~Transaksi() {}
};

// INHERITANCE
class Setor : public Transaksi {
public:
    // CONSTRUCTOR & DESTRUCTOR
    Setor(long long j) : Transaksi(j) {}
    
    // OVERRIDING
    void eksekusi(long long &saldo) override { 
        saldo += jumlah; 
    }
};

// INHERITANCE
class Tarik : public Transaksi {
public:
    // CONSTRUCTOR & DESTRUCTOR
    Tarik(long long j) : Transaksi(j) {}
    
    // OVERRIDING
    void eksekusi(long long &saldo) override { 
        saldo -= jumlah; 
    }
};

#endif
