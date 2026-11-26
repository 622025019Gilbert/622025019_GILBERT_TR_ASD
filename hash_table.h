#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include "config.h"

typedef uint32_t NilaiHash;

typedef struct AkunNode {
    char username[MAX_USERNAME + 1];
    NilaiHash hash_password;
    struct AkunNode *berikutnya;
} AkunNode;

/* Hash table dengan separate chaining; kunci = username (unik). */
typedef struct {
    AkunNode *bucket[HASH_SIZE];
    int jumlah;
} TabelAkun;

typedef enum {
    SISIP_OK,
    SISIP_DUPLIKAT,
    SISIP_GAGAL_MEMORI
} HasilSisip;

NilaiHash hitung_hash_djb2(const char *teks);
int       tabel_indeks(const char *kunci);

void       tabel_init(TabelAkun *t);
void       tabel_bebas(TabelAkun *t);
HasilSisip tabel_sisip(TabelAkun *t, const char *username, NilaiHash hash_password);
AkunNode  *tabel_cari(const TabelAkun *t, const char *username, int *langkah);
void       tabel_tampilkan(const TabelAkun *t);

#endif
