/*
 * ================================================================
 *   SISTEM MANAJEMEN KEBERANGKATAN KERETA API
 *   Mata Kuliah : Algoritma pemrograman lanjut
 *   Nama        : Muhammad Bakil Amru
 *   NIM         : 2509106044
 * ================================================================
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <limits>
#include <climits>
using namespace std;

//   STRUCT - Cetak biru satu data kereta
struct Kereta {
    int    nomorKereta;    // contoh: 101
    string namaKereta;     // contoh: "Argo Bromo Anggrek"
    string asal;           // contoh: "Jakarta"
    string tujuan;         // contoh: "Surabaya"
    string jamBerangkat;   // format HH:MM, contoh: "08:00"
    string jamTiba;        // format HH:MM, contoh: "17:30"
    int    hargaTiket;     // dalam rupiah, contoh: 580000
    int    kursiTersedia;  // jumlah kursi kosong, contoh: 50
};

//   KONSTANTA
const int MAX_KERETA = 100;  // batas maksimal data yang bisa disimpan

//   FUNGSI BANTU INPUT
void bersihkanBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Paksa reset jika cin error, lalu bersihkan buffer
void resetCin() {
    cin.clear();
    bersihkanBuffer();
}

// Buang spasi di ujung kiri dan kanan string
// Contoh: "  Jakarta  " --> "Jakarta"
string trim(const string& s) {
    size_t awal  = s.find_first_not_of(" \t\r\n");
    if (awal == string::npos) return "";   // semua spasi --> kosong
    size_t akhir = s.find_last_not_of(" \t\r\n");
    return s.substr(awal, akhir - awal + 1);
}

// inputAngka()
//   Baca satu baris, pastikan isinya angka bulat dalam range.
int inputAngka(const string& prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        string baris;

        // getline membaca SELURUH baris sekaligus -- tidak ada sisa di buffer
        if (!getline(cin, baris)) {
            resetCin();
            cout << "   Terjadi gangguan baca input, coba ketik ulang.\n";
            continue;
        }

        string s = trim(baris);

        if (s.empty()) {
            cout << "   Input tidak boleh kosong. Ketik sebuah angka.\n";
            continue;
        }

        // Boleh diawali '-' untuk angka negatif, sisanya harus digit semua
        size_t mulai = (s[0] == '-') ? 1 : 0;
        bool semuaDigit = (mulai < s.size()); // minimal 1 digit
        for (size_t k = mulai; k < s.size(); k++) {
            if (!isdigit((unsigned char)s[k])) { semuaDigit = false; break; }
        }

        if (!semuaDigit) {
            cout << "   \"" << s << "\" bukan angka. Contoh yang benar: 42\n";
            continue;
        }

        try {
            long long tmp = stoll(s);
            if (tmp < minVal || tmp > maxVal) {
                cout << "   Angka harus antara " << minVal
                     << " sampai " << maxVal << ". Coba lagi.\n";
                continue;
            }
            return (int)tmp;
        } catch (...) {
            cout << "   Angka terlalu besar untuk ditampung. Coba angka lain.\n";
        }
    }
}

// Versi tanpa batas range (untuk kasus umum)
int inputAngka(const string& prompt) {
    return inputAngka(prompt, INT_MIN, INT_MAX);
}

// --------------------------------------------------------------
// inputTeks()
//   Baca satu baris teks. Tolak jika kosong atau spasi saja.
// --------------------------------------------------------------
string inputTeks(const string& prompt) {
    while (true) {
        cout << prompt;
        string baris;
        if (!getline(cin, baris)) {
            resetCin();
            cout << "   Terjadi gangguan baca input, coba ketik ulang.\n";
            continue;
        }
        string s = trim(baris);
        if (s.empty()) {
            cout << "   Tidak boleh kosong atau hanya spasi. Coba lagi.\n";
            continue;
        }
        return s;  // dikembalikan sudah bersih dari spasi tepi
    }
}

// --------------------------------------------------------------
// inputJam()
//   Baca jam dalam format HH:MM.
//   Contoh benar: 07:30  |  Contoh salah: 7:3, 25:00, jam7
// --------------------------------------------------------------
string inputJam(const string& prompt) {
    while (true) {
        cout << prompt;
        string baris;
        if (!getline(cin, baris)) {
            resetCin();
            cout << "   Terjadi gangguan baca input, coba ketik ulang.\n";
            continue;
        }
        string jam = trim(baris);

        if (jam.empty()) {
            cout << "   Jam tidak boleh kosong. Contoh: 07:30\n";
            continue;
        }
        // Harus persis 5 karakter: dua digit, titik dua, dua digit
        if (jam.length() != 5 || jam[2] != ':') {
            cout << "   Format salah (\"" << jam << "\"). Harus HH:MM, contoh: 07:30\n";
            continue;
        }
        // Pastikan posisi 0,1,3,4 semuanya angka
        bool semuaAngka = true;
        for (int k = 0; k < 5; k++) {
            if (k == 2) continue;
            if (!isdigit((unsigned char)jam[k])) { semuaAngka = false; break; }
        }
        if (!semuaAngka) {
            cout << "   Jam dan menit harus angka. Contoh: 07:30\n";
            continue;
        }
        int hh = stoi(jam.substr(0, 2));
        int mm = stoi(jam.substr(3, 2));
        if (hh > 23 || mm > 59) {
            cout << "   Jam valid: 00-23, menit valid: 00-59. Coba lagi.\n";
            continue;
        }
        return jam;
    }
}

//   FUNGSI TAMPILAN

void cetakGaris(char c = '=', int panjang = 80) {
    for (int i = 0; i < panjang; i++) cout << c;
    cout << "\n";
}

void cetakHeader() {
    cetakGaris('=');
    cout << "         SISTEM MANAJEMEN KEBERANGKATAN KERETA API\n";
    cetakGaris('=');
}

//   TAMPILKAN SEMUA KERETA
void tampilSemuaKereta(Kereta* arr, int jumlah) {
    if (jumlah == 0) {
        cout << "\n   Belum ada data kereta yang tersimpan.\n";
        return;
    }

    cetakGaris('-');
    cout << left
         << setw(5)  << "No"
         << setw(8)  << "No.KA"
         << setw(22) << "Nama Kereta"
         << setw(14) << "Asal"
         << setw(14) << "Tujuan"
         << setw(8)  << "Brkt"
         << setw(8)  << "Tiba"
         << setw(14) << "Harga (Rp)"
         << setw(7)  << "Kursi"
         << "\n";
    cetakGaris('-');

    for (int i = 0; i < jumlah; i++) {
        //            ->namaKereta mengambil field namaKereta dari sana
        cout << left
             << setw(5)  << (i + 1)
             << setw(8)  << (arr + i)->nomorKereta
             << setw(22) << (arr + i)->namaKereta
             << setw(14) << (arr + i)->asal
             << setw(14) << (arr + i)->tujuan
             << setw(8)  << (arr + i)->jamBerangkat
             << setw(8)  << (arr + i)->jamTiba
             << setw(14) << (arr + i)->hargaTiket
             << setw(7)  << (arr + i)->kursiTersedia
             << "\n";
    }
    cetakGaris('-');
    cout << "   Total data tersimpan: " << jumlah << " kereta.\n";
}

//   TAMBAH KERETA BARU
void tambahKereta(Kereta* arr, int& jumlah) {
    if (jumlah >= MAX_KERETA) {
        cout << "\n   Database penuh! Maksimal " << MAX_KERETA << " kereta.\n";
        return;
    }

    cout << "\n";
    cetakGaris('-');
    cout << "   TAMBAH DATA KERETA BARU\n";
    cetakGaris('-');
    cout << "   (Isi setiap kolom, tekan Enter untuk melanjutkan)\n\n";

    // Cek duplikat nomor kereta sebelum menerima data yang lain
    int nomorBaru;
    bool duplikat;
    do {
        duplikat = false;
        nomorBaru = inputAngka("   Nomor Kereta      : ", 1, 99999);
        for (int i = 0; i < jumlah; i++) {
            if ((arr + i)->nomorKereta == nomorBaru) {
                cout << "   Nomor " << nomorBaru
                     << " sudah dipakai oleh kereta \""
                     << (arr + i)->namaKereta
                     << "\". Pakai nomor lain.\n";
                duplikat = true;
                break;
            }
        }
    } while (duplikat);

    // Kita tampung dulu di variabel lokal, baru salin ke array
    Kereta baru;
    baru.nomorKereta   = nomorBaru;
    baru.namaKereta    = inputTeks("   Nama Kereta       : ");
    baru.asal          = inputTeks("   Kota Asal         : ");
    baru.tujuan        = inputTeks("   Kota Tujuan       : ");
    baru.jamBerangkat  = inputJam ("   Jam Berangkat     : ");
    baru.jamTiba       = inputJam ("   Jam Tiba          : ");
    baru.hargaTiket    = inputAngka("   Harga Tiket (Rp)  : ", 1, 9999999);
    baru.kursiTersedia = inputAngka("   Kursi Tersedia    : ", 0, 1000);

    *(arr + jumlah) = baru;
    jumlah++;   

    cout << "\n   Data \"" << baru.namaKereta << "\" berhasil disimpan"
         << " (sekarang ada " << jumlah << " kereta).\n";
}

//   SWAP - Tukar dua data kereta via pointer
void swapKereta(Kereta* a, Kereta* b) {
    Kereta temp = *a;  // simpan isi rak-a ke temp
    *a = *b;           // isi rak-a diganti dengan isi rak-b
    *b = temp;         // isi rak-b diganti dengan temp (isi rak-a semula)
}

//   LINEAR SEARCH - Cari berdasarkan rute (asal & tujuan)
void linearSearchRute(Kereta* arr, int jumlah) {
    if (jumlah == 0) {
        cout << "\n   Belum ada data kereta. Tambah dulu lewat menu 2.\n";
        return;
    }

    cout << "\n";
    cetakGaris('-');
    cout << "   CARI KERETA BERDASARKAN RUTE  (Linear Search)\n";
    cetakGaris('-');
    cout << "   Linear Search memeriksa data SATU PER SATU dari awal.\n\n";

    string cariAsal   = inputTeks("   Kota Asal yang dicari   : ");
    string cariTujuan = inputTeks("   Kota Tujuan yang dicari : ");

    // Ubah ke huruf kecil semua agar pencarian tidak sensitif huruf besar/kecil
    // Contoh: "jakarta" == "Jakarta" == "JAKARTA"
    string asalKecil = cariAsal, tujuanKecil = cariTujuan;
    transform(asalKecil.begin(),   asalKecil.end(),   asalKecil.begin(),   ::tolower);
    transform(tujuanKecil.begin(), tujuanKecil.end(), tujuanKecil.begin(), ::tolower);

    cout << "\n   Proses pencarian (setiap baris = satu pemeriksaan):\n\n";
    cetakGaris('-', 72);
    cout << left
         << setw(6)  << "Cek"
         << setw(8)  << "No.KA"
         << setw(22) << "Nama Kereta"
         << setw(14) << "Asal"
         << setw(14) << "Tujuan"
         << "Hasil\n";
    cetakGaris('-', 72);

    bool adaHasil = false;
    int  langkah  = 0;

    for (int i = 0; i < jumlah; i++) {
        langkah++;

        string asalData   = (arr + i)->asal;
        string tujuanData = (arr + i)->tujuan;
        transform(asalData.begin(),   asalData.end(),   asalData.begin(),   ::tolower);
        transform(tujuanData.begin(), tujuanData.end(), tujuanData.begin(), ::tolower);

        bool cocok = (asalData == asalKecil && tujuanData == tujuanKecil);

        // Tampilkan proses tiap langkah (wajib sesuai syarat)
        cout << left
             << setw(6)  << langkah
             << setw(8)  << (arr + i)->nomorKereta
             << setw(22) << (arr + i)->namaKereta
             << setw(14) << (arr + i)->asal
             << setw(14) << (arr + i)->tujuan
             << (cocok ? "[COCOK]" : "-")
             << "\n";

        if (cocok) adaHasil = true;
    }

    cetakGaris('-', 72);
    cout << "   Selesai. Total langkah pemeriksaan: " << langkah << "\n";

    if (adaHasil) {
        cout << "\n   Kereta yang melayani rute "
             << cariAsal << " -> " << cariTujuan << ":\n\n";
        cetakGaris('-');
        cout << left
             << setw(8)  << "No.KA"
             << setw(22) << "Nama Kereta"
             << setw(14) << "Asal"
             << setw(14) << "Tujuan"
             << setw(8)  << "Brkt"
             << setw(8)  << "Tiba"
             << setw(14) << "Harga (Rp)"
             << setw(7)  << "Kursi"
             << "\n";
        cetakGaris('-');
        for (int i = 0; i < jumlah; i++) {
            string asalData   = (arr + i)->asal;
            string tujuanData = (arr + i)->tujuan;
            transform(asalData.begin(),   asalData.end(),   asalData.begin(),   ::tolower);
            transform(tujuanData.begin(), tujuanData.end(), tujuanData.begin(), ::tolower);
            if (asalData == asalKecil && tujuanData == tujuanKecil) {
                cout << left
                     << setw(8)  << (arr + i)->nomorKereta
                     << setw(22) << (arr + i)->namaKereta
                     << setw(14) << (arr + i)->asal
                     << setw(14) << (arr + i)->tujuan
                     << setw(8)  << (arr + i)->jamBerangkat
                     << setw(8)  << (arr + i)->jamTiba
                     << setw(14) << (arr + i)->hargaTiket
                     << setw(7)  << (arr + i)->kursiTersedia
                     << "\n";
            }
        }
        cetakGaris('-');
    } else {
        cout << "\n   Tidak ada kereta untuk rute "
             << cariAsal << " -> " << cariTujuan << ".\n";
        cout << "   Pastikan nama kota sudah benar (tidak perlu huruf besar).\n";
    }
}

//   SORT SEMENTARA BERDASARKAN NOMOR (untuk Jump Search)
void urutByNomor(Kereta* arr, int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < jumlah; j++) {
            if ((arr + j)->nomorKereta < (arr + minIdx)->nomorKereta)
                minIdx = j;
        }
        if (minIdx != i)
            swapKereta(arr + i, arr + minIdx);
    }
}

//   JUMP SEARCH - Cari berdasarkan nomor kereta
void jumpSearchNomor(Kereta* arr, int jumlah) {
    if (jumlah == 0) {
        cout << "\n   Belum ada data kereta. Tambah dulu lewat menu 2.\n";
        return;
    }

    cout << "\n";
    cetakGaris('-');
    cout << "   CARI KERETA BERDASARKAN NOMOR  (Jump Search)\n";
    cetakGaris('-');
    cout << "   Jump Search melompat blok demi blok, lalu cek satu per satu di blok terakhir.\n\n";

    int cariNomor = inputAngka("   Masukkan nomor kereta yang dicari : ", 1, 99999);

    // Urutkan dulu karena Jump Search butuh data terurut
    cout << "\n   Mengurutkan data berdasarkan nomor kereta...\n";
    urutByNomor(arr, jumlah);
    cout << "   Data sudah diurutkan.\n\n";

    int langkah  = (int)sqrt((double)jumlah); 
    int prev     = 0;     
    int iterasi  = 0;
    bool ketemu  = false;
    int  posisi  = -1;

    cout << "   Jumlah data: " << jumlah
         << "  |  Ukuran lompatan: " << langkah << "\n\n";
    cetakGaris('-', 65);
    cout << left
         << setw(6)  << "Iter"
         << setw(14) << "Aksi"
         << setw(8)  << "Index"
         << setw(10) << "No.KA"
         << "Keterangan\n";
    cetakGaris('-', 65);

    //  lompat blok demi blok 
    while ((arr + min(langkah, jumlah) - 1)->nomorKereta < cariNomor) {
        iterasi++;
        int ujungBlok = min(langkah, jumlah) - 1;
        cout << setw(6)  << iterasi
             << setw(14) << "Lompat"
             << setw(8)  << ujungBlok
             << setw(10) << (arr + ujungBlok)->nomorKereta
             << "Nilai di ujung blok masih lebih kecil, lompat lagi\n";
        prev    = langkah;
        langkah += (int)sqrt((double)jumlah);
        if (prev >= jumlah) {
            cout << "\n   Nomor " << cariNomor << " tidak ada dalam data.\n";
            return;
        }
    }

    int batasAtas = min(langkah, jumlah) - 1;
    cout << "\n   Nomor target kemungkinan ada di blok index ["
         << prev << " - " << batasAtas << "]. Cek satu per satu:\n\n";

    //  cek satu per satu dalam blok sampai ketemu atau melewati batas atas
    while ((arr + prev)->nomorKereta < cariNomor) {
        iterasi++;
        cout << setw(6)  << iterasi
             << setw(14) << "Cek"
             << setw(8)  << prev
             << setw(10) << (arr + prev)->nomorKereta
             << "Belum ketemu, geser ke kanan\n";
        prev++;
        if (prev > batasAtas) {
            cout << "\n   Nomor " << cariNomor << " tidak ada dalam data.\n";
            return;
        }
    }

    // Periksa apakah data di posisi prev benar-benar cocok
    iterasi++;
    if ((arr + prev)->nomorKereta == cariNomor) {
        cout << setw(6)  << iterasi
             << setw(14) << "Cek"
             << setw(8)  << prev
             << setw(10) << (arr + prev)->nomorKereta
             << "[KETEMU]\n";
        ketemu = true;
        posisi = prev;
    }

    cetakGaris('-', 65);
    cout << "   Selesai. Total langkah: " << iterasi << "\n\n";

    if (ketemu) {
        cout << "   Kereta ditemukan di index ke-" << posisi
             << " (urutan ke-" << (posisi + 1) << " setelah diurutkan).\n\n";
        cetakGaris('-');
        cout << "   Detail:\n";
        cetakGaris('-');
        cout << "   Nomor Kereta   : " << (arr + posisi)->nomorKereta   << "\n"
             << "   Nama Kereta    : " << (arr + posisi)->namaKereta    << "\n"
             << "   Rute           : " << (arr + posisi)->asal
                                       << " -> "  << (arr + posisi)->tujuan << "\n"
             << "   Jam Berangkat  : " << (arr + posisi)->jamBerangkat  << "\n"
             << "   Jam Tiba       : " << (arr + posisi)->jamTiba       << "\n"
             << "   Harga Tiket    : Rp " << (arr + posisi)->hargaTiket << "\n"
             << "   Kursi Tersedia : " << (arr + posisi)->kursiTersedia << "\n";
        cetakGaris('-');
    } else {
        cout << "   Nomor " << cariNomor << " tidak ditemukan.\n";
    }
}

//   MERGE SORT - Urutkan nama kereta (A-Z)
void gabungkanTerurut(Kereta* arr, int kiri, int tengah, int kanan) {
    int ukuranKiri   = tengah - kiri + 1;
    int ukuranKanan  = kanan - tengah;

    // Buat array sementara untuk menyimpan dua bagian
    Kereta* bagianKiri  = new Kereta[ukuranKiri];
    Kereta* bagianKanan = new Kereta[ukuranKanan];

    for (int i = 0; i < ukuranKiri;  i++) *(bagianKiri  + i) = *(arr + kiri + i);
    for (int j = 0; j < ukuranKanan; j++) *(bagianKanan + j) = *(arr + tengah + 1 + j);

    int i = 0, j = 0, k = kiri;
    while (i < ukuranKiri && j < ukuranKanan) {
        string namaKiri  = (bagianKiri  + i)->namaKereta;
        string namaKanan = (bagianKanan + j)->namaKereta;
        transform(namaKiri.begin(),  namaKiri.end(),  namaKiri.begin(),  ::tolower);
        transform(namaKanan.begin(), namaKanan.end(), namaKanan.begin(), ::tolower);

        if (namaKiri <= namaKanan) {
            *(arr + k) = *(bagianKiri  + i);
            i++;
        } else {
            *(arr + k) = *(bagianKanan + j);
            j++;
        }
        k++;
    }
    // Salin sisa yang belum digabung
    while (i < ukuranKiri)  { *(arr + k) = *(bagianKiri  + i); i++; k++; }
    while (j < ukuranKanan) { *(arr + k) = *(bagianKanan + j); j++; k++; }

    delete[] bagianKiri;
    delete[] bagianKanan;
}

void mergeSortRekursif(Kereta* arr, int kiri, int kanan) {
    if (kiri < kanan) {
        int tengah = kiri + (kanan - kiri) / 2;
        mergeSortRekursif(arr, kiri, tengah);       // urutkan bagian kiri
        mergeSortRekursif(arr, tengah + 1, kanan);  // urutkan bagian kanan
        gabungkanTerurut(arr, kiri, tengah, kanan); // gabungkan keduanya
    }
}

void mergeSortNama(Kereta* arr, int jumlah) {
    if (jumlah == 0) {
        cout << "\n   Belum ada data kereta.\n";
        return;
    }
    if (jumlah == 1) {
        cout << "\n   Hanya 1 data, tidak perlu diurutkan.\n";
        tampilSemuaKereta(arr, jumlah);
        return;
    }

    cout << "\n";
    cetakGaris('-');
    cout << "   URUTKAN NAMA KERETA A-Z  (Merge Sort)\n";
    cetakGaris('-');
    cout << "   Merge Sort bekerja dengan membagi data menjadi dua bagian\n"
         << "   terus menerus, lalu menggabungkannya kembali secara terurut.\n\n";

    cout << "   Data SEBELUM diurutkan:\n";
    tampilSemuaKereta(arr, jumlah);

    mergeSortRekursif(arr, 0, jumlah - 1);

    cout << "\n   Pengurutan selesai.\n\n";
    cout << "   Data SESUDAH diurutkan (A-Z berdasarkan nama kereta):\n";
    tampilSemuaKereta(arr, jumlah);
}

//   SELECTION SORT - Urutkan berdasarkan harga tiket (termurah)
void selectionSortHarga(Kereta* arr, int jumlah) {
    if (jumlah == 0) {
        cout << "\n   Belum ada data kereta.\n";
        return;
    }
    if (jumlah == 1) {
        cout << "\n   Hanya 1 data, tidak perlu diurutkan.\n";
        tampilSemuaKereta(arr, jumlah);
        return;
    }

    cout << "\n";
    cetakGaris('-');
    cout << "   URUTKAN HARGA TIKET TERMURAH  (Selection Sort)\n";
    cetakGaris('-');
    cout << "   Selection Sort setiap putaran mencari harga termurah\n"
         << "   dari sisa data dan menempatkannya ke posisi yang benar.\n\n";

    cout << "   Data SEBELUM diurutkan:\n";
    tampilSemuaKereta(arr, jumlah);

    cout << "\n   Proses pengurutan:\n";
    cetakGaris('-', 72);
    cout << left
         << setw(8) << "Putaran"
         << setw(24) << "Kereta Dipindah"
         << setw(12) << "Harga"
         << setw(24) << "Ditukar dengan"
         << "Harga\n";
    cetakGaris('-', 72);

    for (int i = 0; i < jumlah - 1; i++) {
        int posMin = i;
        for (int j = i + 1; j < jumlah; j++) {
            if ((arr + j)->hargaTiket < (arr + posMin)->hargaTiket)
                posMin = j;
        }

        if (posMin != i) {
            cout << setw(8)  << (i + 1)
                 << setw(24) << (arr + i)->namaKereta
                 << setw(12) << (arr + i)->hargaTiket
                 << setw(24) << (arr + posMin)->namaKereta
                 << (arr + posMin)->hargaTiket << "\n";
            swapKereta(arr + i, arr + posMin);
        } else {
            cout << setw(8)  << (i + 1)
                 << setw(24) << (arr + i)->namaKereta
                 << setw(12) << (arr + i)->hargaTiket
                 << "(sudah di posisi yang benar)\n";
        }
    }

    cetakGaris('-', 72);
    cout << "\n   Pengurutan selesai.\n\n";
    cout << "   Data SESUDAH diurutkan (dari harga termurah):\n";
    tampilSemuaKereta(arr, jumlah);
}

void isiDataAwal(Kereta* arr, int& jumlah) {
    Kereta contoh[] = {
        {101, "Argo Bromo Anggrek", "Jakarta",    "Surabaya",   "08:00", "17:30", 580000, 50},
        {205, "Gajayana",           "Malang",     "Jakarta",    "17:00", "05:00", 450000, 64},
        {302, "Bima",               "Surabaya",   "Bandung",    "14:00", "23:00", 320000, 80},
        {418, "Taksaka",            "Yogyakarta", "Jakarta",    "09:00", "15:30", 275000, 72},
        {155, "Lodaya",             "Bandung",    "Solo",       "07:15", "13:00", 210000, 56},
        {267, "Sancaka",            "Surabaya",   "Yogyakarta", "06:30", "11:00", 195000, 96},
        {389, "Brantas",            "Blitar",     "Jakarta",    "15:00", "04:30", 250000, 88},
        {512, "Malioboro",          "Yogyakarta", "Surabaya",   "10:00", "14:30", 185000, 60},
    };

    int n = (int)(sizeof(contoh) / sizeof(contoh[0]));
    for (int i = 0; i < n && jumlah < MAX_KERETA; i++) {
        // [PTR] arr + jumlah = slot berikutnya di array utama
        *(arr + jumlah) = contoh[i];
        jumlah++;
    }
}

// ================================================================
//   MENU UTAMA
// ================================================================
void tampilMenu() {
    cout << "\n";
    cetakGaris('=');
    cout << "   MENU UTAMA\n";
    cetakGaris('=');
    cout << "   1. Tampilkan semua jadwal kereta\n";
    cout << "   2. Tambah data kereta baru\n";
    cout << "   3. Cari kereta berdasarkan rute  (Linear Search)\n";
    cout << "   4. Cari kereta berdasarkan nomor (Jump Search)\n";
    cout << "   5. Urutkan nama kereta A-Z        (Merge Sort)\n";
    cout << "   6. Urutkan harga tiket termurah   (Selection Sort)\n";
    cout << "   0. Keluar\n";
    cetakGaris('=');
}

// ================================================================
//   MAIN
// ================================================================
int main() {
    Kereta dataKereta[MAX_KERETA];
    int jumlahKereta = 0;

    isiDataAwal(dataKereta, jumlahKereta);

    cetakHeader();
    cout << "   Selamat datang!\n";
    cout << "   " << jumlahKereta << " data kereta contoh sudah dimuat.\n";

    int pilihan;
    do {
        tampilMenu();
        pilihan = inputAngka("   Pilih menu (0-6): ", 0, 6);

        switch (pilihan) {
            case 1:
                cout << "\n";
                cetakGaris('-');
                cout << "   SEMUA JADWAL KERETA\n";
                cetakGaris('-');
                // [PTR] &dataKereta[0] = alamat elemen ke-0 = awal array
                tampilSemuaKereta(&dataKereta[0], jumlahKereta);
                break;

            case 2:
                tambahKereta(&dataKereta[0], jumlahKereta);
                break;

            case 3:
                linearSearchRute(&dataKereta[0], jumlahKereta);
                break;

            case 4:
                jumpSearchNomor(&dataKereta[0], jumlahKereta);
                break;

            case 5:
                mergeSortNama(&dataKereta[0], jumlahKereta);
                break;

            case 6:
                selectionSortHarga(&dataKereta[0], jumlahKereta);
                break;

            case 0:
                cout << "\n   Sampai jumpa!\n\n";
                break;

            default:
                cout << "\n   Pilihan tidak ada. Ketik angka 0 sampai 6.\n";
        }

        if (pilihan != 0) {
            cout << "\n   Tekan Enter untuk kembali ke menu...";
            string dummy;
            getline(cin, dummy);
        }

    } while (pilihan != 0);

    return 0;
}
