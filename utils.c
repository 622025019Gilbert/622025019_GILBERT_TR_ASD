#ifndef _WIN32
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "config.h"

#ifdef _WIN32
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif

    void jeda_ms(int ms) {
        Sleep(ms);
    }

    void bersihkan_layar(void) {
        system("cls");
    }

    void aktifkan_warna_ansi(void) {
        HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (handle_out != INVALID_HANDLE_VALUE && GetConsoleMode(handle_out, &mode)) {
            SetConsoleMode(handle_out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#else
    #include <unistd.h>

    void jeda_ms(int ms) {
        usleep((useconds_t)ms * 1000);
    }

    void bersihkan_layar(void) {
        system("clear");
    }

    void aktifkan_warna_ansi(void) {
    }
#endif

typedef enum {
    BACA_OK,
    BACA_EOF,
    BACA_TERLALU_PANJANG
} StatusBaca;

static StatusBaca baca_baris(char *buffer, size_t ukuran) {
    if (fgets(buffer, (int)ukuran, stdin) == NULL) {
        return BACA_EOF;
    }
    if (strchr(buffer, '\n') == NULL) {
        int c = getchar();
        if (c != '\n' && c != EOF) {
            while (c != '\n' && c != EOF) {
                c = getchar();
            }
            return BACA_TERLALU_PANJANG;
        }
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return BACA_OK;
}

static void keluar_karena_eof(void) {
    printf("\nInput berakhir. Program ditutup.\n");
    exit(0);
}

static void potong_spasi(char *teks) {
    size_t awal = 0;
    size_t akhir = strlen(teks);

    while (teks[awal] != '\0' && isspace((unsigned char)teks[awal])) {
        awal++;
    }
    while (akhir > awal && isspace((unsigned char)teks[akhir - 1])) {
        akhir--;
    }
    memmove(teks, teks + awal, akhir - awal);
    teks[akhir - awal] = '\0';
}

void cetak_garis(char karakter) {
    for (int i = 0; i < LEBAR_GARIS; i++) {
        putchar(karakter);
    }
    putchar('\n');
}

void cetak_judul(const char *judul) {
    cetak_garis('=');
    printf("  %s\n", judul);
    cetak_garis('=');
}

void tampilkan_loading(void) {
    const int langkah = 100 / LEBAR_BAR_LOADING;

    bersihkan_layar();
    cetak_judul("MEMUAT DATA, HARAP TUNGGU...");
    for (int persen = 0; persen <= 100; persen += langkah * 4) {
        int terisi = persen / langkah;
        printf("\rProgress: [");
        for (int j = 0; j < LEBAR_BAR_LOADING; j++) {
            printf(j < terisi ? "\033[42m \033[0m" : " ");
        }
        printf("] %d%%", persen);
        fflush(stdout);
        jeda_ms(250);
    }
    printf("\n\nSelesai. Membuka menu utama...\n");
    jeda_ms(500);
}

int baca_int_aman(const char *prompt, int min_val, int max_val) {
    char buffer[BUFFER_ANGKA];
    char *sisa;
    long nilai;

    while (1) {
        printf("%s", prompt);
        StatusBaca status = baca_baris(buffer, sizeof(buffer));
        if (status == BACA_EOF) {
            keluar_karena_eof();
        }
        if (status == BACA_TERLALU_PANJANG) {
            printf("Input terlalu panjang. Coba lagi.\n");
            continue;
        }
        potong_spasi(buffer);
        if (buffer[0] == '\0') {
            printf("Input tidak boleh kosong. Coba lagi.\n");
            continue;
        }
        nilai = strtol(buffer, &sisa, 10);
        if (*sisa != '\0' || nilai < min_val || nilai > max_val) {
            printf("Input tidak valid! Masukkan angka %d sampai %d.\n", min_val, max_val);
            continue;
        }
        return (int)nilai;
    }
}

void baca_teks_aman(const char *prompt, char *dest, size_t ukuran) {
    while (1) {
        printf("%s", prompt);
        StatusBaca status = baca_baris(dest, ukuran);
        if (status == BACA_EOF) {
            keluar_karena_eof();
        }
        if (status == BACA_TERLALU_PANJANG) {
            printf("Input terlalu panjang (maksimal %d karakter).\n", (int)ukuran - 1);
            continue;
        }
        potong_spasi(dest);
        if (dest[0] == '\0') {
            printf("Input tidak boleh kosong. Silakan isi kembali.\n");
            continue;
        }
        if (strchr(dest, '|') != NULL) {
            printf("Karakter '|' tidak diperbolehkan.\n");
            continue;
        }
        return;
    }
}

void tekan_enter_untuk_lanjut(void) {
    char sisa[BUFFER_ANGKA];

    printf("\nTekan Enter untuk lanjut...");
    if (baca_baris(sisa, sizeof(sisa)) == BACA_EOF) {
        keluar_karena_eof();
    }
}

int mengandung_teks_abaikan_case(const char *teks, const char *kata) {
    size_t panjang_kata = strlen(kata);

    if (panjang_kata == 0) {
        return 1;
    }
    for (; *teks != '\0'; teks++) {
        size_t i = 0;
        while (i < panjang_kata && teks[i] != '\0' &&
               tolower((unsigned char)teks[i]) == tolower((unsigned char)kata[i])) {
            i++;
        }
        if (i == panjang_kata) {
            return 1;
        }
    }
    return 0;
}
