#include <stdio.h>
#include <string.h>
#include "app.h"

/* Operasi inti: menjaga koleksi, BST, dan graph tetap sinkron (tanpa riwayat). */
static int sisipkan_ke_semua(AppState *app, const Mahasiswa *m) {
    if (!koleksi_tambah(&app->koleksi, m)) {
        return 0;
    }
    app->akar_bst = bst_sisip(app->akar_bst, m);
    graf_tambah_simpul(&app->graf, m->id);
    return 1;
}

static int buang_dari_semua(AppState *app, int id, Mahasiswa *dihapus) {
    if (!koleksi_hapus_id(&app->koleksi, id, dihapus)) {
        return 0;
    }
    app->akar_bst = bst_hapus(app->akar_bst, id);
    graf_hapus_simpul(&app->graf, id);
    return 1;
}

static int set_skor_di_semua(AppState *app, int id, int skor) {
    int indeks = koleksi_cari_indeks_id(&app->koleksi, id);
    BSTNode *node = bst_cari(app->akar_bst, id);

    if (indeks == -1) {
        return 0;
    }
    app->koleksi.data[indeks].skor = skor;
    if (node != NULL) {
        node->data.skor = skor;
    }
    return 1;
}

int app_init(AppState *app, const TabelAkun *tabel_akun, const char *username) {
    memset(app, 0, sizeof(*app));
    app->tabel_akun = tabel_akun;
    strncpy(app->username_aktif, username, MAX_USERNAME);

    if (!koleksi_init(&app->koleksi, KAPASITAS_AWAL)) {
        return 0;
    }
    koleksi_muat_file(&app->koleksi, FILE_MAHASISWA);
    app->akar_bst = bst_bangun(&app->koleksi);

    graf_init(&app->graf);
    for (int i = 0; i < app->koleksi.jumlah; i++) {
        graf_tambah_simpul(&app->graf, app->koleksi.data[i].id);
    }
    graf_muat_file(&app->graf, FILE_GRAPH);

    stack_init(&app->riwayat);
    queue_init(&app->antrean);
    return 1;
}

void app_bebas(AppState *app) {
    koleksi_bebas(&app->koleksi);
    bst_bebas(app->akar_bst);
    app->akar_bst = NULL;
    stack_bebas(&app->riwayat);
    queue_bebas(&app->antrean);
}

int app_simpan(const AppState *app) {
    int mahasiswa_ok = koleksi_simpan_file(&app->koleksi, FILE_MAHASISWA);
    int graf_ok = graf_simpan_file(&app->graf, FILE_GRAPH);
    return mahasiswa_ok && graf_ok;
}

int app_tambah(AppState *app, const Mahasiswa *m) {
    AksiUndo aksi;

    if (!sisipkan_ke_semua(app, m)) {
        return 0;
    }
    aksi.jenis = AKSI_TAMBAH;
    aksi.data = *m;
    aksi.skor_lama = 0;
    stack_push(&app->riwayat, &aksi);
    return 1;
}

int app_hapus(AppState *app, int id) {
    AksiUndo aksi;

    if (!buang_dari_semua(app, id, &aksi.data)) {
        return 0;
    }
    aksi.jenis = AKSI_HAPUS;
    aksi.skor_lama = 0;
    stack_push(&app->riwayat, &aksi);
    return 1;
}

int app_ubah_skor(AppState *app, int id, int skor_baru) {
    AksiUndo aksi;
    int indeks = koleksi_cari_indeks_id(&app->koleksi, id);

    if (indeks == -1) {
        return 0;
    }
    aksi.jenis = AKSI_UBAH_SKOR;
    aksi.skor_lama = app->koleksi.data[indeks].skor;
    set_skor_di_semua(app, id, skor_baru);
    aksi.data = app->koleksi.data[indeks];
    stack_push(&app->riwayat, &aksi);
    return 1;
}

int app_undo(AppState *app, AksiUndo *aksi_dibatalkan) {
    if (!stack_pop(&app->riwayat, aksi_dibatalkan)) {
        return 0;
    }
    switch (aksi_dibatalkan->jenis) {
        case AKSI_TAMBAH:
            buang_dari_semua(app, aksi_dibatalkan->data.id, NULL);
            break;
        case AKSI_HAPUS:
            sisipkan_ke_semua(app, &aksi_dibatalkan->data);
            break;
        case AKSI_UBAH_SKOR:
            set_skor_di_semua(app, aksi_dibatalkan->data.id, aksi_dibatalkan->skor_lama);
            break;
    }
    return 1;
}
