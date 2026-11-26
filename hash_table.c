#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

NilaiHash hitung_hash_djb2(const char *teks) {
    NilaiHash hasil = HASH_AWAL;

    for (; *teks != '\0'; teks++) {
        hasil = ((hasil << 5) + hasil) + (unsigned char)*teks;
    }
    return hasil;
}

int tabel_indeks(const char *kunci) {
    return (int)(hitung_hash_djb2(kunci) % HASH_SIZE);
}

void tabel_init(TabelAkun *t) {
    for (int i = 0; i < HASH_SIZE; i++) {
        t->bucket[i] = NULL;
    }
    t->jumlah = 0;
}

void tabel_bebas(TabelAkun *t) {
    for (int i = 0; i < HASH_SIZE; i++) {
        AkunNode *jalan = t->bucket[i];
        while (jalan != NULL) {
            AkunNode *hapus = jalan;
            jalan = jalan->berikutnya;
            free(hapus);
        }
        t->bucket[i] = NULL;
    }
    t->jumlah = 0;
}

AkunNode *tabel_cari(const TabelAkun *t, const char *username, int *langkah) {
    AkunNode *jalan = t->bucket[tabel_indeks(username)];
    int hitung = 0;

    while (jalan != NULL) {
        hitung++;
        if (strcmp(jalan->username, username) == 0) {
            break;
        }
        jalan = jalan->berikutnya;
    }
    if (langkah != NULL) {
        *langkah = hitung;
    }
    return jalan;
}

HasilSisip tabel_sisip(TabelAkun *t, const char *username, NilaiHash hash_password) {
    int slot;
    AkunNode *baru;

    if (tabel_cari(t, username, NULL) != NULL) {
        return SISIP_DUPLIKAT;
    }
    baru = (AkunNode *)malloc(sizeof(AkunNode));
    if (baru == NULL) {
        return SISIP_GAGAL_MEMORI;
    }
    strncpy(baru->username, username, MAX_USERNAME);
    baru->username[MAX_USERNAME] = '\0';
    baru->hash_password = hash_password;

    slot = tabel_indeks(username);
    baru->berikutnya = t->bucket[slot];
    t->bucket[slot] = baru;
    t->jumlah++;
    return SISIP_OK;
}

void tabel_tampilkan(const TabelAkun *t) {
    int tabrakan = 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        int panjang = 0;
        if (t->bucket[i] == NULL) {
            continue;
        }
        printf("Slot [%02d]: ", i);
        for (AkunNode *jalan = t->bucket[i]; jalan != NULL; jalan = jalan->berikutnya) {
            printf("%s%s", panjang > 0 ? " -> " : "", jalan->username);
            panjang++;
        }
        if (panjang > 1) {
            printf("   (collision: %d kunci)", panjang);
            tabrakan++;
        }
        printf("\n");
    }
    printf("\nTotal akun: %d | Ukuran tabel: %d | Slot dengan collision: %d\n",
           t->jumlah, HASH_SIZE, tabrakan);
}
