#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funkcije.h"
#include "DataType.h"


const char* DATOTEKA = "stanje.txt";

static FILE* otvorenaDatoteka(const char* naziv, const char* mode, int silent) {
    FILE* file = fopen(naziv, mode);
    if (!file && !silent) {
        perror("Greska pri otvaranju datoteke");
    }
    return file;
}

inline static int provjeriOdgovor(char odgovor) {
    return (odgovor == 'd' || odgovor == 'D' || odgovor == 'n' || odgovor == 'N');
}

extern double unesiPrihode() {
    double prihodi;
    static char buffer[100];

    printf("Unesite iznos prihoda: ");
    if (scanf("%lf", &prihodi) != 1) {
        printf("Pogresan unos. Molimo unesite broj.\n");
        fgets(buffer, sizeof(buffer), stdin);
        return unesiPrihode();
    }

    return prihodi;
}

extern double unesiTroskove() {
    double troskovi;
    static char buffer[100];

    printf("Unesite iznos troskova: ");
    if (scanf("%lf", &troskovi) != 1) {
        printf("Pogresan unos. Molimo unesite broj.\n");
        fgets(buffer, sizeof(buffer), stdin);
        return unesiTroskove();
    }

    return troskovi;
}

extern void prikaziIzvjestaj(const Financije* fin) {
    if (fin == NULL || fin->prihodi == NULL || fin->troskovi == NULL) {
        fprintf(stderr, "Error: Invalid parameters passed to prikaziIzvjestaj\n");
        return;
    }

    double ukupniPrihodi = 0.0;
    double ukupniTroskovi = 0.0;
    printf("\nIzvjestaj o financijama:\n");
    for (size_t i = 0; i < fin->brojPrihoda; ++i) {
        ukupniPrihodi += fin->prihodi[i];
    }
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        ukupniTroskovi += fin->troskovi[i];
    }
    printf("Ukupni prihodi: %.2lf\n", ukupniPrihodi);
    printf("Ukupni troskovi: %.2lf\n", ukupniTroskovi);
    printf("Saldo: %.2lf\n", ukupniPrihodi - ukupniTroskovi);
}

extern void planirajBudzet(const Financije* fin) {
    if (fin == NULL || fin->troskovi == NULL) {
        fprintf(stderr, "Error: Invalid parameters passed to planirajBudzet\n");
        return;
    }

    double planiraniTroskovi;

    while (1) {
        printf("Unesite planirani iznos troskova: ");
        if (scanf("%lf", &planiraniTroskovi) == 1) {
            break; 
        }
        else {
            printf("Unos nije validan broj. Molimo unesite ponovo.\n");
            while (getchar() != '\n'); 
        }
    }

    double trenutniTroskovi = 0.0;
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        trenutniTroskovi += fin->troskovi[i];
    }

    double razlika = planiraniTroskovi - trenutniTroskovi;
    if (razlika >= 0) {
        printf("Niste premasili planirani iznos troskova. Mozete potrositi jos %.2lf.\n", razlika);
    }
    else {
        printf("Premasili ste planirani iznos troskova za %.2lf.\n", -razlika);
    }
}

extern void spremiStanje(const Financije* fin) {
    if (fin == NULL || fin->prihodi == NULL || fin->troskovi == NULL) {
        fprintf(stderr, "Error: Invalid parameters passed to spremiStanje\n");
        return;
    }

    FILE* file = otvorenaDatoteka(DATOTEKA, "w", 0);
    if (!file) return;
    fprintf(file, "%zu %zu\n", fin->brojPrihoda, fin->brojTroskova);
    for (size_t i = 0; i < fin->brojPrihoda; ++i) {
        fprintf(file, "%.2lf\n", fin->prihodi[i]);
    }
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        fprintf(file, "%.2lf\n", fin->troskovi[i]);
    }
    fclose(file);
}

extern void ucitajStanje(Financije* fin) {
    if (fin == NULL) {
        fprintf(stderr, "Error: Invalid parameter passed to ucitajStanje\n");
        return;
    }

    FILE* file = otvorenaDatoteka(DATOTEKA, "r", 0);
    if (!file) {
        printf("Nema prethodnog stanja. Pocetak s nula.\n");
        return;
    }
    rewind(file);

    fscanf(file, "%zu %zu", &fin->brojPrihoda, &fin->brojTroskova);
    free(fin->prihodi);
    free(fin->troskovi);
    fin->prihodi = malloc(fin->brojPrihoda * sizeof(double));
    fin->troskovi = malloc(fin->brojTroskova * sizeof(double));
    if (fin->prihodi == NULL || fin->troskovi == NULL) {
        perror("Greska pri alokaciji memorije");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < fin->brojPrihoda; ++i) {
        fscanf(file, "%lf", &fin->prihodi[i]);
    }
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        fscanf(file, "%lf", &fin->troskovi[i]);
    }
    fclose(file);
}


