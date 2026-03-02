#include <iostream>
using namespace std;


/*
seorang dosen ingin memperbaiki nilai mahasiswanya karena ada penambahan nilai dari dosennya,
lalu dosennya membuat program untuk menukar nilai mahasiswanya menggunakan pointer, dengan ketentuan:

x = nilai mhs sebelumnya
y = nilai mhs setelah penambahan dari dosen
*/

// Fungsi untuk menukar dua variabel menggunakan pointer
void tukarNilai(int *a, int *b) {
    int sementara = *a;   // simpan nilai a
    *a = *b;              // isi a dengan nilai b
    *b = sementara;       // isi b dengan nilai awal a
}

int main() {
    int x = 50;   
    int y = 100;   

    cout << "Sebelum ditukar:" << endl;
    cout << "x = " << x << ", y = " << y << endl;

    // Panggil fungsi tukar
    tukarNilai(&x, &y);

    cout << "\nSesudah ditukar:" << endl;
    cout << "x = " << x << ", y = " << y << endl;
    cout << "\nNilai Mahasiswa setelah penambahan dari dosen, yaitu: " << x << endl;

    return 0;
}