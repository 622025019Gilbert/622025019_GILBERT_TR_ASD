#include <stdio.h>
#include "menu.h"
#include "utils.h"

static void tambah_relasi(AppState *app) {
    int id_a;
    int id_b;

    if (!minta_id_terdaftar(app, "ID mahasiswa pertama : ", &id_a) ||
        !minta_id_terdaftar(app, "ID mahasiswa kedua   : ", &id_b)) {
        return;
    }
    switch (graf_tambah_sisi(&app->graf, id_a, id_b)) {
        case SISI_BERHASIL:
            printf("Relasi teman belajar %d <-> %d ditambahkan.\n", id_a, id_b);
            simpan_dengan_pesan(app);
            break;
        case SISI_KE_DIRI_SENDIRI:
            printf("Mahasiswa tidak bisa berteman dengan dirinya sendiri.\n");
            break;
        case SISI_SUDAH_ADA:
            printf("Relasi tersebut sudah ada.\n");
            break;
        default:
            printf("Salah satu mahasiswa tidak ada di graph.\n");
            break;
    }
}

static void hapus_relasi(AppState *app) {
    int id_a;
    int id_b;

    if (!minta_id_terdaftar(app, "ID mahasiswa pertama : ", &id_a) ||
        !minta_id_terdaftar(app, "ID mahasiswa kedua   : ", &id_b)) {
        return;
    }
    if (graf_hapus_sisi(&app->graf, id_a, id_b)) {
        printf("Relasi %d <-> %d dihapus.\n", id_a, id_b);
        simpan_dengan_pesan(app);
    } else {
        printf("Relasi antara ID %d dan %d tidak ditemukan.\n", id_a, id_b);
    }
}

static void jelajahi(const AppState *app, int gunakan_bfs) {
    int id_awal;

    if (!minta_id_terdaftar(app, "ID mahasiswa awal : ", &id_awal)) {
        return;
    }
    printf("\n");
    if (gunakan_bfs) {
        graf_bfs(&app->graf, &app->koleksi, id_awal);
    } else {
        graf_dfs(&app->graf, &app->koleksi, id_awal);
    }
}

void menu_graph(AppState *app) {
    int pilihan;

    do {
        bersihkan_layar();
        cetak_judul("RELASI TEMAN BELAJAR (GRAPH)");
        printf("1. Tambah relasi teman belajar\n");
        printf("2. Hapus relasi\n");
        printf("3. Tampilkan semua relasi\n");
        printf("4. Jelajah BFS (jangkauan terdekat)\n");
        printf("5. Jelajah DFS (rantai relasi)\n");
        printf("0. Kembali\n");
        pilihan = baca_int_aman("Pilih menu [0-5]: ", 0, 5);

        if (pilihan == 0) {
            break;
        }
        bersihkan_layar();
        switch (pilihan) {
            case 1: tambah_relasi(app);                           break;
            case 2: hapus_relasi(app);                            break;
            case 3: graf_tampilkan_relasi(&app->graf, &app->koleksi); break;
            case 4: jelajahi(app, 1);                             break;
            case 5: jelajahi(app, 0);                             break;
        }
        tekan_enter_untuk_lanjut();
    } while (1);
}
