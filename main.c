#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*Code wrote by Matej Volansky 2020*/
#define FILE_N "filmy.txt"
#define MAX_LEN 101
/*.........CHYBOVE HLASKY.........*/
#define NOT_LOADED "Zoznam nie je nacitany.\n" 
#define WRONG_INPUT "Zle zadany vstup.\n"
#define NOT_FOUND "Dany vstup nebol najdeny.\n"

typedef struct meno {
	char k_meno[MAX_LEN];
	char priezvisko[MAX_LEN];
} MENO;

typedef struct herec {
	MENO m_herca;
	int rok_n;
	struct herec* next;
} HEREC;

typedef struct film {
	char nazov[MAX_LEN];
	int rok_v;
	MENO m_rezisera;
	HEREC* zoznam_h;
	struct film* next;
} FILM;

/*POMOCNA FUNKCIA:.........UVOLNENIE PAMATE CELEHO ZOZNAMU HERCOV.........*/
HEREC* unboundActors(HEREC* start) {
	HEREC* p_zoznam = NULL, *act = start;
	while (act != NULL) {	// Pre kazdy zoznam hercov
		p_zoznam = act;
		act = act->next;
		free(p_zoznam);
	}
	return NULL;
}

/*POMOCNA FUNKCIA:.........UVOLNENIE PAMATE CELEHO ZOZNAMU FILMOV.........*/
FILM* unbound(FILM* start) {
	FILM* p_start = NULL, * act = start;
	while (act != NULL) { // Pre kazdy zoznam filmov
		act->zoznam_h = unboundActors(act->zoznam_h); // Uvolni sa zoznam hercov
		p_start = act;
		act = act->next;
		free(p_start);
	}
	return NULL;
}

/*POMOCNA FUNKCIA:.........PRIDANIE FILMU DO ZOZNAMU.........*/
FILM* addMovie(FILM* start, FILM* add) {
	FILM* act = start;
	if (start == NULL) { start = add; return start; } // Ak v zozname este sa nenachadza ziadny zaznam, prida film na zaciatok a vrati zaciatok
	while (act->next != NULL) // Ak v zozname uz zaznamy su, dostane sa nakoniec
		act = act->next;
	act->next = add; // A ulozi tam film ktory sa ziada pridat
	return start;
}

/*POMOCNA FUNKCIA:.........PRIDANIE HERCA DO ZOZNAMU.........*/
HEREC* addActor(HEREC* start, HEREC* add) {
	HEREC* act = start;
	if (start == NULL) { start = add; return start; } // Rovnako ako pri predoslom pridavani filmov
	while (act->next != NULL)
		act = act->next;
	act->next = add;
	return start;
}

/*HLAVNA FUNCKIA:...........NACITANIE ZOZNAMU ZO SUBORU........*/
FILM* load(FILM* start) {
	FILE* f;
	int c;
	if ((f = fopen(FILE_N, "r")) == NULL) { // Osetrenie otvarania suboru
		printf("Nepodarilo sa otvorit subor\n");
		return NULL;
	}

	start = unbound(start); // Prve sa uvolni zoznam, ak je zoznam prazdny funkcia vrati iba NULL

	while ((c = getc(f)) != EOF) { // Nacitava kontrolny znak az do konca suboru
		FILM* add = (FILM*)malloc(sizeof(FILM)); // Film ktory chceme vlozit do zoznamu
		ungetc(c, f); // Nacitany znak vrati do buffera kedze sa jednalo len o kontrolu
		if (add) { // Ak bolo spravne alokovane miesto v pamati pre add
			if (fscanf(f, "%100[^\n]", add->nazov) != 0 && fscanf(f, "%d\n%100s %100s\n", &add->rok_v, add->m_rezisera.k_meno, add->m_rezisera.priezvisko) != 0) {
				HEREC* startActors = NULL; // Zaciatok pre zoznam hercov
				while ((c = getc(f)) == '*' && c != EOF) { // Ak na zaciatku nenacita hviezdicku alebo nacita EOF tak skonci while
					add->zoznam_h = (HEREC*)malloc(sizeof(HEREC)); // Vo filme alokujeme zoznam hercov
					if (add->zoznam_h) { // Ak bolo spravne alokovane
						if (fscanf(f, "%100s %100s %d\n", add->zoznam_h->m_herca.k_meno, add->zoznam_h->m_herca.priezvisko, &add->zoznam_h->rok_n) != 0);
						add->zoznam_h->next = NULL;
						startActors = addActor(startActors, add->zoznam_h); // Prida do zoznamu nacitaneho herca
					}
				}
				ungetc(c, f);
				add->next = NULL;
				add->zoznam_h = startActors; // Prida na zaciatok zoznam hercov
				start = addMovie(start, add); // Prida do celkoveho zoznamu cely film
			}
			else 
				printf(WRONG_INPUT);
		}
	}

	if (fclose(f) == EOF) { // Osetrenie zatvorenia suboru
		printf("Nepodarilo sa zatvorit subor.\n");
		return NULL;
	}
	return start;
}

