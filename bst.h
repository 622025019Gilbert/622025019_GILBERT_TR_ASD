#ifndef BST_H
#define BST_H

#include "mahasiswa.h"

typedef struct BSTNode {
    Mahasiswa data;
    struct BSTNode *kiri;
    struct BSTNode *kanan;
} BSTNode;

BSTNode *bst_sisip(BSTNode *akar, const Mahasiswa *m);
BSTNode *bst_cari(BSTNode *akar, int id);
BSTNode *bst_hapus(BSTNode *akar, int id);
BSTNode *bst_bangun(const KoleksiMahasiswa *k);
void     bst_bebas(BSTNode *akar);

int  bst_hitung_node(const BSTNode *akar);
int  bst_tinggi(const BSTNode *akar);

void bst_preorder(const BSTNode *akar);
void bst_inorder(const BSTNode *akar);
void bst_postorder(const BSTNode *akar);

#endif
