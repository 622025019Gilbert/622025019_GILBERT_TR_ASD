#include <stdio.h>
#include "menu.h"
#include "utils.h"

int minta_id_terdaftar(const AppState *app, const char *prompt, int *id) {
    *id = baca_int_aman(prompt, ID_MIN, ID_MAX);
    if (koleksi_cari_indeks_id(&app->koleksi, *id) == -1) {
        printf("Mahasiswa dengan ID %d tidak ditemukan.\n", *id);
        return 0;
    }
    return 1;
}

void simpan_dengan_pesan(const AppState *app) {
    if (!app_simpan(app)) {
        printf("Peringatan: data gagal disimpan ke berkas di folder data/.\n");
    }
}

void menu_utama(AppState *app) {
    int pilihan;

    do {
        bersihkan_layar();
        cetak_judul("MENU UTAMA");
        printf("Pengguna : %s\n", app->username_aktif);
        printf("Data     : %d mahasiswa\n", app->koleksi.jumlah);
        cetak_garis('-');
        printf("1. Kelola Data Mahasiswa (+ Undo)\n");
        printf("2. Pengurutan Data\n");
        printf("3. Pencarian Data\n");
        printf("4. Antrean Pendaftaran\n");
        printf("5. Indeks Pohon (BST)\n");
        printf("6. Relasi Teman Belajar (Graph)\n");
        printf("7. Akun & Hash Table\n");
        printf("8. Logout\n");
        pilihan = baca_int_aman("Pilih menu [1-8]: ", 1, 8);

        switch (pilihan) {
            case 1: menu_kelola_data(app); break;
            case 2: menu_sorting(app);     break;
            case 3: menu_pencarian(app);   break;
            case 4: menu_antrean(app);     break;
            case 5: menu_bst(app);         break;
            case 6: menu_graph(app);       break;
            case 7: menu_hash_table(app);  break;
            default: break;
        }
    } while (pilihan != 8);

    printf("\nLogout berhasil. Data tersimpan.\n");
    jeda_ms(JEDA_SINGKAT_MS);
}
