#include <stdio.h>
#include "menu.h"
#include "utils.h"

static void cari_node(const AppState *app) {
    int id = baca_int_aman("ID yang dicari: ", ID_MIN, ID_MAX);
    BSTNode *node = bst_cari(app->akar_bst, id);

    if (node == NULL) {
        printf("Mahasiswa dengan ID %d tidak ditemukan di BST.\n", id);
    } else {
        printf("Ditemukan di BST:\n");
        mahasiswa_cetak_baris(&node->data);
    }
}

static void tampilkan_traversal(const AppState *app) {
    if (app->akar_bst == NULL) {
        printf("BST kosong.\n");
        return;
    }
    printf("Jumlah node: %d | Tinggi pohon: %d\n\n", bst_hitung_node(app->akar_bst),
           bst_tinggi(app->akar_bst));
    printf("Pre-order  : ");
    bst_preorder(app->akar_bst);
    printf("\nIn-order   : ");
    bst_inorder(app->akar_bst);
    printf("  (terurut menurut ID)\nPost-order : ");
    bst_postorder(app->akar_bst);
    printf("\n");
}

void menu_bst(AppState *app) {
    int pilihan;

    do {
        bersihkan_layar();
        cetak_judul("INDEKS POHON (BINARY SEARCH TREE) BERDASARKAN ID");
        printf("1. Insert mahasiswa baru\n");
        printf("2. Search berdasarkan ID\n");
        printf("3. Delete mahasiswa berdasarkan ID\n");
        printf("4. Traversal (Pre-order, In-order, Post-order)\n");
        printf("0. Kembali\n");
        pilihan = baca_int_aman("Pilih operasi [0-4]: ", 0, 4);

        if (pilihan == 0) {
            break;
        }
        bersihkan_layar();
        switch (pilihan) {
            case 1: proses_tambah_mahasiswa(app); break;
            case 2: cari_node(app);               break;
            case 3: proses_hapus_mahasiswa(app);  break;
            case 4: tampilkan_traversal(app);     break;
        }
        tekan_enter_untuk_lanjut();
    } while (1);
}
