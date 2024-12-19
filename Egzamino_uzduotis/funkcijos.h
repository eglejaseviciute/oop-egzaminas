#ifndef FUNKCIJOS_H
#define FUNKCIJOS_H


#include "myLib.h"


// Teksto analizatoriaus klase, skirta teksto apdorojimui ir statistikos rinkimui
class TekstoAnalizatorius {
private:
    // Saugo zodi ir jo pasikartojimo skaiciu
    map<string, int> zodzioPasikartojimas;

    // Saugo kiekviena zodi ir eiluciu numeriu rinkini
    map<string, set<int> > zodzioEilutes;

    // Vektorius originalaus teksto eilutėms saugoti
    vector<string> tekstoEilutes;

    // Rinkinys unikaliu svetainiu adresu saugojimui
    set<string> svetainesAdresai;


    // Privatus pagalbiniai metodai
    // Metodas: zodzio raides konvertuoja i mazasias raides ir pasalina skyrybos zenklus
    string normalizuotiZodi(const string& zodis);

    // Metodas svetainiu adresams isskirti is teksto
    void isskirtiSvetainiuAdresus();

public:
    // Konstruktorius, kuris nuskaito faila ir atlieka pradine teksto analize
    TekstoAnalizatorius(const string& failoPavadinimas);

    // Metodas spausdinantis pasikartojančius zodzius kartu su ju eiluciu numeriais
    void spausdintiPasikartojanciusZodzius(ofstream& isvedimas);

    // Metodas spausdinantis svetainiu adresus
    void spausdintiSvetainiuAdresus();

    
   
};
#endif // FUNKCIJOS_H