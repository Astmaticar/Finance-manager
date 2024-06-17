#ifndef DATATYPE_H
#define DATATYPE_H

enum MenuOption {
    UnesiPrihode = 1,
    UnesiTroskove,
    PrikaziIzvjestaj,
    PlanirajBudzet,
    ObrisiPodatke,
    SortirajPrihode,
    SortirajTroskove,
    UsporediSPrethodnimSaldom,
    PretraziPrihode,
    PretraziTroskove,
    IzadjiIzPrograma
};

typedef struct {
    double* prihodi;
    double* troskovi;
    size_t brojPrihoda;
    size_t brojTroskova;
} Financije;

#endif