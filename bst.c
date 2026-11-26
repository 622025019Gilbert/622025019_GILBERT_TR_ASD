#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

static BSTNode *buat_node(const Mahasiswa *m) {
    BSTNode *baru = (BSTNode *)malloc(sizeof(BSTNode));

    if (baru != NULL) {
        baru->data = *m;
        baru->kiri = NULL;
        baru->kanan = NULL;
    }
    return baru;
}

static void cetak_node(const BSTNode *node) {
    printf("[%d:%s] ", node->data.id, node->data.nama);
}

BSTNode *bst_sisip(BSTNode *akar, const Mahasiswa *m) {
    if (akar == NULL) {
        return buat_node(m);
    }
    if (m->id < akar->data.id) {
        akar->kiri = bst_sisip(akar->kiri, m);
    } else if (m->id > akar->data.id) {
        akar->kanan = bst_sisip(akar->kanan, m);
    }
    return akar;
}

BSTNode *bst_cari(BSTNode *akar, int id) {
    if (akar == NULL || akar->data.id == id) {
        return akar;
    }
    return (id < akar->data.id) ? bst_cari(akar->kiri, id) : bst_cari(akar->kanan, id);
}

static BSTNode *bst_node_terkecil(BSTNode *akar) {
    while (akar != NULL && akar->kiri != NULL) {
        akar = akar->kiri;
    }
    return akar;
}

BSTNode *bst_hapus(BSTNode *akar, int id) {
    if (akar == NULL) {
        return NULL;
    }
    if (id < akar->data.id) {
        akar->kiri = bst_hapus(akar->kiri, id);
    } else if (id > akar->data.id) {
        akar->kanan = bst_hapus(akar->kanan, id);
    } else if (akar->kiri == NULL || akar->kanan == NULL) {
        BSTNode *pengganti = (akar->kiri != NULL) ? akar->kiri : akar->kanan;
        free(akar);
        return pengganti;
    } else {
        BSTNode *suksesor = bst_node_terkecil(akar->kanan);
        akar->data = suksesor->data;
        akar->kanan = bst_hapus(akar->kanan, suksesor->data.id);
    }
    return akar;
}

BSTNode *bst_bangun(const KoleksiMahasiswa *k) {
    BSTNode *akar = NULL;

    for (int i = 0; i < k->jumlah; i++) {
        akar = bst_sisip(akar, &k->data[i]);
    }
    return akar;
}

void bst_bebas(BSTNode *akar) {
    if (akar == NULL) {
        return;
    }
    bst_bebas(akar->kiri);
    bst_bebas(akar->kanan);
    free(akar);
}

int bst_hitung_node(const BSTNode *akar) {
    if (akar == NULL) {
        return 0;
    }
    return 1 + bst_hitung_node(akar->kiri) + bst_hitung_node(akar->kanan);
}

int bst_tinggi(const BSTNode *akar) {
    int tinggi_kiri;
    int tinggi_kanan;

    if (akar == NULL) {
        return 0;
    }
    tinggi_kiri = bst_tinggi(akar->kiri);
    tinggi_kanan = bst_tinggi(akar->kanan);
    return 1 + (tinggi_kiri > tinggi_kanan ? tinggi_kiri : tinggi_kanan);
}

void bst_preorder(const BSTNode *akar) {
    if (akar == NULL) return;
    cetak_node(akar);
    bst_preorder(akar->kiri);
    bst_preorder(akar->kanan);
}

void bst_inorder(const BSTNode *akar) {
    if (akar == NULL) return;
    bst_inorder(akar->kiri);
    cetak_node(akar);
    bst_inorder(akar->kanan);
}

void bst_postorder(const BSTNode *akar) {
    if (akar == NULL) return;
    bst_postorder(akar->kiri);
    bst_postorder(akar->kanan);
    cetak_node(akar);
}
