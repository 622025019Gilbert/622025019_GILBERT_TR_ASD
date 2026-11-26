#include <stdio.h>
#include <string.h>
#include "graph.h"

static const char *nama_dari_id(const KoleksiMahasiswa *k, int id) {
    int indeks = koleksi_cari_indeks_id(k, id);
    return (indeks >= 0) ? k->data[indeks].nama : "?";
}

void graf_init(GrafRelasi *g) {
    memset(g, 0, sizeof(*g));
}

int graf_cari_indeks(const GrafRelasi *g, int id) {
    for (int i = 0; i < g->jumlah_simpul; i++) {
        if (g->id_simpul[i] == id) {
            return i;
        }
    }
    return -1;
}

int graf_tambah_simpul(GrafRelasi *g, int id) {
    if (graf_cari_indeks(g, id) != -1 || g->jumlah_simpul >= MAX_MAHASISWA) {
        return 0;
    }
    g->id_simpul[g->jumlah_simpul++] = id;
    return 1;
}

int graf_hapus_simpul(GrafRelasi *g, int id) {
    int indeks = graf_cari_indeks(g, id);
    int n = g->jumlah_simpul;

    if (indeks == -1) {
        return 0;
    }
    for (int i = indeks; i < n - 1; i++) {
        g->id_simpul[i] = g->id_simpul[i + 1];
    }
    for (int baris = 0; baris < n; baris++) {
        for (int kolom = indeks; kolom < n - 1; kolom++) {
            g->sisi[baris][kolom] = g->sisi[baris][kolom + 1];
        }
    }
    for (int baris = indeks; baris < n - 1; baris++) {
        memcpy(g->sisi[baris], g->sisi[baris + 1], sizeof(g->sisi[baris]));
    }
    for (int i = 0; i < n; i++) {
        g->sisi[n - 1][i] = 0;
        g->sisi[i][n - 1] = 0;
    }
    g->id_simpul[n - 1] = 0;
    g->jumlah_simpul--;
    return 1;
}

HasilSisi graf_tambah_sisi(GrafRelasi *g, int id_a, int id_b) {
    int a = graf_cari_indeks(g, id_a);
    int b = graf_cari_indeks(g, id_b);

    if (a == -1 || b == -1) {
        return SISI_SIMPUL_TIDAK_ADA;
    }
    if (a == b) {
        return SISI_KE_DIRI_SENDIRI;
    }
    if (g->sisi[a][b]) {
        return SISI_SUDAH_ADA;
    }
    g->sisi[a][b] = 1;
    g->sisi[b][a] = 1;
    return SISI_BERHASIL;
}

int graf_hapus_sisi(GrafRelasi *g, int id_a, int id_b) {
    int a = graf_cari_indeks(g, id_a);
    int b = graf_cari_indeks(g, id_b);

    if (a == -1 || b == -1 || !g->sisi[a][b]) {
        return 0;
    }
    g->sisi[a][b] = 0;
    g->sisi[b][a] = 0;
    return 1;
}

void graf_tampilkan_relasi(const GrafRelasi *g, const KoleksiMahasiswa *k) {
    int ada_relasi = 0;

    for (int i = 0; i < g->jumlah_simpul; i++) {
        int jumlah_teman = 0;
        printf("[%d] %-20s : ", g->id_simpul[i], nama_dari_id(k, g->id_simpul[i]));
        for (int j = 0; j < g->jumlah_simpul; j++) {
            if (g->sisi[i][j]) {
                printf("%s%s", jumlah_teman > 0 ? ", " : "", nama_dari_id(k, g->id_simpul[j]));
                jumlah_teman++;
            }
        }
        if (jumlah_teman == 0) {
            printf("(belum punya teman belajar)");
        } else {
            ada_relasi = 1;
        }
        printf("\n");
    }
    if (!ada_relasi) {
        printf("\nBelum ada relasi teman belajar yang tercatat.\n");
    }
}

void graf_bfs(const GrafRelasi *g, const KoleksiMahasiswa *k, int id_awal) {
    int awal = graf_cari_indeks(g, id_awal);
    int dikunjungi[MAX_MAHASISWA] = {0};
    int jarak[MAX_MAHASISWA] = {0};
    int antrean[MAX_MAHASISWA];
    int depan = 0;
    int belakang = 0;

    if (awal == -1) {
        printf("Simpul dengan ID %d tidak ada di graph.\n", id_awal);
        return;
    }
    dikunjungi[awal] = 1;
    antrean[belakang++] = awal;

    printf("BFS: jangkauan teman belajar dari %s (urut dari yang terdekat)\n", nama_dari_id(k, id_awal));
    while (depan < belakang) {
        int u = antrean[depan++];
        printf("  Jarak %d -> [%d] %s\n", jarak[u], g->id_simpul[u], nama_dari_id(k, g->id_simpul[u]));
        for (int v = 0; v < g->jumlah_simpul; v++) {
            if (g->sisi[u][v] && !dikunjungi[v]) {
                dikunjungi[v] = 1;
                jarak[v] = jarak[u] + 1;
                antrean[belakang++] = v;
            }
        }
    }
    printf("Total mahasiswa terjangkau (termasuk diri sendiri): %d dari %d\n", belakang, g->jumlah_simpul);
}

static void dfs_rekursif(const GrafRelasi *g, const KoleksiMahasiswa *k, int u,
                         int dikunjungi[], int kedalaman) {
    dikunjungi[u] = 1;
    for (int i = 0; i < kedalaman; i++) {
        printf("   ");
    }
    printf("%s[%d] %s\n", kedalaman > 0 ? "-> " : "", g->id_simpul[u], nama_dari_id(k, g->id_simpul[u]));
    for (int v = 0; v < g->jumlah_simpul; v++) {
        if (g->sisi[u][v] && !dikunjungi[v]) {
            dfs_rekursif(g, k, v, dikunjungi, kedalaman + 1);
        }
    }
}

void graf_dfs(const GrafRelasi *g, const KoleksiMahasiswa *k, int id_awal) {
    int awal = graf_cari_indeks(g, id_awal);
    int dikunjungi[MAX_MAHASISWA] = {0};

    if (awal == -1) {
        printf("Simpul dengan ID %d tidak ada di graph.\n", id_awal);
        return;
    }
    printf("DFS: rantai penyebaran materi dari %s (menjorok = semakin jauh)\n", nama_dari_id(k, id_awal));
    dfs_rekursif(g, k, awal, dikunjungi, 0);
}

int graf_muat_file(GrafRelasi *g, const char *path) {
    FILE *berkas = fopen(path, "r");
    char baris[MAX_BARIS_FILE];

    if (berkas == NULL) {
        return 0;
    }
    while (fgets(baris, sizeof(baris), berkas) != NULL) {
        int id_a;
        int id_b;
        if (sscanf(baris, "%d|%d", &id_a, &id_b) == 2) {
            graf_tambah_sisi(g, id_a, id_b);
        }
    }
    fclose(berkas);
    return 1;
}

int graf_simpan_file(const GrafRelasi *g, const char *path) {
    FILE *berkas = fopen(path, "w");

    if (berkas == NULL) {
        return 0;
    }
    for (int i = 0; i < g->jumlah_simpul; i++) {
        for (int j = i + 1; j < g->jumlah_simpul; j++) {
            if (g->sisi[i][j]) {
                fprintf(berkas, "%d|%d\n", g->id_simpul[i], g->id_simpul[j]);
            }
        }
    }
    fclose(berkas);
    return 1;
}
