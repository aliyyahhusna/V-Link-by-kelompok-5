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