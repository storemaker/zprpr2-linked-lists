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

FILM *pridajFilm(FILM *zac, FILM *vloz){
   FILM *akt = zac;
   
   if(zac == NULL) return vloz;
   while(akt->dalsi_film != NULL)
      akt = akt->dalsi_film;
   akt->dalsi_film = vloz;
   return zac;
}

HEREC *pridajHerca(HEREC *zac, HEREC *vloz){
   HEREC *akt = zac;
   
   if(zac == NULL) return vloz;
   while(akt->dalsi_herec != NULL)
      akt = akt->dalsi_herec;
   akt->dalsi_herec = vloz;
   return zac;
}

void vypisZoznam(FILM *zac) {
   while(zac != NULL) {
      printf("%s\n", zac->nazov_filmu);
      zac = zac->dalsi_film;
   }
}

void nacitaj(FILM **filmy)
{
    FILM *temp = (FILM *) malloc(sizeof(FILM));
    HEREC *herci = NULL, *temp_herci = (HEREC *) malloc(sizeof(HEREC));
    FILE *subor;
    char c;
    int read_mode = 1, pocet_filmov = 0, pocet_opakovani = 0;
    
    // osetrenie otvorenia suboru
    if ((subor = fopen(FILE_PATH, "r")) == NULL) {
        printf("Subor filmy.txt sa nepodarilo nacitat.\n");
        return;
    }

    
    while ((c = getc(subor)) != EOF)
    {
        
        // getc vo while posunie pointer na druhy znak v subore
        // a potom fscanf nenacita prvy znak v subore, toto je
        // fix pre tuto situaciu
        pocet_opakovani++;
        if(pocet_filmov == 0 && pocet_opakovani == 1)
            temp->nazov_filmu[0] = ungetc(c, subor);
        
        // rok vyroby
        if(isnewline(c) && isdigit(fpeek(subor))) {
            read_mode = 2;
        // reziser a novy film
        } else if (isnewline(c) && isalpha(fpeek(subor))) {
            // reziser
            if (read_mode == 2)
                read_mode = 3;
            // novy film
            else if (read_mode == 4) {
                pocet_filmov++;
                read_mode = 1;
                //temp->herec = herci;
                temp->dalsi_film = (FILM *) malloc(sizeof(FILM));
                *filmy = pridajFilm(*filmy, temp);
                temp = temp->dalsi_film;
            }
        // herci
        } else if (isnewline(c) && !isalpha(fpeek(subor)) && fpeek(subor) != EOF) {
            if (read_mode == 3 || read_mode == 4)
                read_mode = 4;
            
            temp_herci->dalsi_herec = (HEREC *) malloc(sizeof(HEREC));
            herci = pridajHerca(herci, temp_herci);
            temp_herci = temp_herci->dalsi_herec;
            
        // posledny film na pridanie
        } else if (fpeek(subor) == EOF) {
            pocet_filmov++;
            temp->herec = herci;
            temp->dalsi_film = (FILM *) malloc(sizeof(FILM));
            *filmy = pridajFilm(*filmy, temp);
            temp->dalsi_film = NULL;
        }
        
        if (read_mode == 1) {
            fscanf(subor, "%100[^\n]s", temp->nazov_filmu);
        } else if (read_mode == 2) {
            fscanf(subor, "%d", &temp->rok_vyroby);
        } else if (read_mode == 3) {
            fscanf(subor, "%100s %100s", temp->meno_rezisera.krstne_meno, temp->meno_rezisera.priezvisko);
        } else if (read_mode == 4) {
            fscanf(subor, "%*c %s %s %d", temp_herci->meno_herca.krstne_meno, temp_herci->meno_herca.priezvisko, &temp_herci->rok_narodenia);
        }
        
    }
    
    // osetrenie zatvorenia suboru
    if ((fclose(subor)) == EOF) {
        printf("Subor filmy.txt sa nepodarilo zatvorit.\n");
        return;
    }
}

int main(int argc, const char * argv[]) {
    
    FILM *filmy = NULL;
    
    nacitaj(&filmy);
    vypisZoznam(filmy);
    
    return 0;
}
