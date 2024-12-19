#include "funkcijos.h"
#include "myLib.h"


// Pagalbine funkcija tikrinimui, ar simbolis yra skyrybos ženklas (naudoja ispunct() funkciją skyrybos ženklų identifikavimui)
bool arSkyrybosZenklas(char c) {
    return ispunct(static_cast<unsigned char>(c)) != 0;
}


string TekstoAnalizatorius::normalizuotiZodi(const string& zodis) {
    string normalizuotas = zodis;

    // Konvertavimas i mazasias raides
    for (char& raide : normalizuotas) {
        raide = tolower(static_cast<unsigned char>(raide));
    }

    // Skyrybos zenklu pasalinimas
    string valytasZodis;
    for (char simbolis : normalizuotas) {
        if (!arSkyrybosZenklas(simbolis)) {
            valytasZodis += simbolis;
        }
    }

    // Patikrinimas, ar zodyje yra bent viena raide
    bool yraRaide = any_of(valytasZodis.begin(), valytasZodis.end(),
        [](char c) { return isalpha(static_cast<unsigned char>(c)); });

    // Jei nera raidziu, grazinamas tuscias string
    return yraRaide ? valytasZodis : "";
}


void TekstoAnalizatorius::isskirtiSvetainiuAdresus() {
    // Sujungia visas teksto eilutes i viena eilute
    string visasTekstas;
    for (const auto& eilute : tekstoEilutes) {
        visasTekstas += eilute + " ";
    }

    // Reguliarioji israiska URL adresu atpazinimui
    regex url_regex(R"((https?://)?([a-zA-Z]+[a-zA-Z0-9.-]*\.[a-zA-Z]{2,})(/\S*)?)",
        regex::icase);

    // Iteratoriai URL adresu paieskai tekste
    auto zodziai_pradzia = sregex_iterator(visasTekstas.begin(), visasTekstas.end(), url_regex);
    auto zodziai_pabaiga = sregex_iterator();

    // Rastu URL adresu itraukimas i rinkini
    for (sregex_iterator i = zodziai_pradzia; i != zodziai_pabaiga; ++i) {
        smatch sutapimas = *i;
        string pilnasUrl = sutapimas.str();

        svetainesAdresai.insert(pilnasUrl);
    }
}


// Konstruktorius failo nuskaitymui ir pradinei analizei
TekstoAnalizatorius::TekstoAnalizatorius(const string& failoPavadinimas) {
    ifstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        throw runtime_error("Nepavyko atidaryti failo!");
    }

    string eilute;
    int eilutesNumeris = 1;

    // Eiluciu skaitymas ir apdorojimas
    while (getline(failas, eilute)) {
        // Originali eilute issaugoma
        tekstoEilutes.push_back(eilute);

        // Zodziu isskyrimas is eilutes
        istringstream iss(eilute);
        string zodis;

        while (iss >> zodis) {
            // Zodzio normalizavimas
            string normalizuotasZodis = normalizuotiZodi(zodis);

            if (!normalizuotasZodis.empty()) {
                // Zodzio pasikartojimu skaiciavimas
                zodzioPasikartojimas[normalizuotasZodis]++;

                // Eiluciu numeriu, kuriose zodis pasirodo, saugojimas
                zodzioEilutes[normalizuotasZodis].insert(eilutesNumeris);
            }
        }
        eilutesNumeris++;
    }

    // Svetainiu adresu isskyrimas
    isskirtiSvetainiuAdresus();
}


// Metodas pasikartojanciu zodziu spausdinimui
void TekstoAnalizatorius::spausdintiPasikartojanciusZodzius(ofstream& isvedimas) {
    // Stulpeliu plociu nustatymas
    const int pasikartojimaiWidth = 35;
    const int eiluteWidth = 30;
    const int zodisWidth = 20;

    // Antrastes spausdinimas
    isvedimas << left
        << setw(pasikartojimaiWidth) << "Pasikartojimu skaicius"
        << setw(eiluteWidth) << "Eilute"
        << "Zodis" << endl;

    // Atskiramosios eilutes spausdinimas
    isvedimas << string(pasikartojimaiWidth + eiluteWidth + zodisWidth, '-') << endl;

    // Zodziu pasikartojimu iteravimas
    for (const auto& pora : zodzioPasikartojimas) {
        if (pora.second > 1) {
            // Pasikartojimu skaiciaus spausdinimas
            isvedimas << left << setw(pasikartojimaiWidth) << pora.second;

            // Eiluciu numeriu spausdinimas
            const set<int>& eilutes = zodzioEilutes.at(pora.first);
            string eiluciuTekstas;
            for (int eilute : eilutes) {
                eiluciuTekstas += to_string(eilute) + " ";
            }
            isvedimas << left << setw(eiluteWidth) << eiluciuTekstas;

            // Zodzio spausdinimas
            isvedimas << right << pora.first << endl;
        }
    }
}


// Svetainiu adresu spausdinimo metodas
void TekstoAnalizatorius::spausdintiSvetainiuAdresus() {
    // Konsoles isvestis
    cout << "Rasti svetainiu adresai:\n";
    cout << "----------------------------\n";

    // Failo kurimas URL adresu saugojimui
    ofstream url_failas("svetainiu_adresai.txt");
    if (!url_failas.is_open()) {
        cerr << "Nepavyko sukurti URL failo!" << endl;
        return;
    }

    url_failas << "Rasti svetainiu adresai:\n" << "----------------------------" << endl;

    // Rastu svetainiu adresu spausdinimas
    for (const auto& adresas : svetainesAdresai) {
        // Spausdinimas ekrane
        cout << adresas << "\n";

        // Spausdinimas faile
        url_failas << adresas << "\n";
    }

    url_failas.close();
}

