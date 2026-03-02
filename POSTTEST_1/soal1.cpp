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
    Analisis Kompleksitas Waktu Algoritma cariMinIndex:
    ---------------------------------------------------------------
    | Baris | Pseudocode             | Label | Cost | Tmin | Tmax |
    ---------------------------------------------------------------
    |   1   | procedure FindMin      |  C1   |  1   |  1   |  1   |
    |   2   | min ← A[0]             |  C2   |  1   |  1   |  1   |
    |   3   | for i ← 1 to n − 1     |  C3   |  n   |  n   |  n   |
    |   4   | if A[i] < min          |  C4   | n-1  | n-1  | n-1  |
    |   5   | min ← A[i]             |  C5   | 0..n-1 | 0  | n-1 |
    |   6   | end if                 |  C6   | n-1  | n-1  | n-1  |
    |   7   | end for                |  C7   |  n   |  n   |  n   |
    |   8   | return min             |  C8   |  1   |  1   |  1   |
    |   9   | end procedure          |  C9   |  1   |  1   |  1   |
    ---------------------------------------------------------------

    [-----------------------------------------------------------------]
    
    Perhitungan:
    Tmin(n) = 1 + 1 + n + (n-1) + 0 + (n-1) + n + 1 + 1
            = 3n + 3 → O(n)

    Tmax(n) = 1 + 1 + n + (n-1) + (n-1) + (n-1) + n + 1 + 1
            = 4n + 3 → O(n)



Kesimpulannya:
- Tmin(n) = 2n → O(n)
- Tmax(n) = 3n - 1 → O(n)
- Kompleksitas waktu algoritma pencarian minimum linear, baik best case maupun worst case.
*/