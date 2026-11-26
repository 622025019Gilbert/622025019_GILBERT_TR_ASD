#include <stdio.h>
#include "app.h"
#include "auth.h"
#include "menu.h"
#include "utils.h"

int main(void) {
    TabelAkun tabel_akun;
    char username_aktif[MAX_USERNAME + 1];

    aktifkan_warna_ansi();
    tabel_init(&tabel_akun);
    auth_muat_akun(&tabel_akun);

    while (menu_awal(&tabel_akun, username_aktif, sizeof(username_aktif))) {
        AppState app;

        tampilkan_loading();
        if (!app_init(&app, &tabel_akun, username_aktif)) {
            printf("Gagal menyiapkan data aplikasi.\n");
            continue;
        }
        menu_utama(&app);
        app_simpan(&app);
        app_bebas(&app);
    }

    tabel_bebas(&tabel_akun);
    return 0;
}
