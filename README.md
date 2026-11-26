# Sistem Manajemen Nilai Mahasiswa (TR ASD 2026)

- **Nama** : (isi nama)
- **NIM**  : (isi NIM)

## Deskripsi
Aplikasi konsol berbahasa C untuk mengelola data nilai mahasiswa. Pengguna harus
register dan login terlebih dahulu. Seluruh materi Topik 1-8 dipakai sebagai bagian
dari sistem, bukan menu demo terpisah.

## Fitur
- Register, login (maks. 3 percobaan), dan logout. Akun disimpan di `data/akun.txt`.
- Kelola data mahasiswa (tampil, tambah, ubah skor, hapus, statistik nilai) dan fitur **Undo**.
- Pengurutan data berdasarkan ID/Nama/Skor, ascending/descending, dengan 5 algoritma.
- Pencarian nama (sequential) dan ID (binary search).
- Antrean pendaftaran calon mahasiswa baru.
- Indeks pohon BST berdasarkan ID (insert, search, delete, traversal).
- Graph relasi teman belajar dengan BFS dan DFS.
- Hash table akun untuk register/login dan pencarian username.

## Peran Setiap Topik dalam Sistem
| Topik | Penerapan |
|---|---|
| 1. Refreshing C | Program berbasis menu, dipecah menjadi fungsi; semua input divalidasi (`baca_int_aman`, `baca_teks_aman`) |
| 2. Array, Pointer, File | `struct Mahasiswa` pada array dinamis (`malloc/realloc/free`); pass-by-reference; data di `data/mahasiswa.txt` |
| 3. Sorting | Bubble, Selection, Insertion, Quick, Merge Sort tulisan sendiri; asc/desc; hasil dapat diterapkan ke data utama |
| 4. Stack & Queue | Stack = riwayat aksi untuk Undo; Queue = antrean pendaftaran mahasiswa baru |
| 5. Searching | Sequential search (nama), binary search (ID, pada salinan yang sudah diurutkan) |
| 6. Hash Table | Tabel akun: fungsi hash djb2, separate chaining; dipakai saat register dan login |
| 7. Tree | BST berdasarkan ID; selalu sinkron dengan data utama |
| 8. Graph | Relasi teman belajar (`data/graph.txt`); BFS = jangkauan terdekat, DFS = rantai relasi |

## Struktur Folder
```
include/   header (.h)
src/       sumber (.c), termasuk main.c
data/      akun.txt, mahasiswa.txt, graph.txt
docs/      laporan TR (PDF)
```

## Cara Kompilasi dan Menjalankan
Jalankan dari **root repositori** (agar folder `data/` terbaca):
```
gcc src/*.c -Iinclude -o app
./app          # Windows: app.exe
```

## Catatan
- Password disimpan sebagai hash djb2 (cukup untuk kebutuhan tugas, bukan hash kriptografis).
- Undo hapus mahasiswa memulihkan data, tetapi relasi graph yang ikut terhapus tidak dipulihkan.
- Antrean pendaftaran bersifat sementara (tidak disimpan ke file).