extern int usporediPrihode(const void* a, const void* b) {
    const double* prihodiA = (const double*)a;
    const double* prihodiB = (const double*)b;
    if (*prihodiA < *prihodiB) return -1;
    else if (*prihodiA > *prihodiB) return 1;
    else return 0;
}

extern int usporediTroskove(const void* a, const void* b) {
    const double* troskoviA = (const double*)a;
    const double* troskoviB = (const double*)b;
    if (*troskoviA < *troskoviB) return -1;
    else if (*troskoviA > *troskoviB) return 1;
    else return 0;
}

extern void sortirajPrihode(Financije* fin) {
    if (fin == NULL || fin->prihodi == NULL) {
        fprintf(stderr, "Error: Invalid parameters passed to sortirajPrihode\n");
        return;
    }

    if (fin->brojPrihoda > 0) {
        qsort(fin->prihodi, fin->brojPrihoda, sizeof(double), usporediPrihode);
        printf("Sortirani prihodi:\n");
        for (size_t i = 0; i < fin->brojPrihoda; ++i) {
            printf("%.2lf\n", fin->prihodi[i]);
        }
    }
    else {
        printf("Nema prihoda za sortiranje.\n");
    }
}

extern void sortirajTroskove(Financije* fin) {
    if (fin == NULL || fin->troskovi == NULL) {
        fprintf(stderr, "Error: Invalid parameters passed to sortirajTroskove\n");
        return;
    }

    if (fin->brojTroskova > 0) {
        qsort(fin->troskovi, fin->brojTroskova, sizeof(double), usporediTroskove);
        printf("Sortirani troskovi:\n");
        for (size_t i = 0; i < fin->brojTroskova; ++i) {
            printf("%.2lf\n", fin->troskovi[i]);
        }
    }
    else {
        printf("Nema troskova za sortiranje.\n");
    }
}

    
extern void obrisiPodatke(Financije* fin) {
    char odgovor;
    while (1) {
        printf("Jeste li sigurni da zelite obrisati sve podatke? (d/n): ");
        if (scanf(" %c", &odgovor) != 1 || (odgovor != 'd' && odgovor != 'D' && odgovor != 'n' && odgovor != 'N')) {
            printf("Nevazeci odgovor. Molimo unesite 'd' ili 'n'.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    if (odgovor == 'd' || odgovor == 'D') {
        free(fin->prihodi);
        free(fin->troskovi);
        fin->prihodi = NULL;
        fin->troskovi = NULL;

        if (remove("stanje.txt") != 0) {
            printf("Greska pri brisanju datoteke.\n");
        }
        else {
            printf("Svi podaci su obrisani.\n");

            fin->prihodi = malloc(0 * sizeof(double));
            fin->troskovi = malloc(0 * sizeof(double));
            if (fin->prihodi == NULL || fin->troskovi == NULL) {
                perror("Greska pri alokaciji memorije");
                exit(EXIT_FAILURE);
            }

            fin->brojPrihoda = 0;
            fin->brojTroskova = 0;
        }
    }
    else {
        printf("Brisanje podataka je otkazano.\n");
    }
}





extern double procitajProsliSaldo() {
    FILE* file = otvorenaDatoteka(DATOTEKA, "r", 1);
    if (!file) {
        return 0.0;
    }
    double saldo = 0.0;
    fscanf(file, "%lf", &saldo);
    fclose(file);
    return saldo;
}

extern void usporediSPrethodnimSaldom(Financije* fin) {
    double saldoProsli = procitajProsliSaldo(); 
    double trenutniSaldo = 0.0;

    for (size_t i = 0; i < fin->brojPrihoda; ++i) {
        trenutniSaldo += fin->prihodi[i];
    }
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        trenutniSaldo -= fin->troskovi[i];
    }

    double razlika = trenutniSaldo - saldoProsli;

    if (razlika > 0) {
        printf("Trenutni saldo je bolji od proslog za %.2lf\n", razlika);
    }
    else if (razlika < 0) {
        printf("Trenutni saldo je losiji od proslog za %.2lf\n", -razlika);
    }
    else {
        printf("Trenutni saldo je isti kao i prosli.\n");
    }

    FILE* file = otvorenaDatoteka(DATOTEKA, "w", 0); 
    if (file) {
        fprintf(file, "%.2lf\n", trenutniSaldo);
        fclose(file);
    }
    else {
        printf("Nije moguce azurirati datoteku s trenutnim saldom.\n");
    }
}


extern void pretraziPrihode(const Financije* fin) {
    double trazeniPrihod;

    do {
        printf("Unesite iznos prihoda koji zelite pronaci: ");
        if (scanf("%lf", &trazeniPrihod) != 1) {
            printf("Neispravan unos. Molimo unesite broj.\n");
            while (getchar() != '\n');
        }
        else {
            while (getchar() != '\n'); 
            break; 
        }
    } while (1);

    double* found = (double*)bsearch(&trazeniPrihod, fin->prihodi, fin->brojPrihoda, sizeof(double), usporediPrihode);

    if (found) {
        printf("Pronaden je prihod od %.2lf.\n", *found);
    }
    else {
        printf("Nije pronaden prihod od %.2lf.\n", trazeniPrihod);
    }
}



extern void pretraziTroskove(const Financije* fin) {
    double trazeniTrosak;

    do {
        printf("Unesite iznos troška koji zelite pronaci: ");
        if (scanf("%lf", &trazeniTrosak) != 1) {
            printf("Neispravan unos. Molimo unesite broj.\n");
            while (getchar() != '\n'); 
        }
        else {
            while (getchar() != '\n'); 
            break; 
        }
    } while (1);

    double* found = (double*)bsearch(&trazeniTrosak, fin->troskovi, fin->brojTroskova, sizeof(double), usporediTroskove);

    if (found) {
        printf("Pronaden je trosak od %.2lf.\n", *found);
    }
    else {
        printf("Nije pronaden trosak od %.2lf.\n", trazeniTrosak);
    }
}


inline extern void dodajPrihod(Financije* fin, double iznos) {
    if (fin == NULL) {
        fprintf(stderr, "Error: Invalid parameter passed to dodajPrihod\n");
        return;
    }

    fin->brojPrihoda++;
    double* temp = realloc(fin->prihodi, fin->brojPrihoda * sizeof(double));
    if (temp == NULL) {
        perror("Greska pri realokaciji memorije");
        exit(EXIT_FAILURE);
    }
    fin->prihodi = temp;
    fin->prihodi[fin->brojPrihoda - 1] = iznos;
}

inline extern void dodajTrosak(Financije* fin, double iznos) {
    if (fin == NULL) {
        fprintf(stderr, "Error: Invalid parameter passed to dodajTrosak\n");
        return;
    }

    fin->brojTroskova++;
    double* temp = realloc(fin->troskovi, fin->brojTroskova * sizeof(double));
    if (temp == NULL) {
        perror("Greska pri realokaciji memorije");
        exit(EXIT_FAILURE);
    }
    fin->troskovi = temp;
    fin->troskovi[fin->brojTroskova - 1] = iznos;
}


extern void nastaviIliIzadji(char* nastavak) {
    if (nastavak == NULL) {
        fprintf(stderr, "Error: Invalid parameter passed to nastaviIliIzadji\n");
        return;
    }

    char unos[10];

    while (1) {
        printf("Zelite li nastaviti? (d/n): ");
        if (scanf(" %9s", unos) != 1) {
            printf("Neispravan unos. Molimo unesite 'd' ili 'n'.\n");
            while (getchar() != '\n');
            continue;
        }

        *nastavak = unos[0]; 

        if (*nastavak == 'd' || *nastavak == 'D' || *nastavak == 'n' || *nastavak == 'N') {
            break; 
        }
        else {
            printf("Nevazeci odgovor. Molimo unesite 'd' ili 'n'.\n");
            while (getchar() != '\n'); 
        }
    }
}

extern void izadjiIzPrograma(Financije* fin) {
    if (fin == NULL) {
        printf("Greska: NULL pokazivac proslijeden u funkciju izadjiIzPrograma.\n");
        return;
    }

    printf("Izlaz iz programa.\n");
    spremiStanje(fin); 
    free(fin->prihodi); 
    free(fin->troskovi); 
    fin->prihodi = NULL;
    fin->troskovi = NULL;
    free(fin); 
    fin = NULL;
}



