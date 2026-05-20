#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//buat mendefene konstanta
#define MAX_SKILL_PER_PERSON 3
#define STR_LEN 50
#define INITIAL_CAPACITY 5








// SORTING & POINTERS dibuat oleh Farras

// Selection Sort, mengurutkan relawan berdasarkan jam terbang terbesar
// Menggunakan pointer arithmetic (daftar + j) untuk akses memori yang efisien
void urutkan_relawan_prioritas(struct Relawan *daftar, int jml) {
    for (int i = 0; i < jml - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < jml; j++) {
            if ((daftar + j)->jamTerlibat > (daftar + max_idx)->jamTerlibat) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            struct Relawan temp = *(daftar + i);
            *(daftar + i) = *(daftar + max_idx);
            *(daftar + max_idx) = temp;
        }
    }
}


//FUNGSI INPUT DATA RELAWAN & PROYEK dibuat oleh Farras

// Fungsi menambah relawan dengan realloc
struct Relawan* tambah_relawan(struct Relawan *daftar, int *jumlah, int *kapasitas) {
    if (*jumlah >= *kapasitas) {
        *kapasitas += 5;
        struct Relawan *temp = realloc(daftar, (*kapasitas) * sizeof(struct Relawan));
        if (temp == NULL) {
            printf("[ERROR] Alokasi memori gagal!\n");
            return daftar;
        }
        daftar = temp; // Update pointer utama ke lokasi memori baru
    }

    // Pointer ke slot kosong berikutnya
    struct Relawan *r = daftar + (*jumlah);
    int idInput;
    
    printf("\n--- REGISTRASI RELAWAN BARU ---\n");
    printf("Masukkan ID unik Relawan (Angka): ");
    // input harus angka
    while (scanf("%d", &idInput) != 1) {
        printf("[ERROR] Input wajib berupa angka biasa! Coba lagi: ");
        bersihkan_buffer();
    }
    bersihkan_buffer();

    // Validasi id unik (tidak boleh sama)
    for (int i = 0; i < *jumlah; i++) {
        if ((daftar + i)->idRelawan == idInput) {
            printf("[AKSES DITOLAK] Relawan dengan ID %d sudah terdaftar!\n", idInput);
            return daftar;
        }
    }
    r->idRelawan = idInput;

    printf("Masukkan Nama Lengkap: ");
    fgets(r->nama, STR_LEN, stdin);
    clean_newline(r->nama);

    printf("Masukkan Jam Terbang Sebelumnya (Isi 0 jika baru mulai): ");
    while (scanf("%d", &(r->jamTerlibat)) != 1) {
        printf("[ERROR] Input salah! Masukkan angka untuk jam terbang: ");
        bersihkan_buffer();
    }

    printf("Berapa keahlian yang ingin didaftarkan? (1-%d): ", MAX_SKILL_PER_PERSON);
    while (scanf("%d", &(r->jumlahSkill)) != 1 || r->jumlahSkill < 1 || r->jumlahSkill > MAX_SKILL_PER_PERSON) {
        printf("[ERROR] Input tidak valid! Masukkan rentang (1-%d): ", MAX_SKILL_PER_PERSON);
        bersihkan_buffer();
    }

    //Input skill dan level dibuat oleh Farras
    //Bagian ini akan meminta user pilih skill -> level -> verifikasi sertifikat
    for (int i = 0; i < r->jumlahSkill; i++) {
        printf("\nKeahlian ke-%d:\n", i + 1);
        printf("1. Teknologi/Coding\n2. Kesehatan/Medis\n3. Pendidikan Sosial\n4. Lingkungan Hidup\nPilih Kategori (1-4): ");
        int choice;
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
            printf("[ERROR] Pilih kategori valid (1-4): ");
            bersihkan_buffer();
        }
        r->skill[i] = (enum KeahlianSkill)choice;

