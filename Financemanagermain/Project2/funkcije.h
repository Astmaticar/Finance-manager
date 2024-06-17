#ifndef FUNKCIJE_H
#define FUNKCIJE_H

#include <stdio.h>
#include <stdlib.h>
#include "DataType.h"

inline static int provjeriOdgovor(char odgovor);
extern double unesiPrihode();
extern double unesiTroskove();
 extern void prikaziIzvjestaj(const Financije* fin);
 extern void planirajBudzet(const Financije* fin);
 extern void spremiStanje(const Financije* fin);
 extern void ucitajStanje(Financije* fin);
extern int usporediPrihode(const void* a, const void* b);
extern int usporediTroskove(const void* a, const void* b);
extern void sortirajPrihode(Financije* fin);
extern void sortirajTroskove(Financije* fin);
extern void obrisiPodatke(Financije* fin);
 extern void usporediSPrethodnimSaldom(Financije* fin);
 extern  double procitajProsliSaldo();
 extern void pretraziPrihode(const Financije* fin);
 extern void pretraziTroskove(const Financije* fin);
 inline extern void dodajPrihod(Financije* fin, double iznos);
 inline extern void dodajTrosak(Financije* fin, double iznos);
 extern void nastaviIliIzadji(char* nastavak);
 extern void izadjiIzPrograma(Financije* fin);

#endif