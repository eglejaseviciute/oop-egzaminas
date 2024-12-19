#include "funkcijos.h"
#include "myLib.h"

int main() {
    try {

        TekstoAnalizatorius analizatorius("tekstas.txt");

        // Rezultatu failo sukurimas
        cout << "Sukurtas failas 'zodziu_pasikartojimai_vietos.txt' kuriame isspausdinami zodziai, kurie pasikartoja daugiau nei viena karta.\n" << endl;
        ofstream isvedimas("zodziu_pasikartojimai_vietos.txt");
        cout << "Sukurtas failas 'svetainiu_adresai.txt' kuriame isspausdinami visi rasti URL adresai.\n" << endl;

        analizatorius.spausdintiPasikartojanciusZodzius(isvedimas);
        isvedimas.close();

        analizatorius.spausdintiSvetainiuAdresus();

    }
    catch (const std::exception& e) {
        std::cerr << "Klaida: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}