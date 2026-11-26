#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "auth.h"
#include "utils.h"

static void tampilkan_hasil_register(HasilRegister hasil) {
    switch (hasil) {
        case REGISTER_BERHASIL:
            printf("Register berhasil! Silakan login.\n");
            break;
        case REGISTER_USERNAME_PENDEK:
            printf("Username terlalu pendek (minimal %d karakter).\n", MIN_USERNAME);
            break;
        case REGISTER_USERNAME_KARAKTER_TIDAK_VALID:
            printf("Username hanya boleh berisi huruf, angka, dan underscore.\n");
            break;
        case REGISTER_USERNAME_TERPAKAI:
            printf("Username sudah digunakan. Pilih username lain.\n");
            break;
        case REGISTER_PASSWORD_PENDEK:
            printf("Password terlalu pendek (minimal %d karakter).\n", MIN_PASSWORD);
            break;
        case REGISTER_GAGAL_MEMORI:
            printf("Register gagal: memori tidak cukup.\n");
            break;
        case REGISTER_GAGAL_FILE:
            printf("Akun terdaftar sementara, tetapi gagal disimpan ke '%s'.\n", FILE_AKUN);
            break;
    }
}

static void proses_register(TabelAkun *tabel_akun) {
    char username[MAX_USERNAME + 1];
    char password[MAX_PASSWORD + 1];
    char konfirmasi[MAX_PASSWORD + 1];

    bersihkan_layar();
    cetak_judul("REGISTER AKUN BARU");
    baca_teks_aman("Username          : ", username, sizeof(username));
    baca_teks_aman("Password          : ", password, sizeof(password));
    baca_teks_aman("Konfirmasi        : ", konfirmasi, sizeof(konfirmasi));

    if (strcmp(password, konfirmasi) != 0) {
        printf("Konfirmasi password tidak sama. Register dibatalkan.\n");
    } else {
        tampilkan_hasil_register(auth_register(tabel_akun, username, password));
    }
    tekan_enter_untuk_lanjut();
}

static int proses_login(const TabelAkun *tabel_akun, char *username_aktif, size_t ukuran) {
    char username[MAX_USERNAME + 1];
    char password[MAX_PASSWORD + 1];

    bersihkan_layar();
    cetak_judul("LOGIN");
    for (int percobaan = 1; percobaan <= MAX_PERCOBAAN_LOGIN; percobaan++) {
        baca_teks_aman("Username : ", username, sizeof(username));
        baca_teks_aman("Password : ", password, sizeof(password));

        if (auth_login(tabel_akun, username, password)) {
            snprintf(username_aktif, ukuran, "%s", username);
            printf("\nLogin berhasil! Selamat datang, %s.\n", username);
            jeda_ms(JEDA_SINGKAT_MS);
            return 1;
        }
        printf("Login gagal! Username atau password salah (percobaan %d/%d).\n\n",
               percobaan, MAX_PERCOBAAN_LOGIN);
    }
    printf("Terlalu banyak percobaan gagal. Kembali ke menu awal.\n");
    tekan_enter_untuk_lanjut();
    return 0;
}

int menu_awal(TabelAkun *tabel_akun, char *username_aktif, size_t ukuran) {
    int pilihan;

    do {
        bersihkan_layar();
        cetak_judul("SISTEM MANAJEMEN NILAI MAHASISWA - ASD 2026");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Keluar\n");
        pilihan = baca_int_aman("Pilih menu [1-3]: ", 1, 3);

        if (pilihan == 1 && proses_login(tabel_akun, username_aktif, ukuran)) {
            return 1;
        }
        if (pilihan == 2) {
            proses_register(tabel_akun);
        }
    } while (pilihan != 3);

    bersihkan_layar();
    printf("Terima kasih. Sampai jumpa!\n");
    return 0;
}
