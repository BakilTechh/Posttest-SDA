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

//   [STRUCT] DEFINISI STRUCT KERETA
struct Kereta {
    int    nomorKereta;    // contoh: 101
    string namaKereta;     // contoh: "Argo Bromo Anggrek"
    string asal;           // contoh: "Jakarta"
    string tujuan;         // contoh: "Surabaya"
    string jamBerangkat;   // format HH:MM
    string jamTiba;        // format HH:MM
    int    hargaTiket;     // dalam rupiah
    int    kursiTersedia;  // jumlah kursi kosong
};

//   [STRUCT] DEFINISI STRUCT PEMESANAN
struct Pemesanan {                    
    int    nomorAntrian;   
    string namaPenumpang;  
    int    nomorKereta;    
    string namaKereta;
    string asal;
    string tujuan;
    string jamBerangkat;
    string jamTiba;
    int    hargaTiket;
};


struct Transaksi {
    int    nomorTransaksi; 
    Pemesanan data;       
    string statusBatal;    
};

const int MAX_QUEUE = 20;            

struct Queue {                       
    Pemesanan data[MAX_QUEUE];        
    int front;   
    int rear;   
    int size;  
    int counterAntrian;
};

//   [STRUCT] STRUCT STACK (Riwayat transaksi berbasis array)
const int MAX_STACK = 50;

struct Stack { 
    Transaksi data[MAX_STACK];
    int top;
    int counterTransaksi;
};

const int MAX_KERETA = 100;

// FUNGSI BANTU INPUT
void bersihkanBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void resetCin() { cin.clear(); bersihkanBuffer(); }

string trim(const string& s) {
    size_t awal = s.find_first_not_of(" \t\r\n");
    if (awal == string::npos) return "";
    size_t akhir = s.find_last_not_of(" \t\r\n");
    return s.substr(awal, akhir - awal + 1);
}

int inputAngka(const string& prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        string baris;
        if (!getline(cin, baris)) { resetCin(); cout << "   Gangguan input, coba lagi.\n"; continue; }
        string s = trim(baris);
        if (s.empty()) { cout << "   Tidak boleh kosong. Ketik angka.\n"; continue; }
        size_t mulai = (s[0] == '-') ? 1 : 0;
        bool ok = (mulai < s.size());
        for (size_t k = mulai; k < s.size(); k++)
            if (!isdigit((unsigned char)s[k])) { ok = false; break; }
        if (!ok) { cout << "   \"" << s << "\" bukan angka. Contoh: 42\n"; continue; }
        try {
            long long tmp = stoll(s);
            if (tmp < minVal || tmp > maxVal) {
                cout << "   Angka harus antara " << minVal << " sampai " << maxVal << ".\n";
                continue;
            }
            return (int)tmp;
        } catch (...) { cout << "   Angka terlalu besar.\n"; }
    }
}
int inputAngka(const string& prompt) { return inputAngka(prompt, INT_MIN, INT_MAX); }

string inputTeks(const string& prompt) {
    while (true) {
        cout << prompt;
        string baris;
        if (!getline(cin, baris)) { resetCin(); cout << "   Gangguan input, coba lagi.\n"; continue; }
        string s = trim(baris);
        if (s.empty()) { cout << "   Tidak boleh kosong atau hanya spasi.\n"; continue; }
        return s;
    }
}

string inputJam(const string& prompt) {
    while (true) {
        cout << prompt;
        string baris;
        if (!getline(cin, baris)) { resetCin(); continue; }
        string jam = trim(baris);
        if (jam.empty())                          { cout << "   Jam tidak boleh kosong. Contoh: 07:30\n"; continue; }
        if (jam.length() != 5 || jam[2] != ':')   { cout << "   Format salah. Harus HH:MM, contoh: 07:30\n"; continue; }
        bool ok = true;
        for (int k = 0; k < 5; k++) { if (k==2) continue; if (!isdigit((unsigned char)jam[k])) { ok=false; break; } }
        if (!ok) { cout << "   Jam dan menit harus angka.\n"; continue; }
        int hh = stoi(jam.substr(0,2)), mm = stoi(jam.substr(3,2));
        if (hh > 23 || mm > 59) { cout << "   Jam 00-23, menit 00-59.\n"; continue; }
        return jam;
    }
}

// ================================================================
//   FUNGSI TAMPILAN UMUM
// ================================================================
void cetakGaris(char c = '=', int p = 80) { for (int i=0;i<p;i++) cout<<c; cout<<"\n"; }

void cetakHeader() {
    cetakGaris('=');
    cout << "         SISTEM MANAJEMEN KEBERANGKATAN KERETA API\n";
    cetakGaris('=');
}

