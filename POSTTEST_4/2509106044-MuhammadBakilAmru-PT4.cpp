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

// ================================================================
//   [STRUCT] KERETA
// ================================================================
struct Kereta {
    int    nomorKereta;
    string namaKereta;
    string asal;
    string tujuan;
    string jamBerangkat;
    string jamTiba;
    int    hargaTiket;
    int    kursiTersedia;
};

// ================================================================
//   [STRUCT] PEMESANAN -- data satu tiket yang dipesan
// ================================================================
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

// ================================================================
//   [STRUCT] TRANSAKSI -- data riwayat tiket yang sudah diproses
// ================================================================
struct Transaksi {
    int       nomorTransaksi;
    Pemesanan data;
    string    status;   // "Aktif" atau "Dibatalkan"
};

// ================================================================
//   [STRUCT] QUEUE BERBASIS ARRAY 
// ================================================================
const int MAX_QUEUE = 20;
const int MAX_STACK = 50;

struct Queue {
    Pemesanan data[MAX_QUEUE];
    int front, rear, size, counterAntrian;
};

struct Stack {
    Transaksi data[MAX_STACK];
    int top, counterTransaksi;
};

//   [STRUCT] [NODE] NODE QUEUE LINKED LIST
struct NodeQueue {                      // [STRUCT] [NODE] [QUEUE-LL]
    Pemesanan data;                     // data pemesanan
    NodeQueue* next;                    // [NODE] pointer ke node berikutnya
};

//   [STRUCT] [NODE] NODE STACK LINKED LIST
struct NodeStack {                      // [STRUCT] [NODE] [STACK-LL]
    Transaksi  data;                    // data transaksi
    NodeStack* next;                    // [NODE] pointer ke node di bawahnya
};

//   [STRUCT] QUEUE LINKED LIST
struct QueueLL {                        // [STRUCT] [QUEUE-LL]
    NodeQueue* front;                   // pointer ke node terdepan
    NodeQueue* rear;                    // pointer ke node terbelakang
    int        size;
    int        counterAntrian;
};

//   [STRUCT] STACK LINKED LIST
struct StackLL {                        // [STRUCT] [STACK-LL]
    NodeStack* top;                     // pointer ke node teratas
    int        size;
    int        counterTransaksi;
};

// ================================================================
//   KONSTANTA
// ================================================================
const int MAX_KERETA = 100;

// ================================================================
//   FUNGSI BANTU INPUT
// ================================================================
void bersihkanBuffer() { cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
void resetCin() { cin.clear(); bersihkanBuffer(); }

string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
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
        if (!getline(cin, baris)) { resetCin(); continue; }
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
        if (jam.empty())                         { cout << "   Jam tidak boleh kosong. Contoh: 07:30\n"; continue; }
        if (jam.length() != 5 || jam[2] != ':') { cout << "   Harus HH:MM, contoh: 07:30\n"; continue; }
        bool ok = true;
        for (int k = 0; k < 5; k++) { if (k==2) continue; if (!isdigit((unsigned char)jam[k])){ok=false;break;} }
        if (!ok) { cout << "   Jam dan menit harus angka.\n"; continue; }
        int hh = stoi(jam.substr(0,2)), mm = stoi(jam.substr(3,2));
        if (hh > 23 || mm > 59) { cout << "   Jam 00-23, menit 00-59.\n"; continue; }
        return jam;
    }
}

// ================================================================
//   FUNGSI TAMPILAN UMUM
// ================================================================
void cetakGaris(char c = '=', int p = 80) { for(int i=0;i<p;i++) cout<<c; cout<<"\n"; }
void cetakHeader() {
    cetakGaris('=');
    cout << "         SISTEM MANAJEMEN KEBERANGKATAN KERETA API\n";
    cetakGaris('=');
}

// ================================================================
//   TAMPIL SEMUA KERETA  
// ================================================================
void tampilSemuaKereta(Kereta* arr, int jumlah) { 
    if (jumlah == 0) { cout << "\n   Belum ada data kereta.\n"; return; }
    cetakGaris('-');
    cout << left << setw(5)<<"No" << setw(8)<<"No.KA" << setw(22)<<"Nama Kereta"
         << setw(14)<<"Asal" << setw(14)<<"Tujuan" << setw(8)<<"Brkt"
         << setw(8)<<"Tiba" << setw(14)<<"Harga (Rp)" << setw(7)<<"Kursi" << "\n";
    cetakGaris('-');
    for (int i = 0; i < jumlah; i++) {
        cout << left
             << setw(5)  << (i+1)
             << setw(8)  << (arr+i)->nomorKereta 
             << setw(22) << (arr+i)->namaKereta
             << setw(14) << (arr+i)->asal
             << setw(14) << (arr+i)->tujuan
             << setw(8)  << (arr+i)->jamBerangkat
             << setw(8)  << (arr+i)->jamTiba
             << setw(14) << (arr+i)->hargaTiket
             << setw(7)  << (arr+i)->kursiTersedia
             << "\n";
    }
    cetakGaris('-');
    cout << "   Total: " << jumlah << " kereta.\n";
}

