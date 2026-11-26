#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void bersihkan_layar(void);
void jeda_ms(int ms);
void aktifkan_warna_ansi(void);

void cetak_garis(char karakter);
void cetak_judul(const char *judul);
void tampilkan_loading(void);

/* Input aman: tidak crash, tidak infinite loop, menolak input kosong/salah. */
int  baca_int_aman(const char *prompt, int min_val, int max_val);
void baca_teks_aman(const char *prompt, char *dest, size_t ukuran);
void tekan_enter_untuk_lanjut(void);

int  mengandung_teks_abaikan_case(const char *teks, const char *kata);

#endif
