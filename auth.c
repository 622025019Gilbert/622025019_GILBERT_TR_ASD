#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "auth.h"

/* 30 = MAX_USERNAME */
#define FORMAT_BARIS_AKUN "%30[^|]|%lu"

static int username_hanya_alfanumerik(const char *username) {
    for (; *username != '\0'; username++) {
        if (!isalnum((unsigned char)*username) && *username != '_') {
            return 0;
        }
    }
    return 1;
}

static int tambah_ke_file(const char *username, NilaiHash hash_password) {
    FILE *berkas = fopen(FILE_AKUN, "a");

    if (berkas == NULL) {
        return 0;
    }
    fprintf(berkas, "%s|%lu\n", username, (unsigned long)hash_password);
    fclose(berkas);
    return 1;
}

int auth_muat_akun(TabelAkun *t) {
    FILE *berkas = fopen(FILE_AKUN, "r");
    char baris[MAX_BARIS_FILE];

    if (berkas == NULL) {
        return 0;
    }
    while (fgets(baris, sizeof(baris), berkas) != NULL) {
        char username[MAX_USERNAME + 1];
        unsigned long hash_baca;
        if (sscanf(baris, FORMAT_BARIS_AKUN, username, &hash_baca) == 2) {
            tabel_sisip(t, username, (NilaiHash)hash_baca);
        }
    }
    fclose(berkas);
    return 1;
}

HasilRegister auth_register(TabelAkun *t, const char *username, const char *password) {
    NilaiHash hash_password;

    if (strlen(username) < MIN_USERNAME) {
        return REGISTER_USERNAME_PENDEK;
    }
    if (!username_hanya_alfanumerik(username)) {
        return REGISTER_USERNAME_KARAKTER_TIDAK_VALID;
    }
    if (tabel_cari(t, username, NULL) != NULL) {
        return REGISTER_USERNAME_TERPAKAI;
    }
    if (strlen(password) < MIN_PASSWORD) {
        return REGISTER_PASSWORD_PENDEK;
    }

    hash_password = hitung_hash_djb2(password);
    if (tabel_sisip(t, username, hash_password) != SISIP_OK) {
        return REGISTER_GAGAL_MEMORI;
    }
    if (!tambah_ke_file(username, hash_password)) {
        return REGISTER_GAGAL_FILE;
    }
    return REGISTER_BERHASIL;
}

int auth_login(const TabelAkun *t, const char *username, const char *password) {
    AkunNode *akun = tabel_cari(t, username, NULL);

    if (akun == NULL) {
        return 0;
    }
    return akun->hash_password == hitung_hash_djb2(password);
}
