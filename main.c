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
#include <string.h>
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
    HEREC *herci;
    struct film *dalsi_film;
} FILM;

FILM *pridajFilm(FILM *head, HEREC *herci, char nazov_filmu[101], char meno_rezisera[101], char priezvisko_rezisera[101], int rok_vyroby)
{
    FILM *temporary = malloc(sizeof(FILM));
    FILM *tail = head;
    
    strcpy(temporary->nazov_filmu, nazov_filmu);
    strcpy(temporary->meno_rezisera.krstne_meno, meno_rezisera);
    strcpy(temporary->meno_rezisera.priezvisko, priezvisko_rezisera);
    temporary->rok_vyroby = rok_vyroby;
    temporary->herci = herci;
    temporary->dalsi_film = NULL;
    
    if (head == NULL) return temporary;
    
    while (tail->dalsi_film != NULL)
        tail = tail->dalsi_film;
    tail->dalsi_film = temporary;
    
    return head;
}

HEREC *pridajHerca(HEREC *head, char krstne_meno[101], char priezvisko[101], int rok_narodenia)
{
    HEREC *temporary = malloc(sizeof(HEREC));
    HEREC *tail = head;
    strcpy(temporary->meno_herca.krstne_meno, krstne_meno);
    strcpy(temporary->meno_herca.priezvisko, priezvisko);
    temporary->rok_narodenia = rok_narodenia;
    temporary->dalsi_herec = NULL;
    
    if (head == NULL) return temporary;
    
    while (tail->dalsi_herec != NULL)
        tail = tail->dalsi_herec;
    tail->dalsi_herec = temporary;
    
    return head;
}

void vypis(FILM *head)
{
    HEREC *herci = NULL;
    
    if (head == NULL) {
        printf("Nebol nacitany ziadny film, prosim, skontrolujte, ci boli pouzite prikazy \"nacitaj\" alebo \"pridaj\"\n");
        return;
    }
    
    while (head != NULL)
    {
        herci = head->herci;
        printf("%s (%d) %s %s\n\tHraju: ", head->nazov_filmu, head->rok_vyroby, head->meno_rezisera.krstne_meno, head->meno_rezisera.priezvisko);
        while (herci != NULL)
        {
            if(herci->dalsi_herec != NULL)
                printf("%s %s (%d), ", herci->meno_herca.krstne_meno, herci->meno_herca.priezvisko, herci->rok_narodenia);
            else
                printf("%s %s (%d)", herci->meno_herca.krstne_meno, herci->meno_herca.priezvisko, herci->rok_narodenia);
            herci = herci->dalsi_herec;
        }
        head = head->dalsi_film;
        printf("\n");
    }
}

FILM *pridaj(FILM *filmy)
{
    char meno_herca[101], priezvisko_herca[101], nazov_filmu[101], meno_rezisera[101], priezvisko_rezisera[101];
    int rok_narodenia, rok_vyroby = 0;
    HEREC *herci = NULL;
    
    scanf("%100[^\n]s", nazov_filmu);
    scanf("%4d", &rok_vyroby);
    scanf("%100s %100s", meno_rezisera, priezvisko_rezisera);
    
    while (1)
    {
        scanf("%100s", meno_herca);
        if(meno_herca[0] == '*')
            break;
        scanf("%100s", priezvisko_herca);
        scanf("%4d", &rok_narodenia);
        
        herci = pridajHerca(herci, meno_herca, priezvisko_herca, rok_narodenia);
    }
    
    return filmy = pridajFilm(filmy, herci, nazov_filmu, meno_rezisera, priezvisko_rezisera, rok_vyroby);
}

void nacitaj(FILM **filmy)
{
    HEREC *herci = NULL;
    FILE *subor;
    char c, meno_herca[101], priezvisko_herca[101], nazov_filmu[101], meno_rezisera[101], priezvisko_rezisera[101];
    int read_mode = 1, pocet_filmov = 0, pocet_opakovani = 0, rok_narodenia, rok_vyroby = 0;
    
    // osetrenie otvorenia suboru
    if ((subor = fopen(FILE_PATH, "r")) == NULL) {
        printf("Subor filmy.txt sa nepodarilo nacitat.\n");
        return;
    }

    while ((c = getc(subor)) != EOF)
    {
        /* readmodes: 1 - nazov filmu, 2 - rok vyroby filmu, 3 - meno rezisera filmu, 4 - herci, ktori hraju vo filme */
        
        // getc vo while posunie pointer na druhy znak v subore
        // a potom fscanf nenacita prvy znak v subore, toto je
        // fix pre tuto situaciu
        pocet_opakovani++;
        if (pocet_filmov == 0 && pocet_opakovani == 1)
            nazov_filmu[0] = ungetc(c, subor);
        
        // rok vyroby
        if (isnewline(c) && isdigit(fpeek(subor))) {
            read_mode = 2;
        }
        // reziser a novy film
        else if (isnewline(c) && isalpha(fpeek(subor))) {
            // reziser
            if (read_mode == 2)
                read_mode = 3;
            // novy film
            else if (read_mode == 4) {
                pocet_filmov++;
                read_mode = 1;
                *filmy = pridajFilm(*filmy, herci, nazov_filmu, meno_rezisera, priezvisko_rezisera, rok_vyroby);
                herci = NULL;
            }
        }
        // herci
        else if (isnewline(c) && !isalpha(fpeek(subor)) && fpeek(subor) != EOF) {
            if (read_mode == 3 || read_mode == 4)
                read_mode = 4;

        }
        // posledny film na pridanie
        else if (fpeek(subor) == EOF) {
            *filmy = pridajFilm(*filmy, herci, nazov_filmu, meno_rezisera, priezvisko_rezisera, rok_vyroby);
        }
        
        if (read_mode == 1) {
            fscanf(subor, "%100[^\n]s", nazov_filmu);
        } else if (read_mode == 2) {
            fscanf(subor, "%4d", &rok_vyroby);
        } else if (read_mode == 3) {
            fscanf(subor, "%100s %100s", meno_rezisera, priezvisko_rezisera);
        } else if (read_mode == 4) {
            fscanf(subor, "%*c %s %s %4d", meno_herca, priezvisko_herca, &rok_narodenia);
            // hotfix, aby sa posledny herec nepridal 2x
            if (fpeek(subor) != EOF)
                herci = pridajHerca(herci, meno_herca, priezvisko_herca, rok_narodenia);
        }
    }
    
    // osetrenie zatvorenia suboru
    if ((fclose(subor)) == EOF) {
        printf("Subor filmy.txt sa nepodarilo zatvorit.\n");
        return;
    }
}

