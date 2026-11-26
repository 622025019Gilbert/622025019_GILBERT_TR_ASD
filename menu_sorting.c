#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "sorting.h"
#include "utils.h"

#define JUMLAH_ALGORITMA 5

static const char *NAMA_ALGORITMA[JUMLAH_ALGORITMA] = {
    "Bubble Sort", "Selection Sort", "Insertion Sort", "Quick Sort", "Merge Sort"
};
static const char *NAMA_KRITERIA[] = {"ID", "Nama", "Skor"};

static int jalankan_algoritma(int nomor, Mahasiswa a[], int n, KriteriaUrut kriteria,
                              ArahUrut arah, StatistikSort *stat) {
    switch (nomor) {
        case 1: bubble_sort(a, n, kriteria, arah, stat);    return 1;
        case 2: selection_sort(a, n, kriteria, arah, stat); return 1;
        case 3: insertion_sort(a, n, kriteria, arah, stat); return 1;
        case 4: quick_sort(a, n, kriteria, arah, stat);     return 1;
        default: return merge_sort(a, n, kriteria, arah, stat);
    }
}

void menu_sorting(AppState *app) {
    KriteriaUrut kriteria;
    ArahUrut arah;
    int nomor_algoritma;
    StatistikSort stat;
    Mahasiswa *salinan;

    bersihkan_layar();
    cetak_judul("PENGURUTAN DATA MAHASISWA");
    if (app->koleksi.jumlah == 0) {
        printf("Belum ada data untuk diurutkan.\n");
        tekan_enter_untuk_lanjut();
        return;
    }
    kriteria = (KriteriaUrut)(baca_int_aman("Urut berdasarkan (1=ID, 2=Nama, 3=Skor): ", 1, 3) - 1);
    arah = (baca_int_aman("Arah urutan (1=Ascending, 2=Descending): ", 1, 2) == 1) ? URUT_NAIK : URUT_TURUN;
    printf("Algoritma: 1=Bubble 2=Selection 3=Insertion 4=Quick 5=Merge\n");
    nomor_algoritma = baca_int_aman("Pilih algoritma [1-5]: ", 1, JUMLAH_ALGORITMA);

    salinan = koleksi_salin_array(&app->koleksi);
    if (salinan == NULL) {
        printf("Memori tidak cukup.\n");
        tekan_enter_untuk_lanjut();
        return;
    }
    if (!jalankan_algoritma(nomor_algoritma, salinan, app->koleksi.jumlah, kriteria, arah, &stat)) {
        printf("Pengurutan gagal: memori tidak cukup.\n");
        free(salinan);
        tekan_enter_untuk_lanjut();
        return;
    }

    bersihkan_layar();
    printf("%s | berdasarkan %s | %s\n\n", NAMA_ALGORITMA[nomor_algoritma - 1],
           NAMA_KRITERIA[kriteria], (arah == URUT_NAIK) ? "Ascending" : "Descending");
    mahasiswa_cetak_tabel(salinan, app->koleksi.jumlah);
    printf("Perbandingan: %ld | Perpindahan/pertukaran: %ld\n\n", stat.perbandingan, stat.perpindahan);

    if (baca_int_aman("Terapkan urutan ini ke data utama? (1=Ya, 2=Tidak): ", 1, 2) == 1) {
        memcpy(app->koleksi.data, salinan, (size_t)app->koleksi.jumlah * sizeof(Mahasiswa));
        printf("Urutan diterapkan dan disimpan.\n");
        simpan_dengan_pesan(app);
    }
    free(salinan);
    tekan_enter_untuk_lanjut();
}
