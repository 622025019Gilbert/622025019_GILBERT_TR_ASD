#include <stdlib.h>
#include <string.h>
#include "sorting.h"

static int bandingkan_dasar(const Mahasiswa *a, const Mahasiswa *b, KriteriaUrut kriteria) {
    int hasil;

    switch (kriteria) {
        case URUT_NAMA: hasil = strcmp(a->nama, b->nama); break;
        case URUT_SKOR: hasil = a->skor - b->skor;        break;
        default:        hasil = a->id - b->id;            break;
    }
    return (hasil != 0) ? hasil : a->id - b->id;
}

/* Hasil < 0 berarti a harus berada sebelum b pada arah yang dipilih. */
static int bandingkan(const Mahasiswa *a, const Mahasiswa *b, KriteriaUrut kriteria, ArahUrut arah) {
    int hasil = bandingkan_dasar(a, b, kriteria);
    return (arah == URUT_NAIK) ? hasil : -hasil;
}

static void tukar(Mahasiswa *x, Mahasiswa *y) {
    Mahasiswa sementara = *x;
    *x = *y;
    *y = sementara;
}

static void reset_statistik(StatistikSort *stat) {
    stat->perbandingan = 0;
    stat->perpindahan = 0;
}

void bubble_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat) {
    reset_statistik(stat);
    for (int i = 0; i < n - 1; i++) {
        int ada_tukar = 0;
        for (int j = 0; j < n - i - 1; j++) {
            stat->perbandingan++;
            if (bandingkan(&a[j], &a[j + 1], kriteria, arah) > 0) {
                tukar(&a[j], &a[j + 1]);
                stat->perpindahan++;
                ada_tukar = 1;
            }
        }
        if (!ada_tukar) {
            break;
        }
    }
}

void selection_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat) {
    reset_statistik(stat);
    for (int i = 0; i < n - 1; i++) {
        int terpilih = i;
        for (int j = i + 1; j < n; j++) {
            stat->perbandingan++;
            if (bandingkan(&a[j], &a[terpilih], kriteria, arah) < 0) {
                terpilih = j;
            }
        }
        if (terpilih != i) {
            tukar(&a[i], &a[terpilih]);
            stat->perpindahan++;
        }
    }
}

void insertion_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat) {
    reset_statistik(stat);
    for (int i = 1; i < n; i++) {
        Mahasiswa kunci = a[i];
        int j = i - 1;
        while (j >= 0) {
            stat->perbandingan++;
            if (bandingkan(&a[j], &kunci, kriteria, arah) <= 0) {
                break;
            }
            a[j + 1] = a[j];
            stat->perpindahan++;
            j--;
        }
        a[j + 1] = kunci;
    }
}

static int partisi(Mahasiswa a[], int rendah, int tinggi, KriteriaUrut kriteria,
                   ArahUrut arah, StatistikSort *stat) {
    Mahasiswa pivot = a[tinggi];
    int batas = rendah - 1;

    for (int j = rendah; j < tinggi; j++) {
        stat->perbandingan++;
        if (bandingkan(&a[j], &pivot, kriteria, arah) < 0) {
            batas++;
            tukar(&a[batas], &a[j]);
            stat->perpindahan++;
        }
    }
    tukar(&a[batas + 1], &a[tinggi]);
    stat->perpindahan++;
    return batas + 1;
}

static void quick_sort_rekursif(Mahasiswa a[], int rendah, int tinggi, KriteriaUrut kriteria,
                                ArahUrut arah, StatistikSort *stat) {
    if (rendah < tinggi) {
        int p = partisi(a, rendah, tinggi, kriteria, arah, stat);
        quick_sort_rekursif(a, rendah, p - 1, kriteria, arah, stat);
        quick_sort_rekursif(a, p + 1, tinggi, kriteria, arah, stat);
    }
}

void quick_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat) {
    reset_statistik(stat);
    quick_sort_rekursif(a, 0, n - 1, kriteria, arah, stat);
}

static void gabung(Mahasiswa a[], Mahasiswa sementara[], int kiri, int tengah, int kanan,
                   KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat) {
    int i = kiri;
    int j = tengah + 1;

    memcpy(&sementara[kiri], &a[kiri], (size_t)(kanan - kiri + 1) * sizeof(Mahasiswa));
    for (int k = kiri; k <= kanan; k++) {
        if (i > tengah) {
            a[k] = sementara[j++];
        } else if (j > kanan) {
            a[k] = sementara[i++];
        } else {
            stat->perbandingan++;
            if (bandingkan(&sementara[i], &sementara[j], kriteria, arah) <= 0) {
                a[k] = sementara[i++];
            } else {
                a[k] = sementara[j++];
            }
        }
        stat->perpindahan++;
    }
}

static void merge_sort_rekursif(Mahasiswa a[], Mahasiswa sementara[], int kiri, int kanan,
                                KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat) {
    if (kiri < kanan) {
        int tengah = kiri + (kanan - kiri) / 2;
        merge_sort_rekursif(a, sementara, kiri, tengah, kriteria, arah, stat);
        merge_sort_rekursif(a, sementara, tengah + 1, kanan, kriteria, arah, stat);
        gabung(a, sementara, kiri, tengah, kanan, kriteria, arah, stat);
    }
}

int merge_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat) {
    Mahasiswa *sementara;

    reset_statistik(stat);
    if (n <= 1) {
        return 1;
    }
    sementara = (Mahasiswa *)malloc((size_t)n * sizeof(Mahasiswa));
    if (sementara == NULL) {
        return 0;
    }
    merge_sort_rekursif(a, sementara, 0, n - 1, kriteria, arah, stat);
    free(sementara);
    return 1;
}