void vypisFilmovPodlaHerca(FILM *head)
{
    char meno_herca[101], priezvisko_herca[101];
    HEREC *herci = NULL;
    
    if (head == NULL) {
        printf("Nebol nacitany ziadny film, prosim, skontrolujte, ci boli pouzite prikazy \"nacitaj\" alebo \"pridaj\"\n");
        return;
    }
    
    scanf("%100s %100s", meno_herca, priezvisko_herca);

    while (head != NULL)
    {
        herci = head->herci;
        
        while (herci != NULL)
        {
            if(strcmp(herci->meno_herca.krstne_meno, meno_herca) == 0 && strcmp(herci->meno_herca.priezvisko, priezvisko_herca) == 0)
                printf("%s (%d)\n", head->nazov_filmu, head->rok_vyroby);
            herci = herci->dalsi_herec;
        }
        
        head = head->dalsi_film;
    }
    
}

void vyhladanieHercovVoFilmoch(FILM *head, char string_hladany_film1[101], char string_hladany_film2[101])
{
    /*FILM *hladany_film1 = NULL, *hladany_film2 = NULL, *temporary = NULL;
    int pocet_najdenych_filmov = 0;*/
    //char string_hladany_film1[101];//, string_hladany_film2[101];
    
    if (head == NULL) {
        printf("Nebol nacitany ziadny film, prosim, skontrolujte, ci boli pouzite prikazy \"nacitaj\" alebo \"pridaj\"\n");
        return;
    }
    
    // nacitanie filmov, ktore sa maju vyhladat
    //scanf("%[^\n]s", string_hladany_film1);
    //fgets(string_hladany_film1, 100, stdin);
    printf("%s", string_hladany_film1);
    //scanf("%100[^\n]s", string_hladany_film2);
    
    /*while (head != NULL)
    {
        // najdenie 1. filmu a ulozenie filmu do pomocnej premennej
        if (!strcmp(head->nazov_filmu, string_hladany_film1)) {
            pocet_najdenych_filmov++;
            printf("nasiel som film 1: %s", head->nazov_filmu);
            hladany_film1 = head;
        }
        // najdenie 2. filmu a ukoncenie loopu, kedze som nasiel 2 z 2 filmov
        else if ((!strcmp(head->nazov_filmu, string_hladany_film2)) && pocet_najdenych_filmov) {
            pocet_najdenych_filmov++;
            printf("nasiel som film 2: %s", head->nazov_filmu);
            hladany_film2 = head;
            break;
        }
        head = head->dalsi_film;
    }*/
    
   /* while (hladany_film1->herci != NULL)
    {
        temporary = hladany_film2;
        
        while (temporary->herci != NULL)
        {
            if(!strcmp(hladany_film1->herci->meno_herca.krstne_meno, temporary->herci->meno_herca.krstne_meno) && !strcmp(hladany_film1->herci->meno_herca.priezvisko, temporary->herci->meno_herca.priezvisko))
                printf("%s (%d)", hladany_film1->nazov_filmu, hladany_film1->rok_vyroby);
            temporary->herci = temporary->herci->dalsi_herec;
        }
        
        hladany_film1->herci = hladany_film1->herci->dalsi_herec;
    } */
    
}

int main(int argc, const char * argv[]) {
    
    FILM *filmy = NULL;
    char handler[101], hladany_film1[101], hladany_film2[101];
    
    while(scanf("%100s", handler) == 1)
    {
        if (!strcmp(handler, "nacitaj"))
            nacitaj(&filmy);
        else if (!strcmp(handler, "pridaj"))
            filmy = pridaj(filmy);
        else if (!strcmp(handler, "vypis"))
            vypis(filmy);
        else if (!strcmp(handler, "filmy"))
            vypisFilmovPodlaHerca(filmy);
        else
            printf("Neznamy prikaz.\n");
    }
    
    return 0;
}
