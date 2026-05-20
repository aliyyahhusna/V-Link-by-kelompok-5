#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//buat mendefene konstanta
#define MAX_SKILL_PER_PERSON 3
#define STR_LEN 50
#define INITIAL_CAPACITY 5


void bersihkan_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Validasi Cisco ID (Contoh: CSCO12345678)
int validate_cisco_id(const char *id) {
    char prefix[5] = {0};
    unsigned long numbers;
    if (sscanf(id, "%4s%8lu", prefix, &numbers) == 2) {
        if (strcmp(prefix, "CSCO") == 0 && strlen(id) == 12) return 1; 
    }
    return 0;
}

// Validasi Nomor Sertifikat coursera
int validate_coursera(const char *id) {
    int len = strlen(id);
    if (len < 11 || len > 13) return 0;
    for (int i = 0; i < len; i++) {
        if (!isalnum((unsigned char)id[i])) return 0;
    }
    return 1;
}

// Validasi Nomor Sertifikat DisasterReady
int validate_disaster_ready(const char *id) {
    if (strlen(id) != 9) return 0;
    if (strncmp(id, "DR-", 3) != 0) return 0;
    for (int i = 3; i < 9; i++) {
        if (!isdigit((unsigned char)id[i])) return 0;
    }
    return 1;
}

// Validasi Nomor Sertifikat uncc
int validate_uncc(const char *id) {
    int len = strlen(id);
    if (len < 9 || strncmp(id, "UNCC-", 5) != 0) return 0;
    for (int i = len - 4; i < len; i++) {
        if (!isdigit((unsigned char)id[i])) return 0;
    }
    return 1;
}

// Validasi Nomor Sertifikat BNSP
int validate_bnsp_sertifikat(const char *id) {
    unsigned long b1, b2, b3;
    unsigned int tahun;
    int matches = sscanf(id, "%lu %lu %lu %u", &b1, &b2, &b3, &tahun);
    return (matches == 4 && tahun >= 2000 && tahun <= 2030);
}

// Validasi Nomor Sertifikat lainnya (hanya cek apakah string kosong atau tidak)
int validate_generic_cert(const char *id) {
    int len = strlen(id);
    if (len < 4 || len > 50) return 0;
    for (int i = 0; i < len; i++) {
        if (!isspace((unsigned char)id[i])) return 1;
    }
    return 0;
}

// Validasi Nomor Ijzah
int validate_ijazah(const char *id) {
    if (strlen(id) != 17) return 0; 
    for (int i = 0; i < 17; i++) {
        if (!isdigit((unsigned char)id[i])) return 0;
    }
    char tahun[3] = {id[5], id[6], '\0'};
    printf("  -> [SISTEM] Terdeteksi Tahun Kelulusan: 20%s\n", tahun);
    return 1;
}

// Menghapus karakter newline \n hasil fgets() agar string rapi
void clean_newline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

// Konversi Enum ke String untuk output tampilan
const char* get_skill_name(enum KeahlianSkill s) {
    switch(s) {
        case TECH: return "Teknologi/Coding";
        case HEALTH: return "Kesehatan/Medis";
        case EDUCATION: return "Pendidikan Sosial";
        case ENVIRONMENT: return "Lingkungan Hidup";
        default: return "Tidak Diketahui";
    }
}

// Konversi Enum Level ke String
const char* get_level_name(enum TingkatSkill l) {
    switch(l) {
        case UNCERTIFIED: return "Uncertified/Otodidak"; 
        case BEGINNER: return "Beginner";
        case INTERMEDIATE: return "Intermediate";
        case EXPERT: return "Expert";
        default: return "None";
    }
}


