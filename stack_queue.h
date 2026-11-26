#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include "mahasiswa.h"

/* ---------- Stack: riwayat aksi untuk fitur UNDO ---------- */
typedef enum {
    AKSI_TAMBAH,
    AKSI_HAPUS,
    AKSI_UBAH_SKOR
} JenisAksi;

typedef struct {
    JenisAksi jenis;
    Mahasiswa data;     /* data mahasiswa yang terdampak */
    int skor_lama;      /* dipakai oleh AKSI_UBAH_SKOR */
} AksiUndo;

typedef struct TumpukanNode {
    AksiUndo aksi;
    struct TumpukanNode *berikutnya;
} TumpukanNode;

typedef struct {
    TumpukanNode *puncak;
    int ukuran;
} TumpukanUndo;

void stack_init(TumpukanUndo *s);
void stack_bebas(TumpukanUndo *s);
int  stack_kosong(const TumpukanUndo *s);
int  stack_push(TumpukanUndo *s, const AksiUndo *aksi);
int  stack_pop(TumpukanUndo *s, AksiUndo *aksi);
void stack_tampilkan(const TumpukanUndo *s);

/* ---------- Queue: antrean pendaftaran mahasiswa baru ---------- */
typedef struct {
    char nama[MAX_NAMA];
    int skor;
} CalonMahasiswa;

typedef struct AntreanNode {
    CalonMahasiswa calon;
    struct AntreanNode *berikutnya;
} AntreanNode;

typedef struct {
    AntreanNode *depan;
    AntreanNode *belakang;
    int ukuran;
} AntreanPendaftaran;

void queue_init(AntreanPendaftaran *q);
void queue_bebas(AntreanPendaftaran *q);
int  queue_kosong(const AntreanPendaftaran *q);
int  queue_enqueue(AntreanPendaftaran *q, const CalonMahasiswa *calon);
int  queue_dequeue(AntreanPendaftaran *q, CalonMahasiswa *calon);
void queue_tampilkan(const AntreanPendaftaran *q);

#endif