//TAMPIL SEMUA KERETA
void tampilSemuaKereta(Kereta* arr, int jumlah) {
    if (jumlah == 0) { cout << "\n   Belum ada data kereta.\n"; return; }
    cetakGaris('-');
    cout << left << setw(5)<<"No" << setw(8)<<"No.KA" << setw(22)<<"Nama Kereta"
         << setw(14)<<"Asal" << setw(14)<<"Tujuan" << setw(8)<<"Brkt"
         << setw(8)<<"Tiba" << setw(14)<<"Harga (Rp)" << setw(7)<<"Kursi" << "\n";
    cetakGaris('-');
    for (int i = 0; i < jumlah; i++) {
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
    cout << "   Total: " << jumlah << " kereta.\n";
}

//   TAMBAH KERETA BARU
void tambahKereta(Kereta* arr, int& jumlah) { // [PTR-PARAM]
    if (jumlah >= MAX_KERETA) { cout << "\n   Database penuh!\n"; return; }
    cout << "\n"; cetakGaris('-');
    cout << "   TAMBAH DATA KERETA BARU\n"; cetakGaris('-');
    cout << "   (Isi setiap kolom, tekan Enter untuk melanjutkan)\n\n";
    int nomorBaru; bool duplikat;
    do {
        duplikat = false;
        nomorBaru = inputAngka("   Nomor Kereta      : ", 1, 99999);
        for (int i = 0; i < jumlah; i++) {
            if ((arr + i)->nomorKereta == nomorBaru) {
                cout << "   Nomor " << nomorBaru << " sudah dipakai oleh kereta \""
                     << (arr + i)->namaKereta << "\". Pakai nomor lain.\n";
                duplikat = true; break;
            }
        }
    } while (duplikat);
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
    cout << "\n   Data \"" << baru.namaKereta << "\" berhasil disimpan.\n";
}

//   SWAP DATA KERETA
void swapKereta(Kereta* a, Kereta* b) {
    Kereta temp = *a;
    *a = *b;      
    *b = temp;  
}

//   LINEAR SEARCH
void linearSearchRute(Kereta* arr, int jumlah) {
    if (jumlah == 0) { cout << "\n   Belum ada data kereta.\n"; return; }
    cout << "\n"; cetakGaris('-');
    cout << "   CARI KERETA BERDASARKAN RUTE  --  Linear Search\n"; cetakGaris('-');
    string cariAsal   = inputTeks("   Kota Asal   : ");
    string cariTujuan = inputTeks("   Kota Tujuan : ");
    string asalKecil = cariAsal, tujuanKecil = cariTujuan;
    transform(asalKecil.begin(), asalKecil.end(), asalKecil.begin(), ::tolower);
    transform(tujuanKecil.begin(), tujuanKecil.end(), tujuanKecil.begin(), ::tolower);
    cout << "\n   Proses pencarian:\n\n";
    cetakGaris('-', 72);
    cout << left << setw(6)<<"Cek" << setw(8)<<"No.KA" << setw(22)<<"Nama Kereta"
         << setw(14)<<"Asal" << setw(14)<<"Tujuan" << "Hasil\n";
    cetakGaris('-', 72);
    bool adaHasil = false; int langkah = 0;
    for (int i = 0; i < jumlah; i++) {
        langkah++;
        string ad = (arr+i)->asal, td = (arr+i)->tujuan;
        transform(ad.begin(), ad.end(), ad.begin(), ::tolower);
        transform(td.begin(), td.end(), td.begin(), ::tolower);
        bool cocok = (ad == asalKecil && td == tujuanKecil);
        cout << left << setw(6)<<langkah << setw(8)<<(arr+i)->nomorKereta
             << setw(22)<<(arr+i)->namaKereta << setw(14)<<(arr+i)->asal
             << setw(14)<<(arr+i)->tujuan << (cocok?"[COCOK]":"-") << "\n";
        if (cocok) adaHasil = true;
    }
    cetakGaris('-', 72);
    cout << "   Selesai. Total langkah: " << langkah << "\n";
    if (adaHasil) {
        cout << "\n   Kereta untuk rute " << cariAsal << " -> " << cariTujuan << ":\n\n";
        cetakGaris('-');
        cout << left << setw(8)<<"No.KA" << setw(22)<<"Nama Kereta" << setw(14)<<"Asal"
             << setw(14)<<"Tujuan" << setw(8)<<"Brkt" << setw(8)<<"Tiba"
             << setw(14)<<"Harga (Rp)" << setw(7)<<"Kursi" << "\n";
        cetakGaris('-');
        for (int i = 0; i < jumlah; i++) {
            string ad=(arr+i)->asal, td=(arr+i)->tujuan;
            transform(ad.begin(),ad.end(),ad.begin(),::tolower);
            transform(td.begin(),td.end(),td.begin(),::tolower);
            if (ad==asalKecil && td==tujuanKecil)
                cout << left << setw(8)<<(arr+i)->nomorKereta << setw(22)<<(arr+i)->namaKereta
                     << setw(14)<<(arr+i)->asal << setw(14)<<(arr+i)->tujuan
                     << setw(8)<<(arr+i)->jamBerangkat << setw(8)<<(arr+i)->jamTiba
                     << setw(14)<<(arr+i)->hargaTiket << setw(7)<<(arr+i)->kursiTersedia << "\n";
        }
        cetakGaris('-');
    } else {
        cout << "\n   Tidak ada kereta untuk rute " << cariAsal << " -> " << cariTujuan << ".\n";
    }
}

//   JUMP SEARCH
void urutByNomor(Kereta* arr, int jumlah) {
    for (int i = 0; i < jumlah-1; i++) {
        int minIdx = i;
        for (int j = i+1; j < jumlah; j++)
            if ((arr+j)->nomorKereta < (arr+minIdx)->nomorKereta) minIdx = j;
        if (minIdx != i) swapKereta(arr+i, arr+minIdx);
    }
}

void jumpSearchNomor(Kereta* arr, int jumlah) {
    if (jumlah == 0) { cout << "\n   Belum ada data kereta.\n"; return; }
    cout << "\n"; cetakGaris('-');
    cout << "   CARI KERETA BERDASARKAN NOMOR  --  Jump Search\n"; cetakGaris('-');
    int cariNomor = inputAngka("   Nomor kereta yang dicari : ", 1, 99999);
    cout << "\n   Mengurutkan data berdasarkan nomor kereta...\n";
    urutByNomor(arr, jumlah);
    cout << "   Data sudah terurut.\n\n";
    int langkah = (int)sqrt((double)jumlah), prev = 0, iterasi = 0;
    bool ketemu = false; int posisi = -1;
    cout << "   Jumlah data: " << jumlah << "  |  Ukuran lompatan: " << langkah << "\n\n";
    cetakGaris('-', 70);
    cout << left << setw(6)<<"Iter" << setw(12)<<"Tahap" << setw(10)<<"Index"
         << setw(10)<<"No.KA" << "Keterangan\n";
    cetakGaris('-', 70);
    while ((arr + min(langkah,jumlah) - 1)->nomorKereta < cariNomor) {
        iterasi++;
        int ujung = min(langkah,jumlah)-1;
        cout << setw(6)<<iterasi << setw(12)<<"Lompat" << setw(10)<<ujung
             << setw(10)<<(arr+ujung)->nomorKereta
             << "Ujung blok [" << ujung << "] lebih kecil, lompat\n";
        prev = langkah; langkah += (int)sqrt((double)jumlah);
        if (prev >= jumlah) {
            cetakGaris('-', 70);
            cout << "\n   Nomor " << cariNomor << " tidak ada dalam data.\n"; return;
        }
    }
    int batasAtas = min(langkah,jumlah)-1;
    cout << "\n   Blok target: index [" << prev << " - " << batasAtas << "]. Cek satu per satu:\n\n";
    while ((arr+prev)->nomorKereta < cariNomor) {
        iterasi++;
        cout << setw(6)<<iterasi << setw(12)<<"Cek" << setw(10)<<prev
             << setw(10)<<(arr+prev)->nomorKereta << "Belum cocok, geser\n";
        prev++;
        if (prev > batasAtas) {
            cetakGaris('-', 70);
            cout << "\n   Nomor " << cariNomor << " tidak ada.\n"; return;
        }
    }
    iterasi++;
    if ((arr+prev)->nomorKereta == cariNomor) {
        cout << setw(6)<<iterasi << setw(12)<<"Cek" << setw(10)<<prev
             << setw(10)<<(arr+prev)->nomorKereta << "[KETEMU]\n";
        ketemu = true; posisi = prev;
    }
    cetakGaris('-', 70);
    cout << "   Selesai. Total langkah: " << iterasi << "\n\n";
    if (ketemu) {
        cout << "   Kereta ditemukan di index ke-" << posisi << ".\n\n";
        cetakGaris('-');
        cout << "   Nomor Kereta   : " << (arr+posisi)->nomorKereta  << "\n"
             << "   Nama Kereta    : " << (arr+posisi)->namaKereta   << "\n"
             << "   Rute           : " << (arr+posisi)->asal << " -> " << (arr+posisi)->tujuan << "\n"
             << "   Jam Berangkat  : " << (arr+posisi)->jamBerangkat << "\n"
             << "   Jam Tiba       : " << (arr+posisi)->jamTiba      << "\n"
             << "   Harga Tiket    : Rp " << (arr+posisi)->hargaTiket << "\n"
             << "   Kursi Tersedia : " << (arr+posisi)->kursiTersedia << "\n";
        cetakGaris('-');
    } else { cout << "   Nomor " << cariNomor << " tidak ditemukan.\n"; }
}

//   MERGE SORT
void gabungkanTerurut(Kereta* arr, int kiri, int tengah, int kanan) {
    int ukL = tengah-kiri+1, ukR = kanan-tengah;
    Kereta* L = new Kereta[ukL];
    Kereta* R = new Kereta[ukR];
    for (int i=0;i<ukL;i++) *(L+i) = *(arr+kiri+i);
    for (int j=0;j<ukR;j++) *(R+j) = *(arr+tengah+1+j);
    int i=0, j=0, k=kiri;
    while (i<ukL && j<ukR) {
        string nL=(L+i)->namaKereta, nR=(R+j)->namaKereta;
        transform(nL.begin(),nL.end(),nL.begin(),::tolower);
        transform(nR.begin(),nR.end(),nR.begin(),::tolower);
        if (nL<=nR) { *(arr+k)=*(L+i); i++; }
        else        { *(arr+k)=*(R+j); j++; }
        k++;
    }
    while (i<ukL) { *(arr+k)=*(L+i); i++; k++; }
    while (j<ukR) { *(arr+k)=*(R+j); j++; k++; }
    delete[] L; delete[] R;
}
void mergeSortRekursif(Kereta* arr, int kiri, int kanan) {
    if (kiri < kanan) {
        int tengah = kiri + (kanan-kiri)/2;
        mergeSortRekursif(arr, kiri, tengah);
        mergeSortRekursif(arr, tengah+1, kanan);
        gabungkanTerurut(arr, kiri, tengah, kanan);
    }
}
void mergeSortNama(Kereta* arr, int jumlah) {
    if (jumlah==0) { cout<<"\n   Belum ada data kereta.\n"; return; }
    if (jumlah==1) { cout<<"\n   Hanya 1 data.\n"; tampilSemuaKereta(arr,jumlah); return; }
    cout << "\n"; cetakGaris('-');
    cout << "   URUTKAN NAMA KERETA A-Z  --  Merge Sort\n"; cetakGaris('-');
    cout << "   Data SEBELUM diurutkan:\n";
    tampilSemuaKereta(arr, jumlah);
    mergeSortRekursif(arr, 0, jumlah-1);
    cout << "\n   Pengurutan selesai.\n\n   Data SESUDAH diurutkan:\n";
    tampilSemuaKereta(arr, jumlah);
}

//   SELECTION SORT
void selectionSortHarga(Kereta* arr, int jumlah) {
    if (jumlah==0) { cout<<"\n   Belum ada data kereta.\n"; return; }
    if (jumlah==1) { cout<<"\n   Hanya 1 data.\n"; tampilSemuaKereta(arr,jumlah); return; }
    cout << "\n"; cetakGaris('-');
    cout << "   URUTKAN HARGA TIKET TERMURAH  --  Selection Sort\n"; cetakGaris('-');
    cout << "   Data SEBELUM diurutkan:\n";
    tampilSemuaKereta(arr, jumlah);
    cout << "\n   Proses pengurutan:\n";
    cetakGaris('-', 75);
    cout << left << setw(9)<<"Putaran" << setw(24)<<"Posisi Saat Ini"
         << setw(12)<<"Harga" << setw(24)<<"Ditukar Dengan" << "Harga\n";
    cetakGaris('-', 75);
    for (int i=0; i<jumlah-1; i++) {
        int posMin = i;
        for (int j=i+1; j<jumlah; j++)
            if ((arr+j)->hargaTiket < (arr+posMin)->hargaTiket) posMin=j;
        if (posMin != i) {
            cout << setw(9)<<(i+1) << setw(24)<<(arr+i)->namaKereta
                 << setw(12)<<(arr+i)->hargaTiket
                 << setw(24)<<(arr+posMin)->namaKereta << (arr+posMin)->hargaTiket << "\n";
            swapKereta(arr+i, arr+posMin);
        } else {
            cout << setw(9)<<(i+1) << setw(24)<<(arr+i)->namaKereta
                 << setw(12)<<(arr+i)->hargaTiket << "(sudah di posisi yang benar)\n";
        }
    }
    cetakGaris('-', 75);
    cout << "\n   Pengurutan selesai.\n\n   Data SESUDAH diurutkan:\n";
    tampilSemuaKereta(arr, jumlah);
}

//   DATA AWAL KERETA
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
    int n = (int)(sizeof(contoh)/sizeof(contoh[0]));
    for (int i=0; i<n && jumlah<MAX_KERETA; i++) {
        *(arr+jumlah) = contoh[i];
        jumlah++;
    }
}

