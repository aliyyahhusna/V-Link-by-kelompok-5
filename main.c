#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//buat mendefene konstanta
#define MAX_SKILL_PER_PERSON 3
#define STR_LEN 50
#define INITIAL_CAPACITY 5

//DATA STRUCTURES & ENUMS  dibuat oleh Aliyyah

enum KeahlianSkill { TECH = 1, HEALTH, EDUCATION, ENVIRONMENT };
enum TingkatSkill { UNCERTIFIED = 0, BEGINNER = 1, INTERMEDIATE = 2, EXPERT = 3 };

union BuktiSertifikat {
    long long nomorLisensi;     
    char namaSertifikat[STR_LEN]; 
};

struct Sertifikat {
    int platformOpsi;
    char platformNama[STR_LEN];
    union BuktiSertifikat bukti;
    int isVerified; 
};

struct Relawan {
    int idRelawan; 
    char nama[STR_LEN];
    enum KeahlianSkill skill[MAX_SKILL_PER_PERSON];
    enum TingkatSkill levelSkill[MAX_SKILL_PER_PERSON];
    struct Sertifikat sertif[MAX_SKILL_PER_PERSON];
    int jumlahSkill;
    int jamTerlibat; 
    int isBusy;      
};

struct Proyek {
    int idProyek;  
    char namaProyek[STR_LEN];
    enum KeahlianSkill skillDibutuhkan;
    enum TingkatSkill levelMinimal;
    int kuotaMaksimal;
    int kuotaTerisi;
};

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


//MATCHMAKING dan DASHBOARD