// ================================================================
//   TAMBAH KERETA 
// ================================================================
void tambahKereta(Kereta* arr, int& jumlah) {
    if (jumlah >= MAX_KERETA) { cout << "\n   Database penuh!\n"; return; }
    cout << "\n"; cetakGaris('-');
    cout << "   TAMBAH DATA KERETA BARU\n"; cetakGaris('-');
    cout << "   (Isi setiap kolom, tekan Enter untuk melanjutkan)\n\n";
    int nomorBaru; bool duplikat;
    do {
        duplikat = false;
        nomorBaru = inputAngka("   Nomor Kereta      : ", 1, 99999);
        for (int i = 0; i < jumlah; i++) {
            if ((arr+i)->nomorKereta == nomorBaru) { 
                cout << "   Nomor " << nomorBaru << " sudah dipakai oleh \""
                     << (arr+i)->namaKereta << "\". Pakai nomor lain.\n";
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

// ================================================================
//   SWAP DATA KERETA =
// ================================================================
void swapKereta(Kereta* a, Kereta* b) {
    Kereta temp = *a; *a = *b; *b = temp;
}

// ================================================================
//   LINEAR SEARCH 
// ================================================================
void linearSearchRute(Kereta* arr, int jumlah) { 
    if (jumlah == 0) { cout << "\n   Belum ada data kereta.\n"; return; }
    cout << "\n"; cetakGaris('-');
    cout << "   CARI KERETA BERDASARKAN RUTE  --  Linear Search\n"; cetakGaris('-');
    string cariAsal = inputTeks("   Kota Asal   : ");
    string cariTujuan = inputTeks("   Kota Tujuan : ");
    string asalK = cariAsal, tujuanK = cariTujuan;
    transform(asalK.begin(),asalK.end(),asalK.begin(),::tolower);
    transform(tujuanK.begin(),tujuanK.end(),tujuanK.begin(),::tolower);
    cout << "\n   Proses pencarian:\n\n";
    cetakGaris('-', 72);
    cout << left << setw(6)<<"Cek" << setw(8)<<"No.KA" << setw(22)<<"Nama Kereta"
         << setw(14)<<"Asal" << setw(14)<<"Tujuan" << "Hasil\n";
    cetakGaris('-', 72);
    bool ada = false; int langkah = 0;
    for (int i = 0; i < jumlah; i++) {
        langkah++;
        string ad=(arr+i)->asal, td=(arr+i)->tujuan;
        transform(ad.begin(),ad.end(),ad.begin(),::tolower);
        transform(td.begin(),td.end(),td.begin(),::tolower);
        bool cocok = (ad==asalK && td==tujuanK);
        cout << left << setw(6)<<langkah << setw(8)<<(arr+i)->nomorKereta
             << setw(22)<<(arr+i)->namaKereta << setw(14)<<(arr+i)->asal
             << setw(14)<<(arr+i)->tujuan << (cocok?"[COCOK]":"-") << "\n";
        if (cocok) ada = true;
    }
    cetakGaris('-', 72);
    cout << "   Selesai. Total langkah: " << langkah << "\n";
    if (ada) {
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
            if (ad==asalK && td==tujuanK)
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

// ================================================================
//   JUMP SEARCH  
// ================================================================
void urutByNomor(Kereta* arr, int jumlah) {
    for (int i=0;i<jumlah-1;i++) {
        int minIdx=i;
        for (int j=i+1;j<jumlah;j++)
            if ((arr+j)->nomorKereta<(arr+minIdx)->nomorKereta) minIdx=j;
        if (minIdx!=i) swapKereta(arr+i, arr+minIdx); 
    }
}
void jumpSearchNomor(Kereta* arr, int jumlah) {
    if (jumlah==0) { cout<<"\n   Belum ada data kereta.\n"; return; }
    cout<<"\n"; cetakGaris('-');
    cout<<"   CARI KERETA BERDASARKAN NOMOR  --  Jump Search\n"; cetakGaris('-');
    int cariNomor = inputAngka("   Nomor kereta yang dicari : ", 1, 99999);
    cout<<"\n   Mengurutkan data berdasarkan nomor kereta...\n";
    urutByNomor(arr, jumlah);
    cout<<"   Data sudah terurut.\n\n";
    int langkah=(int)sqrt((double)jumlah), prev=0, iterasi=0;
    bool ketemu=false; int posisi=-1;
    cout<<"   Jumlah data: "<<jumlah<<"  |  Ukuran lompatan: "<<langkah<<"\n\n";
    cetakGaris('-',70);
    cout<<left<<setw(6)<<"Iter"<<setw(12)<<"Tahap"<<setw(10)<<"Index"<<setw(10)<<"No.KA"<<"Keterangan\n";
    cetakGaris('-',70);
    while ((arr+min(langkah,jumlah)-1)->nomorKereta < cariNomor) {
        iterasi++;
        int ujung=min(langkah,jumlah)-1;
        cout<<setw(6)<<iterasi<<setw(12)<<"Lompat"<<setw(10)<<ujung
            <<setw(10)<<(arr+ujung)->nomorKereta<<"Ujung blok ["<<ujung<<"] lebih kecil, lompat\n";
        prev=langkah; langkah+=(int)sqrt((double)jumlah);
        if (prev>=jumlah) { cetakGaris('-',70); cout<<"\n   Nomor "<<cariNomor<<" tidak ada.\n"; return; }
    }
    int batasAtas=min(langkah,jumlah)-1;
    cout<<"\n   Blok target: index ["<<prev<<" - "<<batasAtas<<"]. Cek satu per satu:\n\n";
    while ((arr+prev)->nomorKereta < cariNomor) { 
        iterasi++;
        cout<<setw(6)<<iterasi<<setw(12)<<"Cek"<<setw(10)<<prev
            <<setw(10)<<(arr+prev)->nomorKereta<<"Belum cocok, geser\n"; 
        prev++;
        if (prev>batasAtas) { cetakGaris('-',70); cout<<"\n   Nomor "<<cariNomor<<" tidak ada.\n"; return; }
    }
    iterasi++;
    if ((arr+prev)->nomorKereta==cariNomor) {
        cout<<setw(6)<<iterasi<<setw(12)<<"Cek"<<setw(10)<<prev
            <<setw(10)<<(arr+prev)->nomorKereta<<"[KETEMU]\n";
        ketemu=true; posisi=prev;
    }
    cetakGaris('-',70);
    cout<<"   Selesai. Total langkah: "<<iterasi<<"\n\n";
    if (ketemu) {
        cout<<"   Kereta ditemukan di index ke-"<<posisi<<".\n\n"; cetakGaris('-');
        cout<<"   Nomor Kereta   : "<<(arr+posisi)->nomorKereta<<"\n" 
            <<"   Nama Kereta    : "<<(arr+posisi)->namaKereta<<"\n"
            <<"   Rute           : "<<(arr+posisi)->asal<<" -> "<<(arr+posisi)->tujuan<<"\n"
            <<"   Jam Berangkat  : "<<(arr+posisi)->jamBerangkat<<"\n"
            <<"   Jam Tiba       : "<<(arr+posisi)->jamTiba<<"\n"
            <<"   Harga Tiket    : Rp "<<(arr+posisi)->hargaTiket<<"\n"
            <<"   Kursi Tersedia : "<<(arr+posisi)->kursiTersedia<<"\n";
        cetakGaris('-');
    } else { cout<<"   Nomor "<<cariNomor<<" tidak ditemukan.\n"; }
}

// ================================================================
//   MERGE SORT  
// ================================================================
void gabungkanTerurut(Kereta* arr, int kiri, int tengah, int kanan) {
    int ukL=tengah-kiri+1, ukR=kanan-tengah;
    Kereta* L=new Kereta[ukL]; Kereta* R=new Kereta[ukR];
    for (int i=0;i<ukL;i++) *(L+i)=*(arr+kiri+i);      
    for (int j=0;j<ukR;j++) *(R+j)=*(arr+tengah+1+j);   
    int i=0,j=0,k=kiri;
    while (i<ukL && j<ukR) {
        string nL=(L+i)->namaKereta, nR=(R+j)->namaKereta;
        transform(nL.begin(),nL.end(),nL.begin(),::tolower);
        transform(nR.begin(),nR.end(),nR.begin(),::tolower);
        if (nL<=nR) { *(arr+k)=*(L+i); i++; } else { *(arr+k)=*(R+j); j++; }
        k++;
    }
    while (i<ukL) { *(arr+k)=*(L+i); i++; k++; }
    while (j<ukR) { *(arr+k)=*(R+j); j++; k++; }
    delete[] L; delete[] R;
}
void mergeSortRekursif(Kereta* arr, int kiri, int kanan) {
    if (kiri<kanan) {
        int tengah=kiri+(kanan-kiri)/2;
        mergeSortRekursif(arr,kiri,tengah);
        mergeSortRekursif(arr,tengah+1,kanan);
        gabungkanTerurut(arr,kiri,tengah,kanan);
    }
}
void mergeSortNama(Kereta* arr, int jumlah) {
    if (jumlah==0) { cout<<"\n   Belum ada data kereta.\n"; return; }
    if (jumlah==1) { cout<<"\n   Hanya 1 data.\n"; tampilSemuaKereta(arr,jumlah); return; }
    cout<<"\n"; cetakGaris('-');
    cout<<"   URUTKAN NAMA KERETA A-Z  --  Merge Sort\n"; cetakGaris('-');
    cout<<"   Data SEBELUM diurutkan:\n"; tampilSemuaKereta(arr,jumlah);
    mergeSortRekursif(arr,0,jumlah-1);
    cout<<"\n   Pengurutan selesai.\n\n   Data SESUDAH diurutkan:\n";
    tampilSemuaKereta(arr,jumlah);
}

// ================================================================
//   SELECTION SORT
// ================================================================
void selectionSortHarga(Kereta* arr, int jumlah) {
    if (jumlah==0) { cout<<"\n   Belum ada data kereta.\n"; return; }
    if (jumlah==1) { cout<<"\n   Hanya 1 data.\n"; tampilSemuaKereta(arr,jumlah); return; }
    cout<<"\n"; cetakGaris('-');
    cout<<"   URUTKAN HARGA TIKET TERMURAH  --  Selection Sort\n"; cetakGaris('-');
    cout<<"   Data SEBELUM diurutkan:\n"; tampilSemuaKereta(arr,jumlah);
    cout<<"\n   Proses pengurutan:\n"; cetakGaris('-',75);
    cout<<left<<setw(9)<<"Putaran"<<setw(24)<<"Posisi Saat Ini"<<setw(12)<<"Harga"
        <<setw(24)<<"Ditukar Dengan"<<"Harga\n"; cetakGaris('-',75);
    for (int i=0;i<jumlah-1;i++) {
        int posMin=i;
        for (int j=i+1;j<jumlah;j++)
            if ((arr+j)->hargaTiket<(arr+posMin)->hargaTiket) posMin=j;
        if (posMin!=i) {
            cout<<setw(9)<<(i+1)<<setw(24)<<(arr+i)->namaKereta
                <<setw(12)<<(arr+i)->hargaTiket
                <<setw(24)<<(arr+posMin)->namaKereta<<(arr+posMin)->hargaTiket<<"\n";
            swapKereta(arr+i,arr+posMin);
        } else {
            cout<<setw(9)<<(i+1)<<setw(24)<<(arr+i)->namaKereta
                <<setw(12)<<(arr+i)->hargaTiket<<"(sudah di posisi yang benar)\n";
        }
    }
    cetakGaris('-',75);
    cout<<"\n   Pengurutan selesai.\n\n   Data SESUDAH diurutkan:\n";
    tampilSemuaKereta(arr,jumlah);
}

// ================================================================
//   DATA AWAL KERETA
// ================================================================
void isiDataAwal(Kereta* arr, int& jumlah) {
    Kereta contoh[] = {
        {101,"Argo Bromo Anggrek","Jakarta",   "Surabaya",  "08:00","17:30",580000,50},
        {205,"Gajayana",          "Malang",    "Jakarta",   "17:00","05:00",450000,64},
        {302,"Bima",              "Surabaya",  "Bandung",   "14:00","23:00",320000,80},
        {418,"Taksaka",           "Yogyakarta","Jakarta",   "09:00","15:30",275000,72},
        {155,"Lodaya",            "Bandung",   "Solo",      "07:15","13:00",210000,56},
        {267,"Sancaka",           "Surabaya",  "Yogyakarta","06:30","11:00",195000,96},
        {389,"Brantas",           "Blitar",    "Jakarta",   "15:00","04:30",250000,88},
        {512,"Malioboro",         "Yogyakarta","Surabaya",  "10:00","14:30",185000,60},
    };
    int n=(int)(sizeof(contoh)/sizeof(contoh[0]));
    for (int i=0;i<n&&jumlah<MAX_KERETA;i++) { *(arr+jumlah)=contoh[i]; jumlah++; }
}

// ================================================================
//   QUEUE & STACK BERBASIS ARRAY 
// ================================================================
void inisialisasiQueue(Queue* q) { q->front=0; q->rear=-1; q->size=0; q->counterAntrian=0; }
void inisialisasiStack(Stack* s) { s->top=-1; s->counterTransaksi=0; }
bool queueKosong(Queue* q) { return q->size==0; }
bool queuePenuh(Queue* q)  { return q->size==MAX_QUEUE; }
bool stackKosong(Stack* s) { return s->top==-1; }
bool stackPenuh(Stack* s)  { return s->top==MAX_STACK-1; }

void enqueue(Queue* q, Kereta* dataKereta, int jumlahKereta) {
    if (queuePenuh(q)) { cout<<"\n   Antrian penuh! Maksimal "<<MAX_QUEUE<<" orang.\n"; return; }
    if (jumlahKereta==0) { cout<<"\n   Belum ada data kereta.\n"; return; }
    cout<<"\n"; cetakGaris('-'); cout<<"   PESAN TIKET (Antrian Array)\n"; cetakGaris('-');
    cout<<"   Pilih kereta:\n\n"; tampilSemuaKereta(dataKereta,jumlahKereta);
    int nomorPilihan=inputAngka("\n   Masukkan No.KA : ",1,99999);
    int idxKereta=-1;
    for (int i=0;i<jumlahKereta;i++)
        if ((dataKereta+i)->nomorKereta==nomorPilihan) { idxKereta=i; break; }
    if (idxKereta==-1) { cout<<"   Nomor tidak ditemukan.\n"; return; }
    if ((dataKereta+idxKereta)->kursiTersedia==0) { cout<<"   Kursi habis.\n"; return; }
    string nama=inputTeks("   Nama Penumpang : ");
    q->rear=(q->rear+1)%MAX_QUEUE;
    q->counterAntrian++;
    Pemesanan* p=&q->data[q->rear];
    p->nomorAntrian=q->counterAntrian; p->namaPenumpang=nama;
    p->nomorKereta=(dataKereta+idxKereta)->nomorKereta;
    p->namaKereta=(dataKereta+idxKereta)->namaKereta;
    p->asal=(dataKereta+idxKereta)->asal; p->tujuan=(dataKereta+idxKereta)->tujuan;
    p->jamBerangkat=(dataKereta+idxKereta)->jamBerangkat;
    p->jamTiba=(dataKereta+idxKereta)->jamTiba;
    p->hargaTiket=(dataKereta+idxKereta)->hargaTiket;
    q->size++; (dataKereta+idxKereta)->kursiTersedia--;
    cout<<"\n   "<<nama<<" masuk antrian nomor "<<q->counterAntrian<<".\n"
        <<"   Kereta: "<<p->namaKereta<<" ("<<p->asal<<" -> "<<p->tujuan<<")\n";
}
void dequeue(Queue* q, Stack* s) {
    if (queueKosong(q)) { cout<<"\n   Antrian kosong! Tidak ada yang bisa diproses.\n"; return; }
    Pemesanan diproses=q->data[q->front];
    q->front=(q->front+1)%MAX_QUEUE; q->size--;
    cout<<"\n"; cetakGaris('-'); cout<<"   PROSES TIKET (Antrian Array)\n"; cetakGaris('-');
    cout<<"   No. Antrian  : "<<diproses.nomorAntrian<<"\n"
        <<"   Penumpang    : "<<diproses.namaPenumpang<<"\n"
        <<"   Kereta       : "<<diproses.namaKereta<<" (No."<<diproses.nomorKereta<<")\n"
        <<"   Rute         : "<<diproses.asal<<" -> "<<diproses.tujuan<<"\n"
        <<"   Harga Tiket  : Rp "<<diproses.hargaTiket<<"\n";
    cetakGaris('-');
    cout<<"   Tiket diproses. Sisa antrian: "<<q->size<<" orang.\n";
    if (stackPenuh(s)) { cout<<"   Riwayat penuh, transaksi tidak dicatat.\n"; return; }
    s->top++; s->counterTransaksi++;
    s->data[s->top]={s->counterTransaksi, diproses, "Aktif"};
    cout<<"   Tercatat sebagai Transaksi #"<<s->counterTransaksi<<".\n";
}
void pop(Stack* s) {
    if (stackKosong(s)) { cout<<"\n   Riwayat kosong! Tidak ada yang bisa dibatalkan.\n"; return; }
    s->data[s->top].status="Dibatalkan";
    cout<<"\n"; cetakGaris('-'); cout<<"   BATALKAN TRANSAKSI TERAKHIR (Stack Array)\n"; cetakGaris('-');
    cout<<"   Transaksi #"<<s->data[s->top].nomorTransaksi<<" -- "
        <<s->data[s->top].data.namaPenumpang<<" -- "
        <<s->data[s->top].data.namaKereta<<"\n";
    s->top--;
    cout<<"   Transaksi dibatalkan. Sisa riwayat: "<<(s->top+1)<<".\n";
}
void peek(Queue* q, Stack* s) {
    cout<<"\n"; cetakGaris('-'); cout<<"   PEEK (Array)\n"; cetakGaris('-');
    cout<<"\n   Antrian terdepan:\n";
    if (queueKosong(q)) cout<<"   Antrian kosong.\n";
    else {
        Pemesanan* d=&q->data[q->front];
        cout<<"   No."<<d->nomorAntrian<<" | "<<d->namaPenumpang
            <<" | "<<d->namaKereta<<" | "<<d->asal<<" -> "<<d->tujuan
            <<" | Rp "<<d->hargaTiket<<"\n";
    }
    cout<<"\n   Riwayat teratas:\n";
    if (stackKosong(s)) cout<<"   Riwayat kosong.\n";
    else {
        Transaksi* t=&s->data[s->top];
        cout<<"   Trx#"<<t->nomorTransaksi<<" | "<<t->data.namaPenumpang
            <<" | "<<t->data.namaKereta<<" | Rp "<<t->data.hargaTiket
            <<" | "<<t->status<<"\n";
    }
}
void tampilAntrian(Queue* q) {
    cout<<"\n"; cetakGaris('-'); cout<<"   DAFTAR ANTRIAN (Berbasis Array)\n"; cetakGaris('-');
    if (queueKosong(q)) { cout<<"   Antrian kosong.\n"; return; }
    cout<<"   Kapasitas: "<<q->size<<" / "<<MAX_QUEUE<<" orang\n\n";
    cout<<left<<setw(6)<<"Pos"<<setw(8)<<"No.Ant"<<setw(20)<<"Penumpang"
        <<setw(8)<<"No.KA"<<setw(22)<<"Kereta"<<setw(16)<<"Rute"<<"Harga\n";
    cetakGaris('-',80);
    for (int i=0;i<q->size;i++) {
        int idx=(q->front+i)%MAX_QUEUE;
        Pemesanan* p=q->data+idx; // [PTR-ARITMATIK]
        cout<<left<<setw(6)<<(i+1)<<setw(8)<<p->nomorAntrian<<setw(20)<<p->namaPenumpang
            <<setw(8)<<p->nomorKereta<<setw(22)<<p->namaKereta
            <<setw(16)<<(p->asal+"->"+p->tujuan)<<"Rp "<<p->hargaTiket<<"\n";
    }
    cetakGaris('-',80);
}
void tampilRiwayat(Stack* s) {
    cout<<"\n"; cetakGaris('-'); cout<<"   RIWAYAT TRANSAKSI (Berbasis Array)\n"; cetakGaris('-');
    if (stackKosong(s)) { cout<<"   Riwayat kosong.\n"; return; }
    cout<<"   Total: "<<(s->top+1)<<" transaksi\n\n";
    cout<<left<<setw(8)<<"No.Trx"<<setw(20)<<"Penumpang"<<setw(8)<<"No.KA"
        <<setw(22)<<"Kereta"<<setw(18)<<"Rute"<<setw(12)<<"Harga"<<"Status\n";
    cetakGaris('-',90);
    for (int i=s->top;i>=0;i--) {
        Transaksi* t=s->data+i; // [PTR-ARITMATIK]
        cout<<left<<setw(8)<<t->nomorTransaksi<<setw(20)<<t->data.namaPenumpang
            <<setw(8)<<t->data.nomorKereta<<setw(22)<<t->data.namaKereta
            <<setw(18)<<(t->data.asal+"->"+t->data.tujuan)
            <<setw(12)<<("Rp "+to_string(t->data.hargaTiket))<<t->status<<"\n";
    }
    cetakGaris('-',90);
}

//   ============================================================
//   FITUR BARU: QUEUE & STACK BERBASIS SINGLE LINKED LIST
//   ============================================================

// ================================================================
//   INISIALISASI QUEUE LL DAN STACK LL
// ================================================================
void inisialisasiQueueLL(QueueLL* q) {
    q->front = nullptr;   // belum ada node
    q->rear  = nullptr;
    q->size  = 0;
    q->counterAntrian = 0;
}

void inisialisasiStackLL(StackLL* s) {
    s->top  = nullptr;    // belum ada node
    s->size = 0;
    s->counterTransaksi = 0;
}

//   [QUEUE-LL] ENQUEUE -- Tambah node baru di belakang antrian
void enqueueLL(QueueLL* q, Kereta* dataKereta, int jumlahKereta) {
    if (jumlahKereta == 0) { cout << "\n   Belum ada data kereta.\n"; return; }

    cout << "\n"; cetakGaris('-');
    cout << "   PESAN TIKET  --  Enqueue (Single Linked List)\n"; cetakGaris('-');
    cout << "   Pilih kereta:\n\n";
    tampilSemuaKereta(dataKereta, jumlahKereta);

    int nomorPilihan = inputAngka("\n   Masukkan No.KA yang dipilih : ", 1, 99999);
    int idxKereta = -1;
    for (int i = 0; i < jumlahKereta; i++)
        if ((dataKereta + i)->nomorKereta == nomorPilihan) { idxKereta = i; break; } 

    if (idxKereta == -1) { cout << "   Nomor kereta tidak ditemukan.\n"; return; }
    if ((dataKereta + idxKereta)->kursiTersedia == 0) { cout << "   Kursi sudah habis.\n"; return; }

    string nama = inputTeks("   Nama Penumpang          : ");

    // Buat node baru di heap
    NodeQueue* nodeBaru = new NodeQueue(); 
    nodeBaru->next = nullptr;             

    // Isi data ke dalam node baru
    q->counterAntrian++;
    nodeBaru->data.nomorAntrian  = q->counterAntrian;
    nodeBaru->data.namaPenumpang = nama;
    nodeBaru->data.nomorKereta   = (dataKereta + idxKereta)->nomorKereta;
    nodeBaru->data.namaKereta    = (dataKereta + idxKereta)->namaKereta;
    nodeBaru->data.asal          = (dataKereta + idxKereta)->asal;
    nodeBaru->data.tujuan        = (dataKereta + idxKereta)->tujuan;
    nodeBaru->data.jamBerangkat  = (dataKereta + idxKereta)->jamBerangkat;
    nodeBaru->data.jamTiba       = (dataKereta + idxKereta)->jamTiba;
    nodeBaru->data.hargaTiket    = (dataKereta + idxKereta)->hargaTiket;

    // Sambungkan ke belakang antrian
    if (q->rear == nullptr) {
        // Antrian kosong: node baru jadi satu-satunya elemen
        q->front = nodeBaru;
        q->rear  = nodeBaru;
    } else {
        // Antrian tidak kosong: sambung ke belakang
        q->rear->next = nodeBaru;
        q->rear       = nodeBaru; 
    }
    q->size++;
    (dataKereta + idxKereta)->kursiTersedia--;

    cout << "\n   Berhasil! " << nama
         << " masuk antrian nomor " << q->counterAntrian << ".\n"
         << "   Kereta  : " << nodeBaru->data.namaKereta
         << "  (" << nodeBaru->data.asal << " -> " << nodeBaru->data.tujuan << ")\n"
         << "   Posisi dalam antrian: " << q->size << "\n";
}

//   [QUEUE-LL] DEQUEUE -- Ambil & hapus node dari depan antrian

void dequeueLL(QueueLL* q, StackLL* s) {
    if (q->front == nullptr) {
        cout << "\n   [UNDERFLOW] Antrian kosong! Tidak ada penumpang yang bisa diproses.\n";
        return;
    }

    // Simpan pointer ke node terdepan
    NodeQueue* nodeLama = q->front; // tunjuk ke node yang akan dihapus
    Pemesanan  diproses = nodeLama->data; // salin datanya dulu sebelum dihapus

    // Geser front ke node berikutnya
    q->front = nodeLama->next; // front sekarang adalah node kedua
    if (q->front == nullptr)   // jika antrian jadi kosong
        q->rear = nullptr;     // rear juga harus nullptr

    delete nodeLama; // bebaskan memori node yang sudah tidak dipakai
    q->size--;

    cout << "\n"; cetakGaris('-');
    cout << "   PROSES TIKET  --  Dequeue (Single Linked List)\n"; cetakGaris('-');
    cout << "   No. Antrian  : " << diproses.nomorAntrian  << "\n"
         << "   Penumpang    : " << diproses.namaPenumpang  << "\n"
         << "   Kereta       : " << diproses.namaKereta     << "  (No." << diproses.nomorKereta << ")\n"
         << "   Rute         : " << diproses.asal << " -> " << diproses.tujuan << "\n"
         << "   Jam Berangkat: " << diproses.jamBerangkat   << "\n"
         << "   Jam Tiba     : " << diproses.jamTiba        << "\n"
         << "   Harga Tiket  : Rp " << diproses.hargaTiket  << "\n";
    cetakGaris('-');
    cout << "   Tiket berhasil diproses. Sisa antrian: " << q->size << " orang.\n";

    // Otomatis push ke Stack LL
    NodeStack* nodeRiwayat = new NodeStack();
    s->counterTransaksi++;
    nodeRiwayat->data.nomorTransaksi = s->counterTransaksi;
    nodeRiwayat->data.data           = diproses;
    nodeRiwayat->data.status         = "Aktif";
    nodeRiwayat->next = s->top;
    s->top            = nodeRiwayat;
    s->size++;

    cout << "   Tercatat di riwayat sebagai Transaksi #" << s->counterTransaksi << ".\n";
}

//  PUSH -- Tambah node baru di atas stack
void pushLL(StackLL* s, Transaksi trx) {
    NodeStack* nodeBaru = new NodeStack();
    nodeBaru->data = trx;
    nodeBaru->next = s->top; 
    s->top         = nodeBaru;
    s->size++;
}

//  POP -- Hapus node dari atas stack
void popLL(StackLL* s) {
    if (s->top == nullptr) { // [UNDERFLOW]
        cout << "\n   [UNDERFLOW] Riwayat kosong! Tidak ada transaksi yang bisa dibatalkan.\n";
        return;
    }

    NodeStack* nodeLama = s->top; // tunjuk ke node yang akan dihapus
    nodeLama->data.status = "Dibatalkan";

    cout << "\n"; cetakGaris('-');
    cout << "   BATALKAN TRANSAKSI TERAKHIR  --  Pop (Single Linked List)\n"; cetakGaris('-');
    cout << "   Transaksi yang dibatalkan:\n"
         << "   No. Transaksi : " << nodeLama->data.nomorTransaksi      << "\n"
         << "   Penumpang     : " << nodeLama->data.data.namaPenumpang   << "\n"
         << "   Kereta        : " << nodeLama->data.data.namaKereta      << "\n"
         << "   Rute          : " << nodeLama->data.data.asal
                                  << " -> " << nodeLama->data.data.tujuan << "\n"
         << "   Harga Tiket   : Rp " << nodeLama->data.data.hargaTiket   << "\n"
         << "   Status        : " << nodeLama->data.status               << "\n";
    cetakGaris('-');

    s->top = nodeLama->next;
    delete nodeLama;         
    s->size--;

    cout << "   Transaksi berhasil dibatalkan.\n"
         << "   Sisa riwayat: " << s->size << " transaksi.\n";
}

//  PEEK -- Lihat tanpa menghapus
void peekLL(QueueLL* q, StackLL* s) {
    cout << "\n"; cetakGaris('-');
    cout << "   PEEK  --  Lihat Tanpa Menghapus (Single Linked List)\n"; cetakGaris('-');

    // Peek Queue: tampilkan node front
    cout << "\n   [ Antrian -- Penumpang Paling Depan ]\n";
    if (q->front == nullptr) { // [UNDERFLOW]
        cout << "   Antrian sedang kosong.\n";
    } else {
        // Langsung akses q->front->data tanpa mengubah apapun
        Pemesanan* depan = &q->front->data; // pointer ke data di node front
        cout << "   No. Antrian  : " << depan->nomorAntrian  << "\n"
             << "   Penumpang    : " << depan->namaPenumpang  << "\n"
             << "   Kereta       : " << depan->namaKereta     << "\n"
             << "   Rute         : " << depan->asal << " -> " << depan->tujuan << "\n"
             << "   Harga Tiket  : Rp " << depan->hargaTiket  << "\n";
    }

    cout << "\n"; cetakGaris('-', 40);

    // Peek Stack: tampilkan node top
    cout << "\n   [ Riwayat -- Transaksi Paling Terakhir ]\n";
    if (s->top == nullptr) { // [UNDERFLOW]
        cout << "   Riwayat masih kosong.\n";
    } else {
        Transaksi* atas = &s->top->data;
        cout << "   No. Transaksi : " << atas->nomorTransaksi      << "\n"
             << "   Penumpang     : " << atas->data.namaPenumpang   << "\n"
             << "   Kereta        : " << atas->data.namaKereta      << "\n"
             << "   Rute          : " << atas->data.asal
                                      << " -> " << atas->data.tujuan << "\n"
             << "   Harga Tiket   : Rp " << atas->data.hargaTiket   << "\n"
             << "   Status        : " << atas->status                << "\n";
    }
}


//  TAMPIL SEMUA ANTRIAN LINKED LIST
void tampilAntrianLL(QueueLL* q) {
    cout << "\n"; cetakGaris('-');
    cout << "   DAFTAR ANTRIAN  --  Single Linked List\n"; cetakGaris('-');

    if (q->front == nullptr) {
        cout << "   Antrian sedang kosong.\n"; return;
    }

    cout << "   Total: " << q->size << " orang dalam antrian\n\n";
    cout << left << setw(6)<<"Posisi" << setw(8)<<"No.Ant" << setw(20)<<"Penumpang"
         << setw(8)<<"No.KA" << setw(22)<<"Kereta" << setw(16)<<"Rute" << "Harga\n";
    cetakGaris('-', 82);

    // Telusuri dari front ke rear menggunakan pointer next
    NodeQueue* current = q->front;
    int posisi = 1;
    while (current != nullptr) {
        Pemesanan* p = &current->data;
        cout << left
             << setw(6)  << posisi
             << setw(8)  << p->nomorAntrian
             << setw(20) << p->namaPenumpang
             << setw(8)  << p->nomorKereta
             << setw(22) << p->namaKereta
             << setw(16) << (p->asal + "->" + p->tujuan)
             << "Rp " << p->hargaTiket << "\n";
        current = current->next;
        posisi++;
    }
    cetakGaris('-', 82);
    cout << "   Posisi 1 = paling depan (akan diproses duluan).\n";
}


//  TAMPIL SEMUA RIWAYAT LINKED LIST
void tampilRiwayatLL(StackLL* s) {
    cout << "\n"; cetakGaris('-');
    cout << "   RIWAYAT TRANSAKSI  --  Single Linked List\n"; cetakGaris('-');

    if (s->top == nullptr) { // [UNDERFLOW]
        cout << "   Belum ada riwayat transaksi.\n"; return;
    }

    cout << "   Total: " << s->size << " transaksi\n\n";
    cout << left << setw(8)<<"No.Trx" << setw(20)<<"Penumpang" << setw(8)<<"No.KA"
         << setw(22)<<"Kereta" << setw(18)<<"Rute" << setw(12)<<"Harga" << "Status\n";
    cetakGaris('-', 90);

    // Telusuri dari top ke bawah menggunakan pointer next
    NodeStack* current = s->top;
    while (current != nullptr) {
        Transaksi* t = &current->data;
        cout << left
             << setw(8)  << t->nomorTransaksi
             << setw(20) << t->data.namaPenumpang
             << setw(8)  << t->data.nomorKereta
             << setw(22) << t->data.namaKereta
             << setw(18) << (t->data.asal + "->" + t->data.tujuan)
             << setw(12) << ("Rp " + to_string(t->data.hargaTiket))
             << t->status << "\n";
        current = current->next; 
    }
    cetakGaris('-', 90);
    cout << "   Urutan: paling atas = transaksi terbaru.\n";
}

// ================================================================
//   BEBASKAN MEMORI LINKED LIST (penting dipanggil saat keluar)
// ================================================================
void hapusSemuaQueueLL(QueueLL* q) {
    NodeQueue* current = q->front;
    while (current != nullptr) {
        NodeQueue* temp = current;
        current = current->next;
        delete temp;
    }
    q->front = nullptr; q->rear = nullptr; q->size = 0;
}
void hapusSemuaStackLL(StackLL* s) {
    NodeStack* current = s->top;
    while (current != nullptr) {
        NodeStack* temp = current;
        current = current->next;
        delete temp;
    }
    s->top = nullptr; s->size = 0;
}

// ================================================================
//   MENU
// ================================================================
void tampilMenuAntrianArray() {
    cout << "\n"; cetakGaris('=');
    cout << "   MENU ANTRIAN & RIWAYAT  --  Berbasis Array\n"; cetakGaris('=');
    cout << "   1. Pesan tiket         (Enqueue)\n";
    cout << "   2. Proses tiket depan  (Dequeue)\n";
    cout << "   3. Lihat semua antrian\n";
    cout << "   4. Batalkan transaksi  (Pop)\n";
    cout << "   5. Lihat semua riwayat\n";
    cout << "   6. Peek\n";
    cout << "   0. Kembali\n"; cetakGaris('=');
}

void tampilMenuAntrianLL() {
    cout << "\n"; cetakGaris('=');
    cout << "   MENU ANTRIAN & RIWAYAT  --  Single Linked List\n"; cetakGaris('=');
    cout << "   1. Pesan tiket         (Enqueue LL)\n";
    cout << "   2. Proses tiket depan  (Dequeue LL)\n";
    cout << "   3. Lihat semua antrian\n";
    cout << "   4. Batalkan transaksi  (Pop LL)\n";
    cout << "   5. Lihat semua riwayat\n";
    cout << "   6. Peek\n";
    cout << "   0. Kembali\n"; cetakGaris('=');
}

void tampilMenuUtama() {
    cout << "\n"; cetakGaris('=');
    cout << "   MENU UTAMA\n"; cetakGaris('=');
    cout << "   -- DATA KERETA --\n";
    cout << "   1. Tampilkan semua jadwal kereta\n";
    cout << "   2. Tambah data kereta baru\n";
    cout << "   3. Cari kereta berdasarkan rute  (Linear Search)\n";
    cout << "   4. Cari kereta berdasarkan nomor (Jump Search)\n";
    cout << "   5. Urutkan nama kereta A-Z        (Merge Sort)\n";
    cout << "   6. Urutkan harga tiket termurah   (Selection Sort)\n";
    cout << "   -- TIKET & TRANSAKSI --\n";
    cout << "   7. Antrian & Riwayat (Berbasis Array)\n";
    cout << "   8. Antrian & Riwayat (Single Linked List)\n";
    cout << "   0. Keluar\n"; cetakGaris('=');
}

// ================================================================
//   MAIN
// ================================================================
int main() {
    // Array of struct kereta
    Kereta dataKereta[MAX_KERETA];
    int jumlahKereta = 0;
    isiDataAwal(dataKereta, jumlahKereta);

    // Queue & Stack berbasis array
    Queue antrianArr;  Stack riwayatArr;
    inisialisasiQueue(&antrianArr);
    inisialisasiStack(&riwayatArr);

    // Queue & Stack berbasis Single Linked List
    QueueLL antrianLL;  StackLL riwayatLL;
    inisialisasiQueueLL(&antrianLL);
    inisialisasiStackLL(&riwayatLL);

    cetakHeader();
    cout << "   Selamat datang!\n";
    cout << "   " << jumlahKereta << " data kereta contoh sudah dimuat.\n";

    int pilihan;
    do {
        tampilMenuUtama();
        pilihan = inputAngka("   Pilih menu (0-8): ", 0, 8);

        switch (pilihan) {
            case 1:
                cout<<"\n"; cetakGaris('-'); cout<<"   SEMUA JADWAL KERETA\n"; cetakGaris('-');
                tampilSemuaKereta(&dataKereta[0], jumlahKereta); break;
            case 2: tambahKereta(&dataKereta[0], jumlahKereta);  break;
            case 3: linearSearchRute(&dataKereta[0], jumlahKereta); break;
            case 4: jumpSearchNomor(&dataKereta[0], jumlahKereta);  break;
            case 5: mergeSortNama(&dataKereta[0], jumlahKereta);    break;
            case 6: selectionSortHarga(&dataKereta[0], jumlahKereta); break;

            case 7: {
                int sub;
                do {
                    tampilMenuAntrianArray();
                    sub = inputAngka("   Pilih menu (0-6): ", 0, 6);
                    switch (sub) {
                        case 1: enqueue(&antrianArr,&dataKereta[0],jumlahKereta); break;
                        case 2: dequeue(&antrianArr,&riwayatArr); break;
                        case 3: tampilAntrian(&antrianArr); break;
                        case 4: pop(&riwayatArr); break;
                        case 5: tampilRiwayat(&riwayatArr); break;
                        case 6: peek(&antrianArr,&riwayatArr); break;
                        case 0: cout<<"\n   Kembali.\n"; break;
                        default: cout<<"\n   Pilihan tidak ada.\n";
                    }
                    if (sub!=0) { cout<<"\n   Tekan Enter..."; string d; getline(cin,d); }
                } while (sub != 0);
                break;
            }

            case 8: {
                int sub;
                do {
                    tampilMenuAntrianLL();
                    sub = inputAngka("   Pilih menu (0-6): ", 0, 6);
                    switch (sub) {
                        case 1: enqueueLL(&antrianLL,&dataKereta[0],jumlahKereta); break;
                        case 2: dequeueLL(&antrianLL,&riwayatLL); break;
                        case 3: tampilAntrianLL(&antrianLL); break;
                        case 4: popLL(&riwayatLL); break;
                        case 5: tampilRiwayatLL(&riwayatLL); break;
                        case 6: peekLL(&antrianLL,&riwayatLL); break;
                        case 0: cout<<"\n   Kembali.\n"; break;
                        default: cout<<"\n   Pilihan tidak ada.\n";
                    }
                    if (sub!=0) { cout<<"\n   Tekan Enter..."; string d; getline(cin,d); }
                } while (sub != 0);
                break;
            }

            case 0:
                // Bebaskan semua memori linked list sebelum keluar
                hapusSemuaQueueLL(&antrianLL);
                hapusSemuaStackLL(&riwayatLL);
                cout << "\n   Sampai jumpa!\n\n";
                break;
            default:
                cout << "\n   Pilihan tidak ada.\n";
        }

        if (pilihan != 0) {
            cout << "\n   Tekan Enter untuk kembali ke menu...";
            string dummy; getline(cin, dummy);
        }
    } while (pilihan != 0);

    return 0;
}