//   INISIALISASI QUEUE DAN STACK
void inisialisasiQueue(Queue* q) {
    q->front = 0;
    q->rear  = -1;
    q->size  = 0;
    q->counterAntrian = 0;
}

void inisialisasiStack(Stack* s) {
    s->top = -1;
    s->counterTransaksi = 0;
}

//   QUEUE -- CEK STATUS
bool queueKosong(Queue* q) { return q->size == 0; }           // [UNDERFLOW] 
bool queuePenuh(Queue* q)  { return q->size == MAX_QUEUE; }   // [OVERFLOW]  

//   STACK -- CEK STATUS
bool stackKosong(Stack* s) { return s->top == -1; }                   // [UNDERFLOW] 
bool stackPenuh(Stack* s)  { return s->top == MAX_STACK - 1; }        // [OVERFLOW]  

//   [QUEUE] ENQUEUE -- Tambah penumpang ke antrian
void enqueue(Queue* q, Kereta* dataKereta, int jumlahKereta) {
    if (queuePenuh(q)) { // [OVERFLOW]
        cout << "\n   [OVERFLOW] Antrian sudah penuh! Maksimal " << MAX_QUEUE << " orang.\n";
        cout << "   Tunggu sampai ada antrian yang diproses.\n";
        return;
    }
    if (jumlahKereta == 0) { cout << "\n   Belum ada data kereta. Tambah kereta dulu.\n"; return; }

    cout << "\n"; cetakGaris('-');
    cout << "   PESAN TIKET -- Masuk ke Antrian\n"; cetakGaris('-');

    // Tampilkan daftar kereta yang tersedia untuk dipilih
    cout << "   Pilih kereta yang ingin dipesan:\n\n";
    tampilSemuaKereta(dataKereta, jumlahKereta);

    // Pilih nomor kereta
    int nomorPilihan = inputAngka("\n   Masukkan No.KA yang dipilih : ", 1, 99999);

    // Cari kereta dengan nomor tersebut
    int idxKereta = -1;
    for (int i = 0; i < jumlahKereta; i++) {
        if ((dataKereta + i)->nomorKereta == nomorPilihan) {
            idxKereta = i; break;
        }
    }
    if (idxKereta == -1) {
        cout << "   Nomor kereta " << nomorPilihan << " tidak ditemukan.\n"; return;
    }
    if ((dataKereta + idxKereta)->kursiTersedia == 0) {
        cout << "   Maaf, kereta \"" << (dataKereta+idxKereta)->namaKereta
             << "\" sudah tidak ada kursi tersedia.\n"; return;
    }

    string namaPenumpang = inputTeks("   Nama Penumpang          : ");

    // Geser rear ke posisi berikutnya (circular)
    q->rear = (q->rear + 1) % MAX_QUEUE;
    q->counterAntrian++;

    // Isi data pemesanan ke slot rear
    Pemesanan* p = &q->data[q->rear];
    p->nomorAntrian  = q->counterAntrian;
    p->namaPenumpang = namaPenumpang;
    p->nomorKereta   = (dataKereta + idxKereta)->nomorKereta;
    p->namaKereta    = (dataKereta + idxKereta)->namaKereta;
    p->asal          = (dataKereta + idxKereta)->asal;
    p->tujuan        = (dataKereta + idxKereta)->tujuan;
    p->jamBerangkat  = (dataKereta + idxKereta)->jamBerangkat;
    p->jamTiba       = (dataKereta + idxKereta)->jamTiba;
    p->hargaTiket    = (dataKereta + idxKereta)->hargaTiket;
    q->size++;

    // Kurangi kursi tersedia
    (dataKereta + idxKereta)->kursiTersedia--;

    cout << "\n   Berhasil! " << namaPenumpang
         << " masuk antrian nomor " << q->counterAntrian << ".\n";
    cout << "   Kereta  : " << p->namaKereta << "  ("
         << p->asal << " -> " << p->tujuan << ")\n";
    cout << "   Posisi antrian saat ini: " << q->size << " orang.\n";
}

