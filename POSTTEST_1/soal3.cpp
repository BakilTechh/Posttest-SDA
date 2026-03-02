#include <iostream>
using namespace std;


void reverseArray(int* arr, int n) {
    int* start = arr;          // pointer ke elemen pertama
    int* end = arr + n - 1;    // pointer ke elemen terakhir

    while (start < end) {
        int temp = *start;     // simpan nilai sementara
        *start = *end;         // tukar nilai
        *end = temp;

        start++;               // geser pointer ke depan
        end--;                 // geser pointer ke belakang
    }
}

int main() {

    int bilanganPrima[7] = {2, 3, 5, 7, 11, 13, 17};
    int n = 7;

    cout << "=== Array sebelum dibalik ===\n";
    for (int i = 0; i < n; i++) {
        cout << "Index " << i 
             << " -> Nilai: " << *(bilanganPrima + i)
             << " | Alamat: " << (bilanganPrima + i) << endl;
    }

    // Kita Balikkan arraynya  
    reverseArray(bilanganPrima, n);

    cout << "\n=== Array sesudah dibalik ===\n";
    for (int i = 0; i < n; i++) {
        cout << "Index " << i 
             << " -> Nilai: " << *(bilanganPrima + i)
             << " | Alamat: " << (bilanganPrima + i) << endl;
    }

    return 0;
}