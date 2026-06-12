#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <limits>

using namespace std;

// TEMPLATE
template <typename T>
T dapatkanInput(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "               [!] Input tidak valid! Silakan coba lagi.\n";
    }
}

// TEMPLATE
template <typename T>
T dapatkanInputBatas(const string& prompt, T minVal, T maxVal) {
    T value;
    while (true) {
        value = dapatkanInput<T>(prompt);
        if (value >= minVal && value <= maxVal) {
            return value;
        }
        cout << "               [!] Input harus antara " << minVal << " dan " << maxVal << "!\n";
    }
}

#include <algorithm>

inline string formatRibuan(long long nominal) {
    string s = to_string(nominal);
    int n = s.length();
    int count = 0;
    string result = "";
    for (int i = n - 1; i >= 0; i--) {
        result += s[i];
        count++;
        if (count % 3 == 0 && i != 0) {
            result += ".";
        }
    }
    reverse(result.begin(), result.end());
    return result;
}

inline string formatRpString(string val) {
    size_t pos = val.find("Rp");
    if (pos != string::npos) val = val.substr(pos + 2);
    // Strip dots
    val.erase(remove(val.begin(), val.end(), '.'), val.end());
    // Trim spaces
    size_t f = val.find_first_not_of(" \t\r\n");
    if (f != string::npos) {
        size_t l = val.find_last_not_of(" \t\r\n");
        val = val.substr(f, l - f + 1);
    } else {
        val = "";
    }
    if (val.empty()) return "Rp 0";
    try {
        return "Rp " + formatRibuan(stoll(val));
    } catch (...) {
        return "Rp " + val;
    }
}

inline void jedaLayar() {
    cout << "\n  TEKAN ENTER UNTUK MELANJUTKAN...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

#endif

