#ifndef GRAPH_H
#define GRAPH_H

#include "mahasiswa.h"

/* Graph tak berarah: simpul = mahasiswa (berdasarkan ID), sisi = teman belajar. */
typedef struct {
    int id_simpul[MAX_MAHASISWA];
    int jumlah_simpul;
    unsigned char sisi[MAX_MAHASISWA][MAX_MAHASISWA];
} GrafRelasi;

typedef enum {
    SISI_BERHASIL,
    SISI_SIMPUL_TIDAK_ADA,
    SISI_KE_DIRI_SENDIRI,
    SISI_SUDAH_ADA
} HasilSisi;

void graf_init(GrafRelasi *g);
int  graf_cari_indeks(const GrafRelasi *g, int id);
int  graf_tambah_simpul(GrafRelasi *g, int id);
int  graf_hapus_simpul(GrafRelasi *g, int id);

HasilSisi graf_tambah_sisi(GrafRelasi *g, int id_a, int id_b);
int       graf_hapus_sisi(GrafRelasi *g, int id_a, int id_b);

void graf_tampilkan_relasi(const GrafRelasi *g, const KoleksiMahasiswa *k);
void graf_bfs(const GrafRelasi *g, const KoleksiMahasiswa *k, int id_awal);
void graf_dfs(const GrafRelasi *g, const KoleksiMahasiswa *k, int id_awal);

int  graf_muat_file(GrafRelasi *g, const char *path);
int  graf_simpan_file(const GrafRelasi *g, const char *path);

#endif
