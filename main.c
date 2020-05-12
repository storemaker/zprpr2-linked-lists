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

/*POMOCNA FUNKCIA:.........UVOLNENIE PAMATE CELEHO ZOZNAMU.........*/
FILM* unbound(FILM* start) {
	FILM* p_start = NULL, *act = start;
	while (act != NULL) { // Pre kazdy zoznam filmov
		HEREC* p_zoznam = NULL;
		while (act->zoznam_h!= NULL) {	// Pre kazdy zoznam hercov
			p_zoznam = act->zoznam_h;
			act->zoznam_h = act->zoznam_h->next;
			free(p_zoznam);
		}
		p_start = act;
		act = act->next;
		free(p_start);
	}
	return NULL;
}

/*POMOCNA FUNKCIA:.........PRIDANIE FILMU DO ZOZNAMU.........*/
FILM* addMovie(FILM* start, FILM* add) {
	FILM* act = start;
	if (start == NULL) { start = add; return start; } // Ak v zozname este sa nenachadza ziadny zaznam, pridame film na zaciatok a vratime
	while (act->next != NULL) // Ak v zozname uz zaznamy su, dostaneme sa nakoniec
		act = act->next;
	act->next = add; // A ulozime tam film ktory chceme pridat
	return start;
}

/*POMOCNA FUNKCIA:.........PRIDANIE HERCA DO ZOZNAMU.........*/
HEREC* addActor(HEREC* start, HEREC* add) {
	HEREC* act = start;
	if (start == NULL) { start = add; return start; } // Rovnako ako pri predoslej pridavani filmov
	while (act->next != NULL)
		act = act->next;
	act->next = add;
	return start;
}

/*HLAVNA FUNCKIA:...........NACITANIE ZOZNAMU ZO SUBORU........*/
FILM* load(FILM* start) {
	FILE* f;
	int c;
	if ((f = fopen(FILE_N, "r")) == NULL) {
		printf("Nepodarilo sa otvorit subor\n");
		return NULL;
	}

	start = unbound(start);

	while ((c = getc(f)) != EOF) {
		FILM* add = (FILM*)malloc(sizeof(FILM));
		ungetc(c, f);
		if (add) {
			if (fscanf(f, "%100[^\n]", add->nazov) != 0 && fscanf(f, "%d\n%100s %100s\n", &add->rok_v, add->m_rezisera.k_meno, add->m_rezisera.priezvisko) != 0) {
				HEREC* startA = NULL;
				while ((c = getc(f)) == '*' && c != EOF) {
					add->zoznam_h = (HEREC*)malloc(sizeof(HEREC));
					if (add->zoznam_h) {
						if (fscanf(f, "%100s %100s %d\n", add->zoznam_h->m_herca.k_meno, add->zoznam_h->m_herca.priezvisko, &add->zoznam_h->rok_n) != 0);
						add->zoznam_h->next = NULL;
						startA = addActor(startA, add->zoznam_h);
					}
				}
				ungetc(c, f);
				add->next = NULL;
				add->zoznam_h = startA;
				start = addMovie(start, add);
			}
			else
				printf(WRONG_INPUT);
		}
	}

	if (fclose(f) == EOF) {
		printf("Nepodarilo sa zatvorit subor.\n");
		return NULL;
	}
	return start;
}

/*HLAVNA FUNKCIA:..............VYPIS CELEHO ZOZNAMU............*/
void output(FILM* start) {
	if (start == NULL)
		printf(NOT_LOADED);
	while (start != NULL) {
		printf("%s (%d) %s %s\nHraju: ", start->nazov, start->rok_v, start->m_rezisera.k_meno, start->m_rezisera.priezvisko);
		HEREC* act = start->zoznam_h;
		while (1) {
			if (act) {
				printf("%s %s %d", act->m_herca.k_meno, act->m_herca.priezvisko, act->rok_n);
				act = act->next;
			}
			if (act == NULL)
				break;
			else
				printf(", ");
		}
		printf("\n");
		start = start->next;
	}
}

/*HLAVNA FUNKCIA:...........PRIDANIE FILMU DO ZOZNAMU..........*/
FILM* addRecord(FILM* start) {
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
				if (!strcmp(act->nazov, title)) {
					if (act == start) {
						FILM* new_start = start->next;
						start = unboundRecord(start);
						return new_start;
					}
					else  {
						FILM* temp = start;
						while (temp->next != act)
							temp = temp->next;
						temp->next = act->next == NULL ? NULL : act->next;
						act = unbound(act);
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
					if (!strcmp(act->m_herca.k_meno, name) && !strcmp(act->m_herca.priezvisko, lastname))
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
				if (!act1 && !strcmp(start->nazov, name1))
					act1 = start->zoznam_h;
				if (!act2 && !strcmp(start->nazov, name2))
					act2 = start->zoznam_h;
				if (act1 && act2) {
					HEREC* temp = act2;
					while (act1 != NULL) {
						while (act2 != NULL) {
							if (!strcmp(act1->m_herca.k_meno, act2->m_herca.k_meno) && !strcmp(act1->m_herca.priezvisko, act2->m_herca.priezvisko)) {
								printf("%s %s (%d)\n", act1->m_herca.k_meno, act1->m_herca.priezvisko, act1->rok_n);
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

HEREC* sort(HEREC* start) {
	HEREC* act = start, *compare = start->next, *temp = NULL;
	while (act != NULL) {
		while (compare != NULL) {
			if ((temp = (HEREC*)malloc(sizeof(HEREC)))) {
				if (strcmp(act->m_herca.priezvisko, compare->m_herca.priezvisko) > 0 
					|| !strcmp(act->m_herca.priezvisko, compare->m_herca.priezvisko) && strcmp(act->m_herca.k_meno, compare->m_herca.k_meno) > 0
					|| !strcmp(act->m_herca.priezvisko, compare->m_herca.priezvisko) && !strcmp(act->m_herca.k_meno, compare->m_herca.k_meno) && act->rok_n > compare->rok_n)
				{
					strcpy(temp->m_herca.priezvisko, act->m_herca.priezvisko);
					strcpy(act->m_herca.priezvisko, compare->m_herca.priezvisko);
					strcpy(compare->m_herca.priezvisko, temp->m_herca.priezvisko);

					strcpy(temp->m_herca.k_meno, act->m_herca.k_meno);
					strcpy(act->m_herca.k_meno, compare->m_herca.k_meno);
					strcpy(compare->m_herca.k_meno, temp->m_herca.k_meno);

					temp->rok_n = act->rok_n;
					act->rok_n = compare->rok_n;
					compare->rok_n = temp->rok_n;
				}
			}
			compare = compare->next;
		}
		//compare = start;
		act = act->next;
		compare = act;
	}
	return start;
}

void Year(FILM* start) {
	if (start != NULL) {
		int year = 0, found = 0;
		HEREC* act = NULL, *copy, *zoznam=NULL;
		if (scanf("%d", &year) != 0) {
			while (start != NULL) {
				if (year == start->rok_v) {
					act = start->zoznam_h;
					found = 1;
					while (act != NULL) {
						if ((copy = (HEREC*)malloc(sizeof(HEREC)))) {
							strcpy(copy->m_herca.k_meno, act->m_herca.k_meno);
							strcpy(copy->m_herca.priezvisko, act->m_herca.priezvisko);
							copy->rok_n = act->rok_n;
							copy->next = NULL;
							HEREC* act_zoznam = zoznam;
							while (act_zoznam != NULL) {
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
			if (found) {
				zoznam = sort(zoznam);
				outputYear(zoznam);
				unbound(zoznam);
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
		char prikaz[8] = { '\0' };
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