/*HLAVNA FUNKCIA:..............VYPIS CELEHO ZOZNAMU............*/
void output(FILM* start) {
	if (start == NULL)
		printf(NOT_LOADED); // Ak nieje nacitany zoznam, vypise sa chybova hlaska
	while (start != NULL) {
		printf("%s (%d) %s %s\nHraju: ", start->nazov, start->rok_v, start->m_rezisera.k_meno, start->m_rezisera.priezvisko);
		HEREC* act = start->zoznam_h;
		while (1) {
			if (act) {
				printf("%s %s %d", act->m_herca.k_meno, act->m_herca.priezvisko, act->rok_n);
				act = act->next;
			}
			if (act == NULL) // Ak nie je este na konci zoznamu, vypise ciarku, inak skonci
				break;
			else
				printf(", ");
		}
		printf("\n");
		start = start->next;
	}
}

/*HLAVNA FUNKCIA:...........PRIDANIE FILMU DO ZOZNAMU..........*/
FILM* addRecord(FILM* start) { // V podstate to iste co funkcia load ale nacitava zo stdin a necita do EOF ale do *
	int c;
	FILM* add = (FILM*)calloc(1,sizeof(FILM));
	if (add) {
		if (scanf("\n%100[^\n]", add->nazov) != 0 && scanf("%d\n%100s %100s", &add->rok_v, add->m_rezisera.k_meno, add->m_rezisera.priezvisko) != 0) {
			HEREC* startA = NULL;
			while ((c = getchar()) != '*') {
				ungetc(c, stdin);
				add->zoznam_h = (HEREC*)calloc(1, sizeof(HEREC));
				if (add->zoznam_h) {
					if (fscanf(stdin, "%100s %100s %d\n", add->zoznam_h->m_herca.k_meno, add->zoznam_h->m_herca.priezvisko, &add->zoznam_h->rok_n) != 0);
					add->zoznam_h->next = NULL;
					startA = addActor(startA, add->zoznam_h);
				}
			}
			add->next = NULL;
			add->zoznam_h = startA;
			start = addMovie(start, add);
		}
		else
			printf(WRONG_INPUT);
	}
	return start;
}

/*POMOCNA FUNKCIA:...........UVOLNENIE PAMATE PRE JEDEN ZAZNAM..........*/
FILM* unboundRecord(FILM* record) {
	FILM* p_record = record;
	HEREC* p_zoznam = NULL;
	while (record->zoznam_h != NULL) {
		p_zoznam = record->zoznam_h;
		record->zoznam_h = record->zoznam_h->next;
		free(p_zoznam);
	}
	free(p_record);
	return NULL;
}

/*HLAVNA FUNKCIA:...........VYMAZANIE DANEHO FILMU ZO ZOZNAMU..........*/
FILM* removeRecord(FILM* start) {
	if (start != NULL) {
		FILM* act = start;
		char title[MAX_LEN] = { '\0' };
		if (scanf("\n%100[^\n]", title) != 0) {
			while (act != NULL) {
				if (!strcmp(act->nazov, title)) { // Ak sa zadany nazov rovna nazvu zo zoznamu 
					if (act == start) { // Ak je zaznam hned na zaciatku
						FILM* new_start = start->next; // Vytvori sa novy zaciatok ktory je posunuty o 1 do zadu
						start = unboundRecord(start); // Originalny zaciatok sa uvolni z pamati
						return new_start; // Vrati sa novy
					}
					else { 
						FILM* temp = start; // Vytvori sa novy act s nazvom temp
						while (temp->next != act) // Dostane sa pred act
							temp = temp->next;
						temp->next = act->next == NULL ? NULL : act->next; // Ak je zaznam uplne na konci tak temp dalsi je NULL, ak je zaznam niekde mimo tak temp sa prepise na act dalsi
						act = unbound(act); // Originalny act sa uvolni z pamati
						return start;
					}
				}
				act = act->next;
			}
			printf(NOT_FOUND);
			return start;
		}
		else printf(WRONG_INPUT);
	}
	else printf(NOT_LOADED);
	return NULL;
}

