#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "funkcije.h"

void prikaziNaslov() {
    printf("===================================\n");
    printf("     Osobni financijski menadzer    \n");
    printf("===================================\n");
}

int main() {
    static int izbor;
    static char nastavak;
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
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
            dodajPrihod(fin, unesiPrihode());
            break;
        case 2:
            dodajTrosak(fin, unesiTroskove());
            break;
        case 3:
            prikaziIzvjestaj(fin);
            break;
        case 4:
            planirajBudzet(fin);
            break;
        case 5:
            obrisiPodatke(fin);
            break;
        case 6:
            sortirajPrihode(fin);
            break;
        case 7:
            sortirajTroskove(fin);
            break;
        case 8:
            usporediSPrethodnimSaldom(fin);
            break;
        case 9:
            pretraziPrihode(fin);
            break;
        case 10:
            pretraziTroskove(fin);
            break;
        case 11:
            izadjiIzPrograma(fin);
            return 0;
        default:
            printf("Nevazeci izbor. Pokusajte ponovo.\n");
        }

        nastaviIliIzadji(&nastavak);
    } while (nastavak == 'd' || nastavak == 'D');

    izadjiIzPrograma(fin);

    return 0;
}