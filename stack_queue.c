#include <stdio.h>
#include <stdlib.h>
#include "stack_queue.h"

static const char *nama_aksi(JenisAksi jenis) {
    switch (jenis) {
        case AKSI_TAMBAH:    return "Tambah mahasiswa";
        case AKSI_HAPUS:     return "Hapus mahasiswa";
        case AKSI_UBAH_SKOR: return "Ubah skor";
    }
    return "?";
}

void stack_init(TumpukanUndo *s) {
    s->puncak = NULL;
    s->ukuran = 0;
}

int stack_kosong(const TumpukanUndo *s) {
    return s->puncak == NULL;
}

int stack_push(TumpukanUndo *s, const AksiUndo *aksi) {
    TumpukanNode *baru = (TumpukanNode *)malloc(sizeof(TumpukanNode));

    if (baru == NULL) {
        return 0;
    }
    baru->aksi = *aksi;
    baru->berikutnya = s->puncak;
    s->puncak = baru;
    s->ukuran++;
    return 1;
}

int stack_pop(TumpukanUndo *s, AksiUndo *aksi) {
    TumpukanNode *hapus = s->puncak;

    if (hapus == NULL) {
        return 0;
    }
    *aksi = hapus->aksi;
    s->puncak = hapus->berikutnya;
    free(hapus);
    s->ukuran--;
    return 1;
}

void stack_bebas(TumpukanUndo *s) {
    AksiUndo dibuang;
    while (stack_pop(s, &dibuang)) {
    }
}

void stack_tampilkan(const TumpukanUndo *s) {
    int nomor = 1;

    if (stack_kosong(s)) {
        printf("[ Riwayat kosong, belum ada aksi yang bisa di-undo ]\n");
        return;
    }
    printf("Riwayat aksi (paling atas = aksi terakhir, akan di-undo lebih dulu):\n");
    for (const TumpukanNode *jalan = s->puncak; jalan != NULL; jalan = jalan->berikutnya) {
        const AksiUndo *a = &jalan->aksi;
        printf("%2d. %-16s [%d] %s", nomor++, nama_aksi(a->jenis), a->data.id, a->data.nama);
        if (a->jenis == AKSI_UBAH_SKOR) {
            printf(" (skor %d -> %d)", a->skor_lama, a->data.skor);
        }
        printf("%s\n", (jalan == s->puncak) ? "   <-- TOP" : "");
    }
}

void queue_init(AntreanPendaftaran *q) {
    q->depan = NULL;
    q->belakang = NULL;
    q->ukuran = 0;
}

int queue_kosong(const AntreanPendaftaran *q) {
    return q->depan == NULL;
}

int queue_enqueue(AntreanPendaftaran *q, const CalonMahasiswa *calon) {
    AntreanNode *baru = (AntreanNode *)malloc(sizeof(AntreanNode));

    if (baru == NULL) {
        return 0;
    }
    baru->calon = *calon;
    baru->berikutnya = NULL;
    if (q->belakang == NULL) {
        q->depan = baru;
    } else {
        q->belakang->berikutnya = baru;
    }
    q->belakang = baru;
    q->ukuran++;
    return 1;
}

int queue_dequeue(AntreanPendaftaran *q, CalonMahasiswa *calon) {
    AntreanNode *hapus = q->depan;

    if (hapus == NULL) {
        return 0;
    }
    *calon = hapus->calon;
    q->depan = hapus->berikutnya;
    if (q->depan == NULL) {
        q->belakang = NULL;
    }
    free(hapus);
    q->ukuran--;
    return 1;
}

void queue_bebas(AntreanPendaftaran *q) {
    CalonMahasiswa dibuang;
    while (queue_dequeue(q, &dibuang)) {
    }
}

void queue_tampilkan(const AntreanPendaftaran *q) {
    int nomor = 1;

    if (queue_kosong(q)) {
        printf("[ Antrean kosong ]\n");
        return;
    }
    printf("Antrean pendaftaran (urutan dilayani dari atas):\n");
    for (const AntreanNode *jalan = q->depan; jalan != NULL; jalan = jalan->berikutnya) {
        printf("%2d. %s (skor awal %d)%s\n", nomor++, jalan->calon.nama, jalan->calon.skor,
               (jalan == q->depan) ? "   <-- FRONT" : "");
    }
}
