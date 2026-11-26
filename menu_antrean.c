#include <stdio.h>
#include "menu.h"
#include "utils.h"

static void daftarkan_calon(AppState *app) {
    CalonMahasiswa calon;

    baca_teks_aman("Nama calon mahasiswa : ", calon.nama, sizeof(calon.nama));
    calon.skor = baca_int_aman("Skor tes awal (0-100) : ", SKOR_MIN, SKOR_MAX);
    if (queue_enqueue(&app->antrean, &calon)) {
        printf("'%s' masuk antrean nomor %d.\n", calon.nama, app->antrean.ukuran);
    } else {
        printf("Gagal masuk antrean: memori tidak cukup.\n");
    }
}

static void proses_antrean_terdepan(AppState *app) {
    CalonMahasiswa calon;
    Mahasiswa baru;

    if (queue_kosong(&app->antrean)) {
        printf("Antrean kosong, tidak ada yang bisa diproses.\n");
        return;
    }
    baru.id = koleksi_id_berikutnya(&app->koleksi);
    if (app->koleksi.jumlah >= MAX_MAHASISWA || baru.id > ID_MAX) {
        printf("Data penuh, antrean belum bisa diproses.\n");
        return;
    }
    queue_dequeue(&app->antrean, &calon);
    snprintf(baru.nama, sizeof(baru.nama), "%s", calon.nama);
    baru.skor = calon.skor;

    if (app_tambah(app, &baru)) {
        printf("'%s' diproses & terdaftar dengan ID %d. Sisa antrean: %d\n",
               baru.nama, baru.id, app->antrean.ukuran);
        simpan_dengan_pesan(app);
    }
}

void menu_antrean(AppState *app) {
    int pilihan;

    do {
        bersihkan_layar();
        cetak_judul("ANTREAN PENDAFTARAN (QUEUE / FIFO)");
        queue_tampilkan(&app->antrean);
        cetak_garis('-');
        printf("1. Daftarkan calon ke antrean (enqueue)\n");
        printf("2. Proses calon terdepan -> jadi mahasiswa (dequeue)\n");
        printf("0. Kembali\n");
        pilihan = baca_int_aman("Pilih menu [0-2]: ", 0, 2);

        if (pilihan == 1) {
            daftarkan_calon(app);
        } else if (pilihan == 2) {
            proses_antrean_terdepan(app);
        }
        if (pilihan != 0) {
            tekan_enter_untuk_lanjut();
        }
    } while (pilihan != 0);
}