//inti dari program, memasangkan relawan dengan proyek yang sesuai dibuat oleh Aliyyah
void jalankan_matchmaking(struct Proyek *daftarProyek, int jmlProyek, struct Relawan *daftarRelawan, int jmlRelawan) {
    printf("\n==================================================\n");
    printf("     SYSTEM ADM: PROSES MATCHMAKING OTOMATIS\n");
    printf("==================================================\n");

    urutkan_relawan_prioritas(daftarRelawan, jmlRelawan);
    printf("[INFO] Database relawan telah diurutkan berdasarkan prioritas Jam Terbang tertinggi.\n\n");

// Menampilkan dashboard status dibuat oleh Aliyyah
void tampilkan_dashboard(struct Proyek *daftarProyek, int jmlProyek, struct Relawan *daftarRelawan, int jmlRelawan) {
    printf("\n==================================================\n");
    printf("          DASHBOARD KEMITRAAN CAPAIAN SDG 17       \n");
    printf("==================================================\n");
    
    printf("\n[STATUS PROYEK SOSIAL MITRA]\n");
    printf("%-7s | %-25s | %-9s | %-12s\n", "ID", "Nama Proyek", "Kuota", "Status");
    printf("------------------------------------------------------------\n");
    for (int p = 0; p < jmlProyek; p++) {
        printf("%-7d | %-25s | %d/%-7d | %s\n", 
                daftarProyek[p].idProyek, daftarProyek[p].namaProyek, 
                daftarProyek[p].kuotaTerisi, daftarProyek[p].kuotaMaksimal,
                (daftarProyek[p].kuotaTerisi == daftarProyek[p].kuotaMaksimal) ? "SIAP JALAN" : "BUTUH MITRA");
    }

    printf("\n[DATABASE PORTOFOLIO RELAWAN]\n");
    printf("%-15s | %-18s | %-15s | %-15s\n", "ID", "Nama Relawan", "Jam Terbang", "Status Kerja");
    printf("---------------------------------------------------------------------------\n");
    for (int r = 0; r < jmlRelawan; r++) {
        struct Relawan *v = daftarRelawan + r;
        printf("%-15d | %-18s | %-11d jam | %s\n", 
                v->idRelawan, v->nama, v->jamTerlibat, 
                (v->isBusy == 1) ? "AKTIF LAPANGAN" : "STANDBY (FREE)");
    }
    printf("==================================================\n");
}

for (int p = 0; p < jmlProyek; p++) {
        struct Proyek *prj = &daftarProyek[p];
        
        if (prj->kuotaTerisi >= prj->kuotaMaksimal) continue;

        printf("Memproses Proyek [%d] '%s' (Butuh: %s, Min Level: %s)\n", 
                prj->idProyek, prj->namaProyek, get_skill_name(prj->skillDibutuhkan), get_level_name(prj->levelMinimal));

        for (int r = 0; r < jmlRelawan; r++) {
            struct Relawan *vln = daftarRelawan + r; 
            if (vln->isBusy == 1) continue;

            for (int s = 0; s < vln->jumlahSkill; s++) {
                if (vln->skill[s] == prj->skillDibutuhkan && vln->levelSkill[s] >= prj->levelMinimal) {
                    vln->isBusy = 1;
                    vln->jamTerlibat += 24; 
                    prj->kuotaTerisi++;
                    printf("  -> MATCH FOUND: '%s' (ID:%d) | Jam Terbang Awal: %d | Skill: [%s]\n", 
                            vln->nama, vln->idRelawan, vln->jamTerlibat - 24, get_level_name(vln->levelSkill[s]));
                    break; 
                }
            }
            if (prj->kuotaTerisi >= prj->kuotaMaksimal) break;
        }
        if (prj->kuotaTerisi < prj->kuotaMaksimal) {
            printf("  -> Sisa kuota belum terpenuhi: %d orang\n", prj->kuotaMaksimal - prj->kuotaTerisi);
        }
        printf("--------------------------------------------------\n");
    }

int main() {
    
    int kapasitasRelawan = INITIAL_CAPACITY;
    int jumlahRelawan = 0;
    // Alokasi memori awal menggunakan malloc
    struct Relawan *databaseRelawan = malloc(kapasitasRelawan * sizeof(struct Relawan));
    int pilihanMenu;
    int kapasitasProyek = INITIAL_CAPACITY;
    int totalProyek = 3;
    // Alokasi memori awal menggunakan malloc
    struct Proyek *databaseProyek = malloc(kapasitasProyek * sizeof(struct Proyek));

    if (databaseRelawan == NULL || databaseProyek == NULL) return 1;

    // Inisialisasi data proyek sosial dibuat oleh Aliyyah
    databaseProyek[0] = (struct Proyek){101, "Digitalisasi UMKM Desa", TECH, INTERMEDIATE, 1, 0};
    databaseProyek[1] = (struct Proyek){102, "Edukasi Sanitasi Sehat", HEALTH, INTERMEDIATE, 1, 0};
    databaseProyek[2] = (struct Proyek){103, "Coding For Kids Camp", TECH, BEGINNER, 2, 0};

    
    //Loop untuk pilihan menu dibuat oleh Aliyyah
    do {
        printf("\n=====================================================\n");
        printf("                  V-LINK (SDG 17)\n");
        printf("=====================================================\n");
        printf("1. MENU USER  : Registrasi Relawan & Sertifikasi\n");
        printf("2. MENU ADMIN : Buat/Tambah Proyek Sosial Baru\n"); 
        printf("3. MENU ADMIN : Jalankan Sistem Matching Otomatis\n");
        printf("4. MENU ADMIN : Tampilkan Dashboard Rekapitulasi\n");
        printf("5. KELUAR PROGRAM\n");
        printf("Masukkan pilihan Anda (1-5): ");
        
        if (scanf("%d", &pilihanMenu) != 1) {
            printf("Tolong masukkan angka menu yang valid!\n");
            bersihkan_buffer();
            continue;
        }
        bersihkan_buffer(); 

        switch(pilihanMenu) {
            case 1: databaseRelawan = tambah_relawan(databaseRelawan, &jumlahRelawan, &kapasitasRelawan); break;
            case 2: databaseProyek = tambah_proyek(databaseProyek, &totalProyek, &kapasitasProyek); break; 
            case 3:
                if (jumlahRelawan == 0) printf("\n Database relawan kosong!\n");
                else jalankan_matchmaking(databaseProyek, totalProyek, databaseRelawan, jumlahRelawan);
                break;
            case 4: tampilkan_dashboard(databaseProyek, totalProyek, databaseRelawan, jumlahRelawan); break;
            case 5: printf("\nTerima kasih telah berkontribusi dalam Kemitraan SDG 17!\n"); break;
            default: printf("\nPilihan menu tidak tersedia!\n");
        }
    } while(pilihanMenu != 5);

    //membebaskan memori untuk mencegah memory leak dibuat oleh Aliyyah
    free(databaseRelawan);
    free(databaseProyek); 
    return 0;
}
