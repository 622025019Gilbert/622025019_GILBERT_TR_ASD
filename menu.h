#ifndef MENU_H
#define MENU_H

#include <stddef.h>
#include "app.h"
#include "hash_table.h"

/* Menu sebelum login. Mengembalikan 1 jika login berhasil, 0 jika pengguna memilih keluar. */
int  menu_awal(TabelAkun *tabel_akun, char *username_aktif, size_t ukuran);

/* Menu setelah login (kembali ke pemanggil = logout). */
void menu_utama(AppState *app);
void menu_kelola_data(AppState *app);
void menu_sorting(AppState *app);
void menu_pencarian(AppState *app);
void menu_antrean(AppState *app);
void menu_bst(AppState *app);
void menu_graph(AppState *app);
void menu_hash_table(AppState *app);

/* Dipakai bersama oleh beberapa menu */
void proses_tambah_mahasiswa(AppState *app);
void proses_hapus_mahasiswa(AppState *app);
int  minta_id_terdaftar(const AppState *app, const char *prompt, int *id);
void simpan_dengan_pesan(const AppState *app);

#endif