/*HLAVNA FUNKCIA:...........VYPIS FILMOV V KTORYCH HRAL DANY HEREC..........*/
void Movies(FILM* start) {
	if (start != NULL) {
		char name[MAX_LEN] = { '\0' }, lastname[MAX_LEN] = { '\0' };
		if (scanf("\n%100s %100s", name, lastname) != 0) {
			while (start != NULL) {
				HEREC* act = start->zoznam_h;
				while (act != NULL) {
					if (!strcmp(act->m_herca.k_meno, name) && !strcmp(act->m_herca.priezvisko, lastname)) // Ak sa v zozname dane meno nachadza, vypise sa nazov filmu
						printf("%s (%d)\n", start->nazov, start->rok_v);
					act = act->next;
				}
				start = start->next;
			}
		}
		else printf(WRONG_INPUT);
	}
	else printf(NOT_LOADED);
}

/*HLAVNA FUNKCIA:...........VYPIS ROVNAKYCH HERCOV KTORY HRALI V DVOCH DANYCH FILMOV..........*/
void Actors(FILM* start){
	if (start != NULL) {
		char name1[MAX_LEN] = { '\0' }, name2[MAX_LEN] = { '\0' };
		if (scanf("\n%100[^\n]\n%100[^\n]", name1, name2) != 0) {
			HEREC* act1 = NULL, * act2 = NULL;
			while (start != NULL) {
				if (!act1 && !strcmp(start->nazov, name1)) // Ak este nebol najdeny dany film cislo 1
					act1 = start->zoznam_h;
				if (!act2 && !strcmp(start->nazov, name2)) // Ak este nebol najdeny dany film cislo 2
					act2 = start->zoznam_h;
				if (act1 && act2) { // Ak uz boli oba najdene
					HEREC* temp = act2; // Aby sme mohli vratit naspat act2 ked skonci druhy cyklus
					while (act1 != NULL) { // Dve vnorene cykly - napr ako pri sortoch for(i... ){ for(j...){} }
						while (act2 != NULL) {
							if (!strcmp(act1->m_herca.k_meno, act2->m_herca.k_meno) && !strcmp(act1->m_herca.priezvisko, act2->m_herca.priezvisko)) {
								printf("%s %s (%d)\n", act1->m_herca.k_meno, act1->m_herca.priezvisko, act1->rok_n); // Ak sa nachadza herec v oboch filmoch, vypise sa
							}
							act2 = act2->next;
						}
						act2 = temp;
						act1 = act1->next;
					}
					break;
				}
				start = start->next;
			}
		}
		else printf(WRONG_INPUT);
	}
	else printf(NOT_LOADED);
}

/*POMOCNA FUNKCIA:...........VYPIS ZOZNAMU..........*/
void outputYear(HEREC* start){
	HEREC* act = start;
	while (1) {
		if (act) {
			printf("%s %s (%d)", act->m_herca.k_meno, act->m_herca.priezvisko, act->rok_n);
			act = act->next;
		}
		if (act == NULL)
			break;
		else
			printf(", ");
	}
	printf("\n");
}

