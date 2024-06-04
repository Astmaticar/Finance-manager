#define _CRT_SECURE_NO_WARNINGS

#include "funkcije.h"
#include <stdlib.h>
#include <string.h>

static FILE* otvorenaDatoteka(const char* naziv, const char* mod) {
    FILE* file = fopen(naziv, mod);
    if (!file) {
        perror("Greska pri otvaranju datoteke");
    }
    return file;
}

static int provjeriOdgovor(char odgovor) {
    return (odgovor == 'd' || odgovor == 'D' || odgovor == 'n' || odgovor == 'N');
}

 double unesiPrihode() {
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

double unesiTroskove() {
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


static void prikaziIzvjestaj(const Financije* fin) {
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

static void planirajBudzet(const Financije* fin) {
    double planiraniTroskovi;
    printf("Unesite planirani iznos troskova: ");
    scanf("%lf", &planiraniTroskovi);

    double trenutniTroskovi = 0.0;
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        trenutniTroskovi += fin->troskovi[i];
    }

    double razlika = planiraniTroskovi - trenutniTroskovi;

    if (razlika >= 0) {
        printf("Niste premašili planirani iznos troskova. Možete potrošiti još %.2lf.\n", razlika);
    }
    else {
        printf("Premašili ste planirani iznos troskova za %.2lf.\n", -razlika);
    }
}

static void spremiStanje(const Financije* fin) {
    FILE* file = otvorenaDatoteka(DATOTEKA, "w");
    if (!file) return;
    fprintf(file, "%zu %zu\n", fin->brojPrihoda, fin->brojTroskova);
    for (size_t i = 0; i < fin->brojPrihoda; ++i) {
        fprintf(file, "%lf\n", fin->prihodi[i]);
    }
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        fprintf(file, "%lf\n", fin->troskovi[i]);
    }
    fclose(file);
}

static void ucitajStanje(Financije* fin) {
    FILE* file = otvorenaDatoteka(DATOTEKA, "r");
    if (!file) {
        printf("Nema prethodnog stanja. Pocetak s nula.\n");
        return;
    }
    fscanf(file, "%zu %zu", &fin->brojPrihoda, &fin->brojTroskova);
    fin->prihodi = malloc(fin->brojPrihoda * sizeof(double));
    fin->troskovi = malloc(fin->brojTroskova * sizeof(double));
    for (size_t i = 0; i < fin->brojPrihoda; ++i) {
        fscanf(file, "%lf", &fin->prihodi[i]);
    }
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        fscanf(file, "%lf", &fin->troskovi[i]);
    }
    fclose(file);
}

static int usporediPrihode(const void* a, const void* b) {
    const double* prihodiA = (const double*)a;
    const double* prihodiB = (const double*)b;
    if (*prihodiA < *prihodiB) return -1;
    else if (*prihodiA > *prihodiB) return 1;
    else return 0;
}

static int usporediTroskove(const void* a, const void* b) {
    const double* troskoviA = (const double*)a;
    const double* troskoviB = (const double*)b;
    if (*troskoviA < *troskoviB) return -1;
    else if (*troskoviA > *troskoviB) return 1;
    else return 0;
}

