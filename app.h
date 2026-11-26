#ifndef APP_H
#define APP_H

#include "mahasiswa.h"
#include "bst.h"
#include "graph.h"
#include "stack_queue.h"
#include "hash_table.h"

/* Satu sumber kebenaran untuk seluruh struktur data yang saling terhubung. */
typedef struct {
    KoleksiMahasiswa koleksi;          /* array dinamis (Topik 2) */
    BSTNode *akar_bst;                 /* indeks pohon berdasarkan ID (Topik 7) */
    GrafRelasi graf;                   /* relasi teman belajar (Topik 8) */
    TumpukanUndo riwayat;              /* stack undo (Topik 4) */
    AntreanPendaftaran antrean;        /* queue pendaftaran (Topik 4) */
    const TabelAkun *tabel_akun;       /* hash table akun (Topik 6) */
    char username_aktif[MAX_USERNAME + 1];
} AppState;

int  app_init(AppState *app, const TabelAkun *tabel_akun, const char *username);
void app_bebas(AppState *app);
int  app_simpan(const AppState *app);

/* Aksi dengan pencatatan riwayat (bisa di-undo) */
int  app_tambah(AppState *app, const Mahasiswa *m);
int  app_hapus(AppState *app, int id);
int  app_ubah_skor(AppState *app, int id, int skor_baru);
int  app_undo(AppState *app, AksiUndo *aksi_dibatalkan);

#endif
