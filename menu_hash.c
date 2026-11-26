#include <stdio.h>
#include "menu.h"
#include "utils.h"

static void cari_akun(const AppState *app) {
    char username[MAX_USERNAME + 1];
    int langkah;
    AkunNode *akun;

    baca_teks_aman("Username yang dicari: ", username, sizeof(username));
    akun = tabel_cari(app->tabel_akun, username, &langkah);
    printf("Fungsi hash (djb2 mod %d) -> slot [%02d]\n", HASH_SIZE, tabel_indeks(username));
    if (akun == NULL) {
        printf("Username '%s' tidak ditemukan (%d perbandingan pada rantai).\n", username, langkah);
    } else {
        printf("Username '%s' ditemukan setelah %d perbandingan pada rantai slot.\n",
               akun->username, langkah);
    }
}

void menu_hash_table(AppState *app) {
    int pilihan;

    do {
        bersihkan_layar();
        cetak_judul("AKUN & HASH TABLE");
        printf("Login sebagai: %s\n", app->username_aktif);
        printf("Tabel akun memakai hash function sendiri + separate chaining.\n");
        cetak_garis('-');
        printf("1. Lihat struktur hash table akun\n");
        printf("2. Cari akun berdasarkan username\n");
        printf("0. Kembali\n");
        pilihan = baca_int_aman("Pilih menu [0-2]: ", 0, 2);

        if (pilihan == 0) {
            break;
        }
        bersihkan_layar();
        if (pilihan == 1) {
            tabel_tampilkan(app->tabel_akun);
        } else {
            cari_akun(app);
        }
        tekan_enter_untuk_lanjut();
    } while (1);
}