static void sortirajPrihode(Financije* fin) {
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

static void sortirajTroskove(Financije* fin) {
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

static void obrisiPodatke(Financije* fin) {
    static char odgovor;
    do {
        printf("Jeste li sigurni da zelite obrisati sve podatke? (d/n): ");
        scanf(" %c", &odgovor);
        if (!provjeriOdgovor(odgovor)) {
            printf("Nevazeci odgovor. Molimo unesite 'd' ili 'n'.\n");
        }
    } while (!provjeriOdgovor(odgovor));

    if (odgovor == 'd' || odgovor == 'D') {
        free(fin->prihodi);
        free(fin->troskovi);
        fin->prihodi = NULL;
        fin->troskovi = NULL;
        fin->brojPrihoda = 0;
        fin->brojTroskova = 0;
        printf("Svi podaci su obrisani.\n");
    }
    else {
        printf("Brisanje podataka je otkazano.\n");
    }
}



static void usporediSPrethodnimSaldom(Financije* fin) {
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
        printf("Trenutni saldo je bolji od prethodnog za %.2lf.\n", razlika);
    }
    else if (razlika < 0) {
        printf("Trenutni saldo je gori od prethodnog za %.2lf.\n", -razlika);
    }
    else {
        printf("Trenutni saldo je jednak prethodnom.\n");
    }
}

double procitajProsliSaldo() {
    FILE* file = otvorenaDatoteka(DATOTEKA, "r");
    if (!file) {
        return 0.0;
    }
    size_t brojPrihoda, brojTroskova;
    double prihodi, troskovi;
    fscanf(file, "%zu %zu", &brojPrihoda, &brojTroskova);
    double saldo = 0.0;
    for (size_t i = 0; i < brojPrihoda; ++i) {
        fscanf(file, "%lf", &prihodi);
        saldo += prihodi;
    }
    for (size_t i = 0; i < brojTroskova; ++i) {
        fscanf(file, "%lf", &troskovi);
        saldo -= troskovi;
    }
    rewind(file);
    fclose(file);
    return saldo;
}

static void pretraziPrihode(const Financije* fin) {
    double trazeniPrihod;
    printf("Unesite iznos prihoda koji želite pronaći: ");
    scanf("%lf", &trazeniPrihod);
    static int found = 0;
    for (size_t i = 0; i < fin->brojPrihoda; ++i) {
        if (fin->prihodi[i] == trazeniPrihod) {
            printf("Pronađen je prihod od %.2lf.\n", trazeniPrihod);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Nije pronađen prihod od %.2lf.\n", trazeniPrihod);
    }
}

static void pretraziTroskove(const Financije* fin) {
    double trazeniTrosak;
    printf("Unesite iznos troska koji želite pronaći: ");
    scanf("%lf", &trazeniTrosak);
    static int found = 0;
    for (size_t i = 0; i < fin->brojTroskova; ++i) {
        if (fin->troskovi[i] == trazeniTrosak) {
            printf("Pronađen je trosak od %.2lf.\n", trazeniTrosak);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Nije pronađen trosak od %.2lf.\n", trazeniTrosak);
    }
}

static void dodajPrihod(Financije* fin, double iznos) {
    fin->brojPrihoda++;
    fin->prihodi = realloc(fin->prihodi, fin->brojPrihoda * sizeof(double));
    if (!fin->prihodi) {
        perror("Greska pri alokaciji memorije za prihode");
        exit(1);
    }
    fin->prihodi[fin->brojPrihoda - 1] = iznos;
}

static void dodajTrosak(Financije* fin, double iznos) {
    fin->brojTroskova++;
    fin->troskovi = realloc(fin->troskovi, fin->brojTroskova * sizeof(double));
    if (!fin->troskovi) {
        perror("Greska pri alokaciji memorije za troskove");
        exit(1);
    }
    fin->troskovi[fin->brojTroskova - 1] = iznos;
}

static void nastaviIliIzadji(char* nastavak) {
    do {
        printf("Zelite li nastaviti? (d/n): ");
        scanf(" %c", nastavak);
        if (!provjeriOdgovor(*nastavak)) {
            printf("Nevazeci odgovor. Molimo unesite 'd' ili 'n'.\n");
        }
    } while (!provjeriOdgovor(*nastavak));
}

static void izadjiIzPrograma(Financije* fin) {
    FILE *a = fopen("file.txt", "w+");
    printf("Izlaz iz programa.\n");
    fclose(a);
    remove("file.txt");
    spremiStanje(fin);
    free(fin->prihodi);
    free(fin->troskovi);
    free(fin);
}