//   [QUEUE] DEQUEUE -- Proses penumpang terdepan dari antrian
void dequeue(Queue* q, Stack* s) {
    if (queueKosong(q)) {
        cout << "\n   [UNDERFLOW] Antrian kosong! Tidak ada penumpang yang bisa diproses.\n";
        return;
    }

    // Ambil data dari posisi front (terdepan)
    Pemesanan diproses = q->data[q->front];

    // Geser front ke posisi berikutnya (circular)
    q->front = (q->front + 1) % MAX_QUEUE;
    q->size--;

    cout << "\n"; cetakGaris('-');
    cout << "   PROSES TIKET -- Penumpang Terdepan Dilayani\n"; cetakGaris('-');
    cout << "   No. Antrian  : " << diproses.nomorAntrian  << "\n"
         << "   Penumpang    : " << diproses.namaPenumpang  << "\n"
         << "   Kereta       : " << diproses.namaKereta     << "  (No. " << diproses.nomorKereta << ")\n"
         << "   Rute         : " << diproses.asal << " -> " << diproses.tujuan << "\n"
         << "   Jam Berangkat: " << diproses.jamBerangkat   << "\n"
         << "   Jam Tiba     : " << diproses.jamTiba        << "\n"
         << "   Harga Tiket  : Rp " << diproses.hargaTiket  << "\n";
    cetakGaris('-');
    cout << "   Tiket berhasil diproses. Sisa antrian: " << q->size << " orang.\n";

    // Catat ke Stack riwayat (push otomatis)
    if (stackPenuh(s)) { // [OVERFLOW] jika stack penuh, riwayat tidak bisa dicatat
        cout << "   Peringatan: riwayat transaksi penuh, transaksi ini tidak dicatat.\n";
        return;
    }
    s->top++;
    s->counterTransaksi++;
    // isi slot top via pointer s
    Transaksi* t = &s->data[s->top]; // pointer ke slot transaksi baru
    t->nomorTransaksi = s->counterTransaksi;
    t->data           = diproses;
    t->statusBatal    = "Aktif";

    cout << "   Tercatat di riwayat sebagai Transaksi #" << s->counterTransaksi << ".\n";
}

