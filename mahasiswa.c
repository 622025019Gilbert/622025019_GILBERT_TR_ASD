#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mahasiswa.h"

/* 59 = MAX_NAMA - 1 */
#define FORMAT_BARIS_MAHASISWA "%d|%59[^|]|%d"

static int data_mahasiswa_valid(const Mahasiswa *m) {
    return m->id >= ID_MIN && m->id <= ID_MAX &&
           m->skor >= SKOR_MIN && m->skor <= SKOR_MAX;
}

static int koleksi_perluas(KoleksiMahasiswa *k) {
    int kapasitas_baru = (k->kapasitas == 0) ? KAPASITAS_AWAL : k->kapasitas * 2;
    Mahasiswa *baru = (Mahasiswa *)realloc(k->data, (size_t)kapasitas_baru * sizeof(Mahasiswa));

    if (baru == NULL) {
        return 0;
    }
    k->data = baru;
    k->kapasitas = kapasitas_baru;
    return 1;
}

int koleksi_init(KoleksiMahasiswa *k, int kapasitas_awal) {
    k->data = (Mahasiswa *)malloc((size_t)kapasitas_awal * sizeof(Mahasiswa));
    k->jumlah = 0;
    k->kapasitas = (k->data != NULL) ? kapasitas_awal : 0;
    return k->data != NULL;
}

void koleksi_bebas(KoleksiMahasiswa *k) {
    free(k->data);
    k->data = NULL;
    k->jumlah = 0;
    k->kapasitas = 0;
}

int koleksi_cari_indeks_id(const KoleksiMahasiswa *k, int id) {
    for (int i = 0; i < k->jumlah; i++) {
        if (k->data[i].id == id) {
            return i;
        }
    }
    return -1;
}

int koleksi_tambah(KoleksiMahasiswa *k, const Mahasiswa *m) {
    if (k->jumlah >= MAX_MAHASISWA || !data_mahasiswa_valid(m)) {
        return 0;
    }
    if (koleksi_cari_indeks_id(k, m->id) != -1) {
        return 0;
    }
    if (k->jumlah >= k->kapasitas && !koleksi_perluas(k)) {
        return 0;
    }
    k->data[k->jumlah++] = *m;
    return 1;
}

int koleksi_hapus_id(KoleksiMahasiswa *k, int id, Mahasiswa *dihapus) {
    int indeks = koleksi_cari_indeks_id(k, id);

    if (indeks == -1) {
        return 0;
    }
    if (dihapus != NULL) {
        *dihapus = k->data[indeks];
    }
    for (int i = indeks; i < k->jumlah - 1; i++) {
        k->data[i] = k->data[i + 1];
    }
    k->jumlah--;
    return 1;
}

int koleksi_id_berikutnya(const KoleksiMahasiswa *k) {
    int terbesar = ID_MIN - 1;

    for (int i = 0; i < k->jumlah; i++) {
        if (k->data[i].id > terbesar) {
            terbesar = k->data[i].id;
        }
    }
    return terbesar + 1;
}

Mahasiswa *koleksi_salin_array(const KoleksiMahasiswa *k) {
    size_t banyak = (k->jumlah > 0) ? (size_t)k->jumlah : 1;
    Mahasiswa *salinan = (Mahasiswa *)malloc(banyak * sizeof(Mahasiswa));

    if (salinan != NULL && k->jumlah > 0) {
        memcpy(salinan, k->data, (size_t)k->jumlah * sizeof(Mahasiswa));
    }
    return salinan;
}

int koleksi_muat_file(KoleksiMahasiswa *k, const char *path) {
    FILE *berkas = fopen(path, "r");
    char baris[MAX_BARIS_FILE];

    if (berkas == NULL) {
        return 0;
    }
    while (fgets(baris, sizeof(baris), berkas) != NULL) {
        Mahasiswa m;
        if (sscanf(baris, FORMAT_BARIS_MAHASISWA, &m.id, m.nama, &m.skor) == 3) {
            koleksi_tambah(k, &m);
        }
    }
    fclose(berkas);
    return 1;
}

int koleksi_simpan_file(const KoleksiMahasiswa *k, const char *path) {
    FILE *berkas = fopen(path, "w");

    if (berkas == NULL) {
        return 0;
    }
    for (int i = 0; i < k->jumlah; i++) {
        fprintf(berkas, "%d|%s|%d\n", k->data[i].id, k->data[i].nama, k->data[i].skor);
    }
    fclose(berkas);
    return 1;
}

const char *skor_ke_grade(int skor) {
    if (skor >= BATAS_GRADE_A) return "A";
    if (skor >= BATAS_GRADE_B) return "B";
    if (skor >= BATAS_GRADE_C) return "C";
    if (skor >= BATAS_GRADE_D) return "D";
    return "E";
}

void koleksi_hitung_statistik(const KoleksiMahasiswa *k, StatistikNilai *hasil) {
    long total = 0;

    hasil->jumlah = k->jumlah;
    hasil->rata_rata = 0.0;
    hasil->tertinggi = SKOR_MIN;
    hasil->terendah = SKOR_MAX;
    hasil->jumlah_lulus = 0;

    for (int i = 0; i < k->jumlah; i++) {
        int skor = k->data[i].skor;
        total += skor;
        if (skor > hasil->tertinggi) hasil->tertinggi = skor;
        if (skor < hasil->terendah) hasil->terendah = skor;
        if (skor >= BATAS_LULUS) hasil->jumlah_lulus++;
    }
    if (k->jumlah > 0) {
        hasil->rata_rata = (double)total / k->jumlah;
    } else {
        hasil->terendah = SKOR_MIN;
    }
}

void mahasiswa_cetak_baris(const Mahasiswa *m) {
    printf("| %-5d | %-28s | %5d | %-5s |\n", m->id, m->nama, m->skor, skor_ke_grade(m->skor));
}

void mahasiswa_cetak_tabel(const Mahasiswa data[], int jumlah) {
    printf("+-------+------------------------------+-------+-------+\n");
    printf("| ID    | Nama                         | Skor  | Grade |\n");
    printf("+-------+------------------------------+-------+-------+\n");
    for (int i = 0; i < jumlah; i++) {
        mahasiswa_cetak_baris(&data[i]);
    }
    printf("+-------+------------------------------+-------+-------+\n");
    printf("Total: %d mahasiswa\n", jumlah);
}