/*POMOCNA FUNKCIA:...........USPORIADANIE ZOZNAMU..........*/
HEREC* sort(HEREC* start) {
	HEREC* act = start, *compare = start->next, *temp = NULL;
	while (act != NULL) {
		while (compare != NULL) {
			if ((temp = (HEREC*)malloc(sizeof(HEREC)))) {
				if (strcmp(act->m_herca.priezvisko, compare->m_herca.priezvisko) > 0 // Vsetky pripady kedy sa ma usporiadat zoznam
					|| !strcmp(act->m_herca.priezvisko, compare->m_herca.priezvisko) && strcmp(act->m_herca.k_meno, compare->m_herca.k_meno) > 0
					|| !strcmp(act->m_herca.priezvisko, compare->m_herca.priezvisko) && !strcmp(act->m_herca.k_meno, compare->m_herca.k_meno) && act->rok_n > compare->rok_n)
				{
					strcpy(temp->m_herca.priezvisko, act->m_herca.priezvisko); // Prekopiruje sa priezvisko
					strcpy(act->m_herca.priezvisko, compare->m_herca.priezvisko);
					strcpy(compare->m_herca.priezvisko, temp->m_herca.priezvisko);

					strcpy(temp->m_herca.k_meno, act->m_herca.k_meno); // Meno
					strcpy(act->m_herca.k_meno, compare->m_herca.k_meno);
					strcpy(compare->m_herca.k_meno, temp->m_herca.k_meno);

					temp->rok_n = act->rok_n; // Rok narodenia
					act->rok_n = compare->rok_n;
					compare->rok_n = temp->rok_n;
				}
			}
			compare = compare->next;
		}
		act = act->next;
		compare = act;
	}
	return start;
}

/*HLAVNA FUNKCIA:...........VYPIS USPORIADANEHO ZOZNAMU HERCOV KTORY HRALI VO FILMOCH Z DANEHO ROKU..........*/
void Year(FILM* start) {
	if (start != NULL) {
		int year = 0, found = 0;
		HEREC* act = NULL, *copy, *zoznam=NULL;
		if (scanf("%d", &year) != 0) {
			while (start != NULL) {
				if (year == start->rok_v) { // Ak sa dany rok rovna roku filmu 
					act = start->zoznam_h;
					found = 1; // Uz sa nasiel jeden zaznam s rovnakym rokom
					while (act != NULL) {
						if ((copy = (HEREC*)malloc(sizeof(HEREC)))) { // Prekopiruje sa act do copy, a tak sa copy prida do zoznamu
							strcpy(copy->m_herca.k_meno, act->m_herca.k_meno);
							strcpy(copy->m_herca.priezvisko, act->m_herca.priezvisko);
							copy->rok_n = act->rok_n;
							copy->next = NULL;
							HEREC* act_zoznam = zoznam;
							while (act_zoznam != NULL) { // Kontrola ci uz rovnaky herec nieje v zozname
								if (!strcmp(copy->m_herca.k_meno, act_zoznam->m_herca.k_meno) && !strcmp(copy->m_herca.priezvisko, act_zoznam->m_herca.priezvisko) && act_zoznam->rok_n == copy->rok_n)
									break;
								act_zoznam = act_zoznam->next;
							}
							if (act_zoznam == NULL)
								zoznam = addActor(zoznam, copy);
						}
						act = act->next;
					}
				}
				start = start->next;
			}
			if (found) { // Ak sa nasiel film s rovnakym zadanym rokom
				zoznam = sort(zoznam); // Zoznam sa usporiada
				outputYear(zoznam); // Vypise
				zoznam = unboundActors(zoznam); // Tak sa uvolni z pamate
			}
			else printf(NOT_FOUND);
		}
		else printf(WRONG_INPUT);
	}
	else printf(NOT_LOADED);
}

int main() {
	FILM* start = NULL;
	while(1) {
		char prikaz[8] = { '\0' }; // Najdlhsi prikaz ma 7 znakov + 1 koncovy znak
		if (scanf("%8s", prikaz) != 0);
		if (!strcmp(prikaz, "nacitaj"))
			start = load(start);
		else if (!strcmp(prikaz, "vypis"))
			output(start);
		else if (!strcmp(prikaz, "pridaj"))
			start = addRecord(start);
		else if (!strcmp(prikaz, "vymaz"))
			start = removeRecord(start);
		else if (!strcmp(prikaz, "filmy"))
			Movies(start);
		else if (!strcmp(prikaz, "herci"))
			Actors(start);
		else if (!strcmp(prikaz, "rok"))
			Year(start);
		else if (!strcmp(prikaz, "koniec"))
			return 0;
	}
}