        printf("\n0. Uncertified / Otodidak\n1. Beginner\n2. Intermediate\n3. Expert\n");
        printf("Pilih Tingkat Kemampuan (0-3): ");
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 3) {
            printf("[ERROR] Pilih tingkat kemampuan valid (0-3): ");
            bersihkan_buffer();
        }
        r->levelSkill[i] = (enum TingkatSkill)choice;
        bersihkan_buffer(); 

        if (r->levelSkill[i] == UNCERTIFIED && r->jamTerlibat == 0) {
            printf("\n[AKSES DITOLAK] Anda mendaftar sebagai Uncertified tanpa Jam Terbang.\n");
            printf("Minimal harus memiliki sertifikat dasar atau pengalaman lapangan!\n");
            printf("Registrasi relawan dibatalkan.\n");
            return daftar; 
        }

        if (r->levelSkill[i] > UNCERTIFIED) {
            printf("\n--- VERIFIKASI SERTIFIKAT UNTUK LEVEL %s ---\n", get_level_name(r->levelSkill[i]));
            printf("Pilih Platform Penerbit:\n");
            printf("1. Cisco (Cisco ID)\n");
            printf("2. Coursera\n");
            printf("3. DisasterReady\n");
            printf("4. UNCC:Learn (PBB)\n");
            printf("5. BNSP/LSP (No. Sertifikat Atas)\n");
            printf("6. Ijazah Kuliah (PIN Dikti)\n");
            printf("7. Lainnya\n"); 
            printf("Pilihan (1-7): ");
            while (scanf("%d", &(r->sertif[i].platformOpsi)) != 1 || r->sertif[i].platformOpsi < 1 || r->sertif[i].platformOpsi > 7) {
                printf("[ERROR] Masukkan opsi yang benar (1-7): ");
                bersihkan_buffer();
            }
            bersihkan_buffer(); 

            if (r->sertif[i].platformOpsi == 7) { 
                printf("Masukkan Nama Lembaga Independen: ");
                fgets(r->sertif[i].platformNama, STR_LEN, stdin);
                clean_newline(r->sertif[i].platformNama);
            }

            char input_cert[100] = {0};
            printf("Masukkan String/Nomor ID/PIN: ");
            fgets(input_cert, sizeof(input_cert), stdin);
            clean_newline(input_cert);

            int valid = 0;
            switch (r->sertif[i].platformOpsi) {
                case 1: strcpy(r->sertif[i].platformNama, "Cisco"); valid = validate_cisco_id(input_cert); break;
                case 2: strcpy(r->sertif[i].platformNama, "Coursera"); valid = validate_coursera(input_cert); break;
                case 3: strcpy(r->sertif[i].platformNama, "DisasterReady"); valid = validate_disaster_ready(input_cert); break;
                case 4: strcpy(r->sertif[i].platformNama, "UNCC:Learn"); valid = validate_uncc(input_cert); break;
                case 5: strcpy(r->sertif[i].platformNama, "BNSP Sertifikat"); valid = validate_bnsp_sertifikat(input_cert); break;
                case 6: strcpy(r->sertif[i].platformNama, "Ijazah Kuliah"); valid = validate_ijazah(input_cert); break; 
                default: valid = validate_generic_cert(input_cert); break;
            }

            if (valid) {
                printf("-> [VALID] Dokumen %s diakui oleh sistem!\n", r->sertif[i].platformNama);
                r->sertif[i].isVerified = 1;
                // Opsi 5 (BNSP Sertifikat) dan Opsi 6 (Ijazah) menggunakan tipe data angka (nomorLisensi) di union
                if (r->sertif[i].platformOpsi >= 5 && r->sertif[i].platformOpsi <= 6) {
                    r->sertif[i].bukti.nomorLisensi = atoll(input_cert); 
                } else {
                    strcpy(r->sertif[i].bukti.namaSertifikat, input_cert);
                }
            } else {
                printf("-> [FRAUD DETECTED] Dokumen Invalid. Level dijatuhkan ke Uncertified!\n");
                r->sertif[i].isVerified = 0;
                r->levelSkill[i] = UNCERTIFIED; 
                
                if (r->jamTerlibat == 0) {
                    printf("\n[AKSES DITOLAK] Dokumen invalid dan Jam Terbang Anda 0.\n");
                    printf("Registrasi relawan dibatalkan.\n");
                    return daftar;
                }
            }
        } else {
            r->sertif[i].isVerified = 0;
            r->sertif[i].platformOpsi = 0;
            strcpy(r->sertif[i].platformNama, "Tanpa Sertifikat");
        }
    }

    r->isBusy = 0; // Set status awal (tersedia)
    printf("\n[SUKSES] Akun relawan '%s' berhasil disimpan!\n", r->nama);
    (*jumlah)++; //increment jumlah total relawan
    return daftar;
}

