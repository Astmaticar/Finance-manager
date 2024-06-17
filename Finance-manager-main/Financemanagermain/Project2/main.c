#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "funkcije.h"
#include "DataType.h"

void prikaziNaslov() {
    printf("===================================\n");
    printf("     Osobni financijski menadzer    \n");
    printf("===================================\n");
}

int main() {
    int izbor;
    char nastavak; 
    Financije* fin = (Financije*)malloc(sizeof(Financije));
    if (!fin) {
        perror("Greska pri alokaciji memorije");
        return 1;
    }

    fin->prihodi = NULL;
    fin->troskovi = NULL;
    fin->brojPrihoda = 0;
    fin->brojTroskova = 0;

    prikaziNaslov();
    ucitajStanje(fin);

    do {
        printf("\n1. Unesi prihode\n");
        printf("2. Unesi troskove\n");
        printf("3. Prikazi izvjestaj\n");
        printf("4. Planiraj budzet\n");
        printf("5. Obrisi sve podatke\n");
        printf("6. Sortiraj prihode\n");
        printf("7. Sortiraj troskove\n");
        printf("8. Usporedi s prethodnim saldom\n");
        printf("9. Pretrazi prihode\n");
        printf("10. Pretrazi troskove\n");
        printf("11. Izlaz\n");
        printf("Izaberite opciju: ");

        if (scanf_s("%d", &izbor) != 1) {
            printf("Nevazeci izbor. Molimo unesite broj.\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (izbor) {
        case UnesiPrihode:
            dodajPrihod(fin, unesiPrihode());
            break;
        case UnesiTroskove:
            dodajTrosak(fin, unesiTroskove());
            break;
        case PrikaziIzvjestaj:
            prikaziIzvjestaj(fin);
            break;
        case PlanirajBudzet:
            planirajBudzet(fin);
            break;
        case ObrisiPodatke:
            obrisiPodatke(fin);
            break;
        case SortirajPrihode:
            sortirajPrihode(fin);
            break;
        case SortirajTroskove:
            sortirajTroskove(fin);
            break;
        case UsporediSPrethodnimSaldom:
            usporediSPrethodnimSaldom(fin);
            break;
        case PretraziPrihode:
            pretraziPrihode(fin);
            break;
        case PretraziTroskove:
            pretraziTroskove(fin);
            break;
        case IzadjiIzPrograma:
            izadjiIzPrograma(fin);
            return 0;
        default:
            printf("Nevazeci izbor. Pokusajte ponovo.\n");
            while (getchar() != '\n'); 
        }

        nastaviIliIzadji(&nastavak);
    } while (nastavak == 'd' || nastavak == 'D');

    izadjiIzPrograma(fin);

    return 0;
}
