#include <iostream>
#include <string>
using namespace std;

// Kita definisikan dlu struct mahasiswanya
struct Mahasiswa {
    string nama;
    string nim;
    float ipk;
};

int main() {
    const int jumlah = 5;
    Mahasiswa mhs[jumlah];

    // Kita isi data mahasiswanya
    mhs[0] = {"Bakil", "044", 4.00};
    mhs[1] = {"Balik", "045", 3.65};
    mhs[2] = {"Likab", "046", 3.75};
    mhs[3] = {"Baki", "047", 3.25};
    mhs[4] = {"Bakul", "067", 3.20};

    // Lalu kita tampilkan datanya
    cout << "=== Daftar Mahasiswa ===\n";
    for (int i = 0; i < jumlah; i++) {
        cout << i+1 << ". Nama: " << mhs[i].nama
             << " | NIM: " << mhs[i].nim
             << " | IPK: " << mhs[i].ipk << endl;
    }

    // Mencari mahasiswa dengan IPK tertinggi
    int indeksTertinggi = 0;
    for (int i = 1; i < jumlah; i++) {
        if (mhs[i].ipk > mhs[indeksTertinggi].ipk) {
            indeksTertinggi = i;
        }
    }
// Tampilkan mahasiswanya
    cout << "\n=== Mahasiswa dengan IPK Tertinggi ===\n";
    cout << "Nama : " << mhs[indeksTertinggi].nama << endl;
    cout << "NIM  : " << mhs[indeksTertinggi].nim << endl;
    cout << "IPK  : " << mhs[indeksTertinggi].ipk << endl;

    return 0;
}