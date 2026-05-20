#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//buat mendefene konstanta
#define MAX_SKILL_PER_PERSON 3
#define STR_LEN 50
#define INITIAL_CAPACITY 5

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
