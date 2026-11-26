#include "searching.h"
#include "utils.h"

int sequential_search_nama(const Mahasiswa a[], int n, const char *kata, int hasil[], int maks_hasil) {
    int ditemukan = 0;

    for (int i = 0; i < n && ditemukan < maks_hasil; i++) {
        if (mengandung_teks_abaikan_case(a[i].nama, kata)) {
            hasil[ditemukan++] = i;
        }
    }
    return ditemukan;
}

int binary_search_id(const Mahasiswa a[], int n, int id, int *langkah) {
    int kiri = 0;
    int kanan = n - 1;

    *langkah = 0;
    while (kiri <= kanan) {
        int tengah = kiri + (kanan - kiri) / 2;
        (*langkah)++;
        if (a[tengah].id == id) {
            return tengah;
        }
        if (a[tengah].id < id) {
            kiri = tengah + 1;
        } else {
            kanan = tengah - 1;
        }
    }
    return -1;
}