//   [STACK] PUSH -- Catat transaksi ke riwayat (manual)
void push(Stack* s, Transaksi transaksi) {
    if (stackPenuh(s)) { // [OVERFLOW]
        cout << "\n   [OVERFLOW] Riwayat penuh! Maksimal " << MAX_STACK << " transaksi.\n";
        return;
    }
    s->top++;
    s->data[s->top] = transaksi;
    s->counterTransaksi++;
}

//   [STACK] POP -- Batalkan transaksi terakhir
void pop(Stack* s) {
    if (stackKosong(s)) { // [UNDERFLOW]
        cout << "\n   [UNDERFLOW] Riwayat kosong! Tidak ada transaksi yang bisa dibatalkan.\n";
        return;
    }
    // Tandai sebagai dibatalkan sebelum top diturunkan
    s->data[s->top].statusBatal = "Dibatalkan";

    cout << "\n"; cetakGaris('-');
    cout << "   BATALKAN TRANSAKSI TERAKHIR  --  Pop dari Stack\n"; cetakGaris('-');
    cout << "   Transaksi yang dibatalkan:\n";
    cout << "   No. Transaksi : " << s->data[s->top].nomorTransaksi  << "\n"
         << "   Penumpang     : " << s->data[s->top].data.namaPenumpang << "\n"
         << "   Kereta        : " << s->data[s->top].data.namaKereta   << "\n"
         << "   Rute          : " << s->data[s->top].data.asal
                                  << " -> " << s->data[s->top].data.tujuan << "\n"
         << "   Harga Tiket   : Rp " << s->data[s->top].data.hargaTiket << "\n";
    cetakGaris('-');

    s->top--;
    cout << "   Transaksi berhasil dibatalkan.\n";
    cout << "   Sisa riwayat aktif: " << (s->top + 1) << " transaksi.\n";
}


