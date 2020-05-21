#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "helpers.h"
#define FILE_PATH "/Users/storemaker/Projects/zprpr2 projekt2/zprpr2 projekt2/filmy.txt"
#define RESULTS_NOT_FOUND_MSG "Zoznam filmov neobsahuje ziadne filmy.\nSkuste nacitat filmy zo suboru alebo pomocou funkcie 'pridaj'."

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


// funkcia na pridanie filmu do zoznamu vsetkych filmov
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
    
    // ak nebol pridany este ziaden film
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
    
    // ak nebol pridany este ziaden herec
    if (head == NULL) return temporary;
    
    while (tail->dalsi_herec != NULL)
        tail = tail->dalsi_herec;
    
    tail->dalsi_herec = temporary;
    
    return head;
}

HEREC *vymazHercov(HEREC *head)
{
    HEREC *temp = head;
    HEREC *tail;

    while (temp != NULL)
    {
        tail = temp->dalsi_herec;
        free(temp);
        temp = tail;
    }
    return NULL;
}

void vypis(FILM *head)
{
    if (head == NULL) {
        printf(RESULTS_NOT_FOUND_MSG);
        return;
    }
    
    HEREC *herci = NULL;

    while (head != NULL)
    {
        herci = head->herci;
        printf("%s (%d) %s %s\n\tHraju: ", head->nazov_filmu, head->rok_vyroby, head->meno_rezisera.krstne_meno, head->meno_rezisera.priezvisko);
        while (herci != NULL)
        {
            // pokial existuje este dalsi herec, vypisem ciarku
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
    char meno_herca[101], priezvisko_herca[101], nazov_filmu[101], meno_rezisera[101], priezvisko_rezisera[101], garbage;
    int rok_narodenia, rok_vyroby = 0;
    HEREC *herci = NULL;
    
    // premenna "garbage" na nacitanie znaku noveho riadku
    scanf("%c", &garbage);
    
    // nacitavanie udajov o filme, prosim, nove filmy pridavajte
    // manualnym pisanim, nie copy-paste, inak nacitavanie spravne nefunguje
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

FILM *vymazNtyFilm(FILM *head, int n)
{
    FILM *temp1 = head;
    
    // pokial chcem vymazat prvy film
    if (n == 1) {
        head->herci = vymazHercov(temp1->herci);
        head = temp1->dalsi_film;
        free(temp1);
        return head;
    }
    
    FILM *temp2 = head;
    
    // dostanem sa pred film, ktory chcem vymazat
    for (int i = 0; i < n-2; i++)
        temp1 = temp1->dalsi_film;
    
    // temp2 je film, ktory chcem vymazat
    temp2 = temp1->dalsi_film;
    // dealokujem hercov z filmu, ktory chcem vymazat
    temp2->herci = vymazHercov(temp2->herci);
    // zmenim pointer na film, ktory nasleduje po vymazanom filme
    temp1->dalsi_film = temp2->dalsi_film;
    
    // dealokujem pointer
    free(temp2);
    
    return head;
}

FILM *vymazFilm(FILM *head)
{
    // nie su nacitane ziadne filmy
    if (head == NULL) {
        printf(RESULTS_NOT_FOUND_MSG);
        return head;
    }
    
    char nazov_filmu[101], garbage;
    FILM *temporary = head;
    int counter = 1;
    
    // nacitanie, ktory film sa ma vymazat
    scanf("%c", &garbage);
    scanf("%100[^\n]s", nazov_filmu);
    
    // zistim poradie filmu, ktory sa ma vymazat
    while (temporary != NULL)
    {
        // pokial sa tento check rovna 0, nasiel som poradie filmu,
        // ktory sa ma vymazat
        if(strcmp(temporary->nazov_filmu, nazov_filmu) == 0)
            break;
        temporary = temporary->dalsi_film;
        counter++;
    }
    
    // vymazanie korektneho filmu
    head = vymazNtyFilm(head, counter);
    return head;
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
        
        // funkcia fpeek sa "pozera" na dalsi charakter v subore,
        // tak viem aky dalsi udaj ma nasledovat
        
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
            // reziser, ak viem, ze som predtym nacitaval rok vyroby,
            // viem, ze dalsi nacitany udaj musi byt meno rezisera
            if (read_mode == 2)
                read_mode = 3;
            // nacitanie noveho filmu
            else if (read_mode == 4) {
                pocet_filmov++;
                read_mode = 1;
                // viem, ze mam nacitavat novy film, pridam posledny nacitany film
                // do zoznamu filmov
                *filmy = pridajFilm(*filmy, herci, nazov_filmu, meno_rezisera, priezvisko_rezisera, rok_vyroby);
                // vymazanie zoznamu hercov pre novy film
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
        
        // podla tzv. read modes viem aky udaj mam nacitavat
        if (read_mode == 1) {
            // [^\n] nacitavanie vsetkych znakov az po newline znak,
            // pretoze niektore filmy v subore filmy.txt maju medzery
            fscanf(subor, "%100[^\n]s", nazov_filmu);
        } else if (read_mode == 2) {
            fscanf(subor, "%4d", &rok_vyroby);
        } else if (read_mode == 3) {
            fscanf(subor, "%100s %100s", meno_rezisera, priezvisko_rezisera);
        } else if (read_mode == 4) {
            // %*c pre ignorovanie *
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
    if (head == NULL) {
        printf(RESULTS_NOT_FOUND_MSG);
        return;
    }
    
    char meno_herca[101], priezvisko_herca[101], garbage;
    HEREC *herci = malloc(sizeof(HEREC));
    
    // nacitanie hladaneho herca
    scanf("%c", &garbage);
    scanf("%100s %100s", meno_herca, priezvisko_herca);
    
    // prechadzanie filmami
    while (head != NULL)
    {
        herci = head->herci;
        
        // prechadzanie hercami aktualneho filmu
        while (herci != NULL)
        {
            // ak je hladany herec najdeny, vypise sa v akom filme hraje
            if(strcmp(herci->meno_herca.krstne_meno, meno_herca) == 0 && strcmp(herci->meno_herca.priezvisko, priezvisko_herca) == 0)
                printf("%s (%d)\n", head->nazov_filmu, head->rok_vyroby);
            herci = herci->dalsi_herec;
        }
        head = head->dalsi_film;
    }
    
    // vymazanie alokovaneho pola hercov z heapu
    herci = vymazHercov(herci);
    
}

// funkcia na vymazanie celeho zoznamu filmov
FILM *vymazFilmy(FILM *head)
{
    FILM *temp = head;
    FILM *tail;

    while (temp != NULL)
    {
        tail = temp->dalsi_film;
        // vymazanie zoznamu hercov pre aktualny film
        temp->herci = vymazHercov(temp->herci);
        free(temp);
        temp = tail;
    }
    return NULL;
}

// nefunguje sortovanie podla abecedy a roku, vsetko ostatne pri tomto prikaze funguje
void rok(FILM *head)
{
    if (head == NULL) {
        printf(RESULTS_NOT_FOUND_MSG);
        return;
    }
    
    HEREC *herci = NULL, *temporary = NULL;
    int duplicate = 0;
    
    // prechadzanie vsetkymi filmami
    while (head != NULL)
    {
        // prechadzanie vsetkymi hercami aktualneho prechadzaneho filmu
        while (head->herci != NULL)
        {
            // pomocna premenna pre zoznam hercov
            temporary = herci;
            duplicate = 0;
            
            // pomocny loop na zistenie, ci dany herec je uz v zozname hercov, ktory
            // sa ma vytvorit, vypisat a vymazat
            while (temporary != NULL) {
                if(!strcmp(head->herci->meno_herca.krstne_meno, temporary->meno_herca.krstne_meno) && !strcmp(head->herci->meno_herca.priezvisko, temporary->meno_herca.priezvisko))
                    duplicate = 1;
                temporary = temporary->dalsi_herec;
            }
            
            // ak bol aktualny herec najdeny v novom zozname hercov, nebude pridany
            if(!duplicate)
                herci = pridajHerca(herci, head->herci->meno_herca.krstne_meno, head->herci->meno_herca.priezvisko, head->herci->rok_narodenia);
            head->herci = head->herci->dalsi_herec;
        }
        head = head->dalsi_film;
    }
    
    // vypis vsetkych najdenych hercov
    while (herci != NULL)
    {
        if(herci->dalsi_herec != NULL)
            printf("%s %s (%d), ", herci->meno_herca.krstne_meno, herci->meno_herca.priezvisko, herci->rok_narodenia);
        else
            printf("%s %s (%d)", herci->meno_herca.krstne_meno, herci->meno_herca.priezvisko, herci->rok_narodenia);
        herci = herci->dalsi_herec;
    }
    
    herci = vymazHercov(herci);
    
    printf("\n");
}

void vyhladanieHercovVoFilmoch(FILM *head)
{
    if (head == NULL) {
        printf(RESULTS_NOT_FOUND_MSG);
        return;
    }
    
    FILM *hladany_film1 = malloc(sizeof(FILM)), *hladany_film2 = malloc(sizeof(FILM));
    HEREC *herci = NULL;
    int pocet_najdenych_filmov = 0;
    char string_hladany_film1[101], string_hladany_film2[101], garbage;
    
    // po vstupe do funkcie je v stdin stale newline znak, co znamena,
    // ze s mojim aktualnym riesenim scanf("%100[^\n]s") by tieto scanf
    // nic nenacitali, preto nacitam newline znak do premennej, cim
    // "vymazem" newline znak zo stdin
    scanf("%c", &garbage);
    scanf("%100[^\n]s", string_hladany_film1);
    scanf("%c", &garbage);
    scanf("%100[^\n]s", string_hladany_film2);
    
    
    // prechadzanie zoznamom filmov
    while (head != NULL)
    {
        // ak sa nazov filmu rovna prvemu alebo druhemu nazvu hladaneho filmu
        if (!strcmp(head->nazov_filmu, string_hladany_film1) || !strcmp(head->nazov_filmu, string_hladany_film2)) {
            pocet_najdenych_filmov++;
            
            // pocet najdenych filmov, kvoli tomu, ze neviem v akom poradi su hladane
            // filmy v zozname, preto potrebujem pomocnu premennu, aby som vedel
            // vzdy spravne priradit filmy do pomocnych zoznamov
            if (pocet_najdenych_filmov == 1) {
                // kopirovanie filmu do pomocnej premennej
                strcpy(hladany_film1->nazov_filmu, head->nazov_filmu);
                strcpy(hladany_film1->meno_rezisera.krstne_meno, head->meno_rezisera.krstne_meno);
                strcpy(hladany_film1->meno_rezisera.priezvisko, head->meno_rezisera.priezvisko);
                hladany_film1->rok_vyroby = head->rok_vyroby;
                hladany_film1->herci = head->herci;
                hladany_film1->dalsi_film = NULL;
            } else if (pocet_najdenych_filmov == 2) {
                strcpy(hladany_film2->nazov_filmu, head->nazov_filmu);
                strcpy(hladany_film2->meno_rezisera.krstne_meno, head->meno_rezisera.krstne_meno);
                strcpy(hladany_film2->meno_rezisera.priezvisko, head->meno_rezisera.priezvisko);
                hladany_film2->rok_vyroby = head->rok_vyroby;
                hladany_film2->herci = head->herci;
                hladany_film2->dalsi_film = NULL;
            }
        }
        head = head->dalsi_film;
    }
    
    // vyhladavanie funguje tak, ze pre kazdeho jedneho herca z filmu 1
    // prechadzam vsetkymi hercami z filmu 2 a porovnam, ci sa meno rovna
    // ak sa mena rovnaju, vypisem ho
    while (hladany_film1->herci != NULL)
    {
        // pomocna premenna pre hercov z filmu 2
        herci = hladany_film2->herci;
        
        // prechadzanie vsetkymi hercami z filmu 2 na zaklade hercov z filmu 1
        while (herci != NULL)
        {
            if(!strcmp(hladany_film1->herci->meno_herca.krstne_meno, herci->meno_herca.krstne_meno) && !strcmp(hladany_film1->herci->meno_herca.priezvisko, herci->meno_herca.priezvisko))
                printf("%s %s (%d)\n", herci->meno_herca.krstne_meno, herci->meno_herca.priezvisko, herci->rok_narodenia);
            herci = herci->dalsi_herec;
        }
        // prechadzanie hercami z filmu 1
        hladany_film1->herci = hladany_film1->herci->dalsi_herec;
    }
}

int main(int argc, const char * argv[]) {
    
    FILM *filmy = NULL;
    char handler[101];
    
    // handler pre prikazy
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
        else if (!strcmp(handler, "herci"))
            vyhladanieHercovVoFilmoch(filmy);
        else if (!strcmp(handler, "vymaz"))
            filmy = vymazFilm(filmy);
        else if (!strcmp(handler, "koniec")) {
            filmy = vymazFilmy(filmy);
            exit(0);
        }
        else if (!strcmp(handler, "rok"))
            rok(filmy);
        else
            printf("Neznamy prikaz.\n");
    }
    
    return 0;
}
