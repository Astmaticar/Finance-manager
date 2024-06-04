#ifndef FUNKCIJE_H
#define FUNKCIJE_H

#include <stdio.h>
#include <stdlib.h>

static const char* DATOTEKA = "stanje.txt";

typedef struct {
    double* prihodi;
    double* troskovi;
    size_t brojPrihoda;
    size_t brojTroskova;
} Financije;

static int provjeriOdgovor(char odgovor);
double unesiPrihode();
double unesiTroskove();
static void prikaziIzvjestaj(const Financije* fin);
static void planirajBudzet(const Financije* fin);
static void spremiStanje(const Financije* fin);
static void ucitajStanje(Financije* fin);
int usporediPrihode(const void* a, const void* b);
int usporediTroskove(const void* a, const void* b);
static void sortirajPrihode(Financije* fin);
static void sortirajTroskove(Financije* fin);
static void obrisiPodatke(Financije* fin);
static void usporediSPrethodnimSaldom(Financije* fin);
double procitajProsliSaldo();
static void pretraziPrihode(const Financije* fin);
static void pretraziTroskove(const Financije* fin);
static void dodajPrihod(Financije* fin, double iznos);
static void dodajTrosak(Financije* fin, double iznos);
static void nastaviIliIzadji(char* nastavak);
static void izadjiIzPrograma(Financije* fin);

#endif