//   PEEK -- Tampil terdepan antrian DAN teratas riwayat
void peek(Queue* q, Stack* s) {
    cout << "\n"; cetakGaris('-');
    cout << "   PEEK -- Lihat Tanpa Menghapus\n"; cetakGaris('-');

    // --- Peek Queue: siapa yang paling depan? ---
    cout << "\n   [ Antrian -- Penumpang Paling Depan ]\n";
    if (queueKosong(q)) { // [UNDERFLOW]
        cout << "   Antrian sedang kosong.\n";
    } else {
        Pemesanan* depan = &q->data[q->front]; // pointer ke elemen front
        cout << "   No. Antrian  : " << depan->nomorAntrian  << "\n"
             << "   Penumpang    : " << depan->namaPenumpang  << "\n"
             << "   Kereta       : " << depan->namaKereta     << "\n"
             << "   Rute         : " << depan->asal << " -> " << depan->tujuan << "\n"
             << "   Harga Tiket  : Rp " << depan->hargaTiket  << "\n";
    }

    cout << "\n";
    cetakGaris('-', 40);

    // --- Peek Stack: transaksi apa yang paling terakhir? ---
    cout << "\n   [ Riwayat -- Transaksi Paling Terakhir ]\n";
    if (stackKosong(s)) { // [UNDERFLOW]
        cout << "   Riwayat masih kosong.\n";
    } else {
        Transaksi* atas = &s->data[s->top]; // pointer ke elemen top
        cout << "   No. Transaksi : " << atas->nomorTransaksi      << "\n"
             << "   Penumpang     : " << atas->data.namaPenumpang   << "\n"
             << "   Kereta        : " << atas->data.namaKereta      << "\n"
             << "   Rute          : " << atas->data.asal
                                      << " -> " << atas->data.tujuan << "\n"
             << "   Harga Tiket   : Rp " << atas->data.hargaTiket   << "\n"
             << "   Status        : " << atas->statusBatal           << "\n";
    }
}

