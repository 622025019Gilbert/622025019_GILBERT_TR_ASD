#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "searching.h"
#include "sorting.h"
#include "utils.h"

static void cari_sequential_nama(const AppState *app) {
    char kata[MAX_NAMA];
    int hasil[MAX_MAHASISWA];
    int jumlah_temuan;

    baca_teks_aman("Masukkan nama/sebagian nama: ", kata, sizeof(kata));
    jumlah_temuan = sequential_search_nama(app->koleksi.data, app->koleksi.jumlah, kata,
                                           hasil, MAX_MAHASISWA);
    if (jumlah_temuan == 0) {
        printf("Data dengan nama '%s' tidak ditemukan.\n", kata);
        return;
    }
    printf("Ditemukan %d data:\n", jumlah_temuan);
    for (int i = 0; i < jumlah_temuan; i++) {
        mahasiswa_cetak_baris(&app->koleksi.data[hasil[i]]);
    }
}

static void cari_binary_id(const AppState *app) {
    int id = baca_int_aman("Masukkan ID yang dicari: ", ID_MIN, ID_MAX);
    int langkah = 0;
    int indeks;
    StatistikSort stat;
    Mahasiswa *terurut = koleksi_salin_array(&app->koleksi);

    if (terurut == NULL || !merge_sort(terurut, app->koleksi.jumlah, URUT_ID, URUT_NAIK, &stat)) {
        printf("Memori tidak cukup.\n");
        free(terurut);
        return;
    }
    printf("Data diurutkan menaik berdasarkan ID (Merge Sort) sebelum Binary Search.\n");
    indeks = binary_search_id(terurut, app->koleksi.jumlah, id, &langkah);
    if (indeks == -1) {
        printf("Data dengan ID %d tidak ditemukan (%d langkah pencarian).\n", id, langkah);
    } else {
        printf("Ditemukan pada indeks %d setelah %d langkah:\n", indeks, langkah);
        mahasiswa_cetak_baris(&terurut[indeks]);
    }
    free(terurut);
}

void menu_pencarian(AppState *app) {
    int metode;

    bersihkan_layar();
    cetak_judul("PENCARIAN DATA MAHASISWA");
    if (app->koleksi.jumlah == 0) {
        printf("Data kosong, pencarian tidak dapat dilakukan.\n");
        tekan_enter_untuk_lanjut();
        return;
    }
    printf("1. Sequential Search (berdasarkan nama)\n");
    printf("2. Binary Search (berdasarkan ID)\n");
    metode = baca_int_aman("Pilih metode [1-2]: ", 1, 2);

    if (metode == 1) {
        cari_sequential_nama(app);
    } else {
        cari_binary_id(app);
    }
    tekan_enter_untuk_lanjut();
}
