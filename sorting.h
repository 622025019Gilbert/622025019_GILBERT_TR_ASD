#ifndef SORTING_H
#define SORTING_H

#include "mahasiswa.h"

typedef enum { URUT_ID, URUT_NAMA, URUT_SKOR } KriteriaUrut;
typedef enum { URUT_NAIK, URUT_TURUN } ArahUrut;

typedef struct {
    long perbandingan;
    long perpindahan;
} StatistikSort;

void bubble_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat);
void selection_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat);
void insertion_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat);
void quick_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat);
int  merge_sort(Mahasiswa a[], int n, KriteriaUrut kriteria, ArahUrut arah, StatistikSort *stat);

#endif