//   [QUEUE] TAMPIL SEMUA ANTRIAN
void tampilAntrian(Queue* q) {
    cout << "\n"; cetakGaris('-');
    cout << "   DAFTAR ANTRIAN PEMBELIAN TIKET\n"; cetakGaris('-');
    if (queueKosong(q)) { // [UNDERFLOW]
        cout << "   Antrian sedang kosong.\n"; return;
    }
    cout << "   Kapasitas: " << q->size << " / " << MAX_QUEUE << " orang\n\n";
    cout << left << setw(6)<<"Posisi" << setw(8)<<"No.Ant" << setw(20)<<"Nama Penumpang"
         << setw(8)<<"No.KA" << setw(22)<<"Nama Kereta"
         << setw(14)<<"Rute" << "Harga\n";
    cetakGaris('-', 80);

    for (int i = 0; i < q->size; i++) {
        // Hitung index sebenarnya (circular array)
        int idx = (q->front + i) % MAX_QUEUE;


        Pemesanan* p = q->data + idx; //pointer ke elemen antrian ke-i

        cout << left
             << setw(6)  << (i + 1)          // posisi dalam antrian (1 = paling depan)
             << setw(8)  << p->nomorAntrian
             << setw(20) << p->namaPenumpang
             << setw(8)  << p->nomorKereta
             << setw(22) << p->namaKereta
             << setw(14) << (p->asal + "->" + p->tujuan)
             << "Rp " << p->hargaTiket << "\n";
    }
    cetakGaris('-', 80);
    cout << "   Keterangan: posisi 1 = paling depan (akan diproses duluan).\n";
}

//   [STACK] TAMPIL SEMUA RIWAYAT TRANSAKSI
void tampilRiwayat(Stack* s) {
    cout << "\n"; cetakGaris('-');
    cout << "   RIWAYAT TRANSAKSI\n"; cetakGaris('-');
    if (stackKosong(s)) { // [UNDERFLOW]
        cout << "   Belum ada riwayat transaksi.\n"; return;
    }
    int jumlahAktif = s->top + 1;
    cout << "   Total riwayat aktif: " << jumlahAktif << " transaksi\n\n";
    cout << left << setw(8)<<"No.Trx" << setw(20)<<"Nama Penumpang"
         << setw(8)<<"No.KA" << setw(22)<<"Nama Kereta"
         << setw(18)<<"Rute" << setw(12)<<"Harga" << "Status\n";
    cetakGaris('-', 90);

    // Tampilkan dari top ke bawah (terbaru ke terlama)
    for (int i = s->top; i >= 0; i--) {
        //(s->data + i) menunjuk ke slot riwayat ke-i
        //s->data adalah alamat awal array Transaksi
        Transaksi* t = s->data + i; //pointer ke elemen riwayat ke-i

        cout << left
             << setw(8)  << t->nomorTransaksi
             << setw(20) << t->data.namaPenumpang
             << setw(8)  << t->data.nomorKereta
             << setw(22) << t->data.namaKereta
             << setw(18) << (t->data.asal + "->" + t->data.tujuan)
             << setw(12) << ("Rp " + to_string(t->data.hargaTiket))
             << t->statusBatal << "\n";
    }
    cetakGaris('-', 90);
    cout << "   Keterangan: urutan dari paling baru (atas) ke paling lama (bawah).\n";
}