// Fungsi menambah proyek dibuat oleh Farras
struct Proyek* tambah_proyek(struct Proyek *daftar, int *jumlah, int *kapasitas) {
    if (*jumlah >= *kapasitas) {
        *kapasitas += 5;
        struct Proyek *temp = realloc(daftar, (*kapasitas) * sizeof(struct Proyek));
        if (temp == NULL) {
            printf("[ERROR] Alokasi memori gagal!\n");
            return daftar;
        }
        daftar = temp;
    }

    struct Proyek *p = daftar + (*jumlah);
    int idInput;

    //input ID, Nama, Skill dibutuhkan, Level Minimal, Kuota
    printf("\n--- INPUT PROYEK SOSIAL BARU (ADMIN) ---\n");
    printf("Masukkan ID unik Proyek (Angka): ");
    while (scanf("%d", &idInput) != 1) {
        printf("[ERROR] Input wajib berupa angka biasa! Coba lagi: ");
        bersihkan_buffer();
    }
    bersihkan_buffer();

    for (int i = 0; i < *jumlah; i++) {
        if ((daftar + i)->idProyek == idInput) {
            printf("[AKSES DITOLAK] Proyek dengan ID %d sudah terdaftar!\n", idInput);
            return daftar;
        }
    }
    p->idProyek = idInput;

    printf("Masukkan Nama Proyek Sosial: ");
    fgets(p->namaProyek, STR_LEN, stdin);
    clean_newline(p->namaProyek);

    printf("1. Teknologi/Coding\n2. Kesehatan/Medis\n3. Pendidikan Sosial\n4. Lingkungan Hidup\nPilih Kategori Kebutuhan Skill (1-4): ");
    int choice;
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        printf("[ERROR] Pilih kategori valid (1-4): ");
        bersihkan_buffer();
    }
    p->skillDibutuhkan = (enum KeahlianSkill)choice;

    printf("\n0. Uncertified / Otodidak\n1. Beginner\n2. Intermediate\n3. Expert\n");
    printf("Pilih Standar Level Minimum (0-3): ");
    while (scanf("%d", &choice) != 1 || choice < 0 || choice > 3) {
        printf("[ERROR] Pilih level valid (0-3): ");
        bersihkan_buffer();
    }
    p->levelMinimal = (enum TingkatSkill)choice;

    printf("Masukkan Kuota Maksimal Relawan (Angka): ");
    while (scanf("%d", &(p->kuotaMaksimal)) != 1 || p->kuotaMaksimal < 1) {
        printf("[ERROR] Kuota tidak valid! Masukkan kembali: ");
        bersihkan_buffer();
    }
    bersihkan_buffer();

    p->kuotaTerisi = 0; 
    printf("\n[SUKSES] Proyek '%s' berhasil dipublikasikan ke sistem Capaian SDG 17!\n", p->namaProyek);
    (*jumlah)++;
    return daftar;
}


int main() {
    int kapasitasRelawan = INITIAL_CAPACITY;
    int jumlahRelawan = 0;
    struct Relawan *databaseRelawan = malloc(kapasitasRelawan * sizeof(struct Relawan));
    int pilihanMenu;
    int kapasitasProyek = INITIAL_CAPACITY;
    int totalProyek = 3;
    struct Proyek *databaseProyek = malloc(kapasitasProyek * sizeof(struct Proyek));
    
    return 0;
}
