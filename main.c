//
//  main.c
//  zprpr2 projekt2
//
//  Created by Jakub Taraba on 04/05/2020.
//  Copyright © 2020 Jakub Taraba. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "helpers.h"
#define FILE_PATH "/Users/storemaker/Projects/zprpr2 projekt2/zprpr2 projekt2/filmy.txt"

typedef struct {
    char krstne_meno[101];
    char priezvisko[101];
} MENO;

typedef struct herec {
    MENO meno_herca;
    int rok_narodenia;
    struct herec *dalsi_herec;
} HEREC;

typedef struct film {
    char nazov_filmu[101];
    int rok_vyroby;
    MENO meno_rezisera;
    HEREC *herec;
    struct film *dalsi_film;
} FILM;

void nacitaj(FILM **filmy)
{
    FILE *subor;
    char c;
    int read_mode = 1, line = 1;
    
    // osetrenie otvorenia suboru
    if ((subor = fopen(FILE_PATH, "r")) == NULL) {
        printf("Subor filmy.txt sa nepodarilo nacitat.\n");
        return;
    }
    
    printf("%d: read_mode: %d\n", line, read_mode);
    
    while ((c = getc(subor)) != EOF)
    {
        if(isnewline(c) && isdigit(fpeek(subor))) {
            line++;
            read_mode = 2;
            printf("%d: read_mode: %d\n", line, read_mode);
        } else if (isnewline(c) && isalpha(fpeek(subor))) {
            line++;
            if (read_mode == 2)
                read_mode = 3;
            else if (read_mode == 4)
                read_mode = 1;
            printf("%d: read_mode: %d\n", line, read_mode);
        } else if (isnewline(c) && !isalpha(fpeek(subor)) && fpeek(subor) != EOF) {
            line++;
            if (read_mode == 3 || read_mode == 4)
                read_mode = 4;
            printf("%d: read_mode: %d\n", line, read_mode);
        }
        
        /*if (read_mode == 1) {
            fscanf(subor, "%100[^\n]", (*filmy)->nazov_filmu);
        } else if (read_mode == 2) {
            fscanf(subor, "%d", &(*filmy)->rok_vyroby);
        } else if (read_mode == 3) {
            fscanf(subor, "%100s %100s", (*filmy)->meno_rezisera.krstne_meno, (*filmy)->meno_rezisera.priezvisko);
        } else if (read_mode == 4) {
            
            fscanf("%*c %s %s %d", )
        }*/
        
    }
    
    // osetrenie zatvorenia suboru
    if ((fclose(subor)) == EOF) {
        printf("Subor filmy.txt sa nepodarilo zatvorit.\n");
        return;
    }
}

int main(int argc, const char * argv[]) {
    
    FILM *filmy = NULL;
    filmy = (FILM *) malloc(1 * sizeof(FILM));
    filmy[0].herec = (HEREC *) malloc(1 * sizeof(HEREC));
    
    // nazov filmu
    //scanf("%100s %d %100s %100s", filmy[0].nazov, &filmy[0].rok_vyroby, filmy[0].meno_rezisera.krstne_meno, filmy[0].meno_rezisera.priezvisko);
    //scanf("%100s %100s", filmy[0].herec->meno_herca.krstne_meno, filmy[0].herec->meno_herca.priezvisko);
    
    //printf("%s %d %s %s", filmy[0].nazov, filmy[0].rok_vyroby, filmy[0].meno_rezisera.krstne_meno, filmy[0].meno_rezisera.priezvisko);
    //printf("%s %s", filmy[0].herec->meno_herca.krstne_meno, filmy[0].herec->meno_herca.priezvisko);
    nacitaj(&filmy);
    
    return 0;
}