//   [MENU] MENU ANTRIAN & RIWAYAT
void tampilMenuAntrianRiwayat() {
    cout << "\n";
    cetakGaris('=');
    cout << "   MENU ANTRIAN & RIWAYAT TRANSAKSI\n";
    cetakGaris('=');
    cout << "   -- QUEUE (Antrian Beli Tiket) --\n";
    cout << "   1. Pesan tiket (Enqueue -- masuk antrian)\n";
    cout << "   2. Proses tiket terdepan (Dequeue -- layani antrian)\n";
    cout << "   3. Lihat semua antrian\n";
    cout << "   -- STACK (Riwayat Transaksi) --\n";
    cout << "   4. Batalkan transaksi terakhir (Pop)\n";
    cout << "   5. Lihat semua riwayat transaksi\n";
    cout << "   -- LAINNYA --\n";
    cout << "   6. Peek (lihat terdepan antrian & teratas riwayat)\n";
    cout << "   0. Kembali ke menu utama\n";
    cetakGaris('=');
}

//   [MENU] MENU UTAMA
void tampilMenuUtama() {
    cout << "\n";
    cetakGaris('=');
    cout << "   MENU UTAMA\n";
    cetakGaris('=');
    cout << "   -- DATA KERETA --\n";
    cout << "   1. Tampilkan semua jadwal kereta\n";
    cout << "   2. Tambah data kereta baru\n";
    cout << "   3. Cari kereta berdasarkan rute  (Linear Search)\n";
    cout << "   4. Cari kereta berdasarkan nomor (Jump Search)\n";
    cout << "   5. Urutkan nama kereta A-Z        (Merge Sort)\n";
    cout << "   6. Urutkan harga tiket termurah   (Selection Sort)\n";
    cout << "   -- TIKET & TRANSAKSI --\n";
    cout << "   7. Antrian & Riwayat Transaksi\n";
    cout << "   0. Keluar\n";
    cetakGaris('=');
}

//   MAIN
int main() {
    Kereta dataKereta[MAX_KERETA];
    int jumlahKereta = 0;
    isiDataAwal(dataKereta, jumlahKereta);

    // [STRUCT] Queue dan Stack
    Queue antrianTiket; 
    Stack riwayatTrx; 
    inisialisasiQueue(&antrianTiket); 
    inisialisasiStack(&riwayatTrx); 

    cetakHeader();
    cout << "   Selamat datang!\n";
    cout << "   " << jumlahKereta << " data kereta contoh sudah dimuat.\n";

    int pilihan;
    do {
        tampilMenuUtama();
        pilihan = inputAngka("   Pilih menu (0-7): ", 0, 7);

        switch (pilihan) {
            case 1:
                cout << "\n"; cetakGaris('-');
                cout << "   SEMUA JADWAL KERETA\n"; cetakGaris('-');
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
            case 7: {
                // Sub-menu antrian & riwayat
                int subPilihan;
                do {
                    tampilMenuAntrianRiwayat();
                    subPilihan = inputAngka("   Pilih menu (0-6): ", 0, 6);
                    switch (subPilihan) {
                        case 1:
                            enqueue(&antrianTiket, &dataKereta[0], jumlahKereta);
                            break;
                        case 2:
                            dequeue(&antrianTiket, &riwayatTrx);
                            break;
                        case 3:
                            tampilAntrian(&antrianTiket);
                            break;
                        case 4:
                            pop(&riwayatTrx);
                            break;
                        case 5:
                            tampilRiwayat(&riwayatTrx);
                            break;
                        case 6:
                            peek(&antrianTiket, &riwayatTrx);
                            break;
                        case 0:
                            cout << "\n   Kembali ke menu utama.\n";
                            break;
                        default:
                            cout << "\n   Pilihan tidak ada.\n";
                    }
                    if (subPilihan != 0) {
                        cout << "\n   Tekan Enter untuk lanjut...";
                        string dummy; getline(cin, dummy);
                    }
                } while (subPilihan != 0);
                break;
            }
            case 0:
                cout << "\n   Sampai jumpa!\n\n";
                break;
            default:
                cout << "\n   Pilihan tidak ada. Ketik angka 0 sampai 7.\n";
        }

        if (pilihan != 0) {
            cout << "\n   Tekan Enter untuk kembali ke menu...";
            string dummy; getline(cin, dummy);
        }
    } while (pilihan != 0);

    return 0;
}