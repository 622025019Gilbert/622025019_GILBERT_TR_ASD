#ifndef SEARCHING_H
#define SEARCHING_H

#include "mahasiswa.h"

/* Mengisi hasil[] dengan indeks yang namanya mengandung kata. Mengembalikan jumlah temuan. */
int sequential_search_nama(const Mahasiswa a[], int n, const char *kata, int hasil[], int maks_hasil);

/* Syarat: a[] sudah terurut naik berdasarkan ID. Mengembalikan indeks atau -1. */
int binary_search_id(const Mahasiswa a[], int n, int id, int *langkah);

#endif
