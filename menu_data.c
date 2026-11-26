#include <stdio.h>
#include "menu.h"
#include "utils.h"

void proses_tambah_mahasiswa(AppState *app) {
    Mahasiswa baru;

    if (app->koleksi.jumlah >= MAX_MAHASISWA) {
        printf("Data penuh (maksimal %d mahasiswa).\n", MAX_MAHASISWA);
        return;
    }
    baru.id = baca_int_aman("ID mahasiswa        : ", ID_MIN, ID_MAX);
    if (koleksi_cari_indeks_id(&app->koleksi, baru.id) != -1) {
        printf("ID %d sudah digunakan. Penambahan dibatalkan.\n", baru.id);
        return;
    }
    baca_teks_aman("Nama mahasiswa      : ", baru.nama, sizeof(baru.nama));
    baru.skor = baca_int_aman("Skor (0-100)        : ", SKOR_MIN, SKOR_MAX);

    if (app_tambah(app, &baru)) {
        printf("Mahasiswa '%s' berhasil ditambahkan.\n", baru.nama);
        simpan_dengan_pesan(app);
    } else {
        printf("Gagal menambahkan mahasiswa.\n");
    }
}

void proses_hapus_mahasiswa(AppState *app) {
    int id;

    if (!minta_id_terdaftar(app, "ID mahasiswa yang dihapus: ", &id)) {
        return;
    }
    if (baca_int_aman("Yakin hapus? (1=Ya, 2=Tidak): ", 1, 2) == 2) {
        printf("Penghapusan dibatalkan.\n");
        return;
    }
    if (app_hapus(app, id)) {
        printf("Mahasiswa ID %d dihapus (relasi graph ikut terhapus). Bisa di-undo.\n", id);
        simpan_dengan_pesan(app);
    }
}

static void proses_ubah_skor(AppState *app) {
    int id;
    int skor_baru;

    if (!minta_id_terdaftar(app, "ID mahasiswa : ", &id)) {
        return;
    }
    skor_baru = baca_int_aman("Skor baru (0-100) : ", SKOR_MIN, SKOR_MAX);
    if (app_ubah_skor(app, id, skor_baru)) {
        printf("Skor mahasiswa ID %d diubah menjadi %d.\n", id, skor_baru);
        simpan_dengan_pesan(app);
    }
}

static void tampilkan_statistik(const AppState *app) {
    StatistikNilai stat;

    if (app->koleksi.jumlah == 0) {
        printf("Belum ada data untuk dihitung.\n");
        return;
    }
    koleksi_hitung_statistik(&app->koleksi, &stat);
    printf("Jumlah mahasiswa : %d\n", stat.jumlah);
    printf("Rata-rata skor   : %.2f\n", stat.rata_rata);
    printf("Skor tertinggi   : %d\n", stat.tertinggi);
    printf("Skor terendah    : %d\n", stat.terendah);
    printf("Lulus (>= %d)    : %d mahasiswa\n", BATAS_LULUS, stat.jumlah_lulus);
    printf("Tidak lulus      : %d mahasiswa\n", stat.jumlah - stat.jumlah_lulus);
}

static void proses_undo(AppState *app) {
    AksiUndo aksi;

    if (!app_undo(app, &aksi)) {
        printf("Tidak ada aksi yang bisa di-undo (stack kosong).\n");
        return;
    }
    switch (aksi.jenis) {
        case AKSI_TAMBAH:
            printf("Undo: penambahan '%s' dibatalkan.\n", aksi.data.nama);
            break;
        case AKSI_HAPUS:
            printf("Undo: '%s' dikembalikan (relasi graph-nya tidak ikut dipulihkan).\n", aksi.data.nama);
            break;
        case AKSI_UBAH_SKOR:
            printf("Undo: skor '%s' dikembalikan ke %d.\n", aksi.data.nama, aksi.skor_lama);
            break;
    }
    simpan_dengan_pesan(app);
}

void menu_kelola_data(AppState *app) {
    int pilihan;

    do {
        bersihkan_layar();
        cetak_judul("KELOLA DATA MAHASISWA");
        printf("1. Tampilkan semua data\n");
        printf("2. Tambah mahasiswa\n");
        printf("3. Ubah skor mahasiswa\n");
        printf("4. Hapus mahasiswa\n");
        printf("5. Statistik nilai\n");
        printf("6. Undo aksi terakhir\n");
        printf("7. Lihat riwayat aksi (isi stack)\n");
        printf("0. Kembali\n");
        pilihan = baca_int_aman("Pilih menu [0-7]: ", 0, 7);

        if (pilihan == 0) {
            break;
        }
        bersihkan_layar();
        switch (pilihan) {
            case 1:
                if (app->koleksi.jumlah == 0) printf("Belum ada data mahasiswa.\n");
                else mahasiswa_cetak_tabel(app->koleksi.data, app->koleksi.jumlah);
                break;
            case 2: proses_tambah_mahasiswa(app); break;
            case 3: proses_ubah_skor(app);        break;
            case 4: proses_hapus_mahasiswa(app);  break;
            case 5: tampilkan_statistik(app);     break;
            case 6: proses_undo(app);             break;
            case 7: stack_tampilkan(&app->riwayat); break;
        }
        tekan_enter_untuk_lanjut();
    } while (1);
}
