#ifndef CONFIG_H
#define CONFIG_H

/* Batas data */
#define MAX_MAHASISWA        100
#define KAPASITAS_AWAL       16
#define MAX_NAMA             60
#define MAX_USERNAME         30
#define MIN_USERNAME         3
#define MAX_PASSWORD         30
#define MIN_PASSWORD         4
#define ID_MIN               1
#define ID_MAX               9999
#define SKOR_MIN             0
#define SKOR_MAX             100

/* Penilaian */
#define BATAS_GRADE_A        85
#define BATAS_GRADE_B        70
#define BATAS_GRADE_C        60
#define BATAS_GRADE_D        50
#define BATAS_LULUS          BATAS_GRADE_C

/* Hash table */
#define HASH_SIZE            31
#define HASH_AWAL            5381u

/* Login dan input */
#define MAX_PERCOBAAN_LOGIN  3
#define BUFFER_ANGKA         32
#define MAX_BARIS_FILE       256

/* Tampilan */
#define LEBAR_GARIS          56
#define LEBAR_BAR_LOADING    20
#define JEDA_SINGKAT_MS      700

/* Berkas database (dijalankan dari root repositori) */
#define FILE_AKUN            "data/akun.txt"
#define FILE_MAHASISWA       "data/mahasiswa.txt"
#define FILE_GRAPH           "data/graph.txt"

#endif
