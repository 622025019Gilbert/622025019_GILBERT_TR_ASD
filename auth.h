#ifndef AUTH_H
#define AUTH_H

#include "hash_table.h"

typedef enum {
    REGISTER_BERHASIL,
    REGISTER_USERNAME_PENDEK,
    REGISTER_USERNAME_KARAKTER_TIDAK_VALID,
    REGISTER_USERNAME_TERPAKAI,
    REGISTER_PASSWORD_PENDEK,
    REGISTER_GAGAL_MEMORI,
    REGISTER_GAGAL_FILE
} HasilRegister;

int           auth_muat_akun(TabelAkun *t);
HasilRegister auth_register(TabelAkun *t, const char *username, const char *password);
int           auth_login(const TabelAkun *t, const char *username, const char *password);

#endif
