#ifndef MAHASISWA_H
#define MAHASISWA_H

#include "config.h"

typedef struct {
    int id;
    char nama[MAX_NAMA];
    int skor;
} Mahasiswa;

typedef struct {
    Mahasiswa *data;
    int jumlah;
    int kapasitas;
} KoleksiMahasiswa;

typedef struct {
    int jumlah;
    double rata_rata;
    int tertinggi;
    int terendah;
    int jumlah_lulus;
} StatistikNilai;

int  koleksi_init(KoleksiMahasiswa *k, int kapasitas_awal);
void koleksi_bebas(KoleksiMahasiswa *k);

int  koleksi_tambah(KoleksiMahasiswa *k, const Mahasiswa *m);
int  koleksi_hapus_id(KoleksiMahasiswa *k, int id, Mahasiswa *dihapus);
int  koleksi_cari_indeks_id(const KoleksiMahasiswa *k, int id);
int  koleksi_id_berikutnya(const KoleksiMahasiswa *k);
Mahasiswa *koleksi_salin_array(const KoleksiMahasiswa *k);

int  koleksi_muat_file(KoleksiMahasiswa *k, const char *path);
int  koleksi_simpan_file(const KoleksiMahasiswa *k, const char *path);

void koleksi_hitung_statistik(const KoleksiMahasiswa *k, StatistikNilai *hasil);
const char *skor_ke_grade(int skor);

void mahasiswa_cetak_baris(const Mahasiswa *m);
void mahasiswa_cetak_tabel(const Mahasiswa data[], int jumlah);

#endif
