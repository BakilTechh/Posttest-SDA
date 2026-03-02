#include <iostream>
using namespace std;

int cariMinIndex(int data[], int n) {
    int posisi = 0; // anggap elemen pertama sebagai minimum
    for (int i = 1; i < n; i++) {
        if (data[i] < data[posisi]) {
            posisi = i; // update posisi jika ditemukan nilai lebih kecil
        }
    }
    return posisi;
}

int main() {
    int arr[8] = {1, 1, 2, 3, 5, 8, 13, 21};
    int idx = cariMinIndex(arr, 8);

    cout << "Elemen minimum = " << arr[idx] 
         << " berada pada indeks ke-" << idx << endl;

    return 0;
}

/*
Analisis Kompleksitas Waktu:
    ---------------------------------------------------------------
    | Baris | Pseudocode              | Cost        | Tmin | Tmax |
    ---------------------------------------------------------------
    |   1   | min ← arr[0]            | 1           |  1   |  1   |
    |   2   | for i ← 1 to n-1        | (n-1)       | n-1  | n-1  |
    |   3   | if arr[i] < min         | (n-1)       | n-1  | n-1  |
    |   4   | min ← arr[i]            | 0..(n-1)    |  0   | n-1  |
    |   5   | return min              | 1           |  1   |  1   |
    ---------------------------------------------------------------

[------------------------------------------------------------------]
Penjelasan :
Baris 1: 1 kali → cost = 1
Baris 2: (n-1) kali → cost = n-1
Baris 3: (n-1) kali → cost = n-1
Baris 4: Best case = 0 kali, Worst case = n-1 kali
Baris 5: 1 kali → cost = 1

Tmin(n) :
= 1 + (n-1) + (n-1) + 0 + 1
= 2n - 2 + 2
= 2n
→ O(n)

Tmax(n) : 
1 + (n-1) + (n-1) + (n-1) + 1
= 3n - 3 + 2
= 3n - 1
→ O(n)

Kesimpulannya:
- Tmin(n) = 2n → O(n)
- Tmax(n) = 3n - 1 → O(n)
- Kompleksitas waktu algoritma pencarian minimum linear, baik best case maupun worst case.
*/