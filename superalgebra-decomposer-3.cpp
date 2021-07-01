#include <iostream>
#include <string>

#include <time.h>

#include "polynomial-3.h"

using namespace std;

extern const int max;
extern const int max1;

extern const int max_parts;

extern const int pmax;

extern const int nn;

int convert(string element) {

    int value = 0;

    if (element.compare("E31") == 0) value = 8;
    if (element.compare("E21") == 0) value = 7;
    if (element.compare("E32") == 0) value = 6;
    if (element.compare("H12") == 0) value = 5;
    if (element.compare("H23") == 0) value = 4;
    if (element.compare("E23") == 0) value = 3;
    if (element.compare("E12") == 0) value = 2;
    if (element.compare("E13") == 0) value = 1;

    return value;
}

string unconvert(int id) {

    if (id == 8) return "E31";
    if (id == 7) return "E21";
    if (id == 6) return "E32";
    if (id == 5) return "H12";
    if (id == 4) return "H23";
    if (id == 3) return "E23";
    if (id == 2) return "E12";
    if (id == 1) return "E13";
}

int main() {

    printf("Zadej pocet clenu polynomu (max = 10): ");

    int pocet_clenu;

    cin >> pocet_clenu;

    printf("\n");

    int puvodni_polynom[pmax][max][2];

    for (int ip = 0; ip < pmax; ip++) {
        for (int i1 = 0; i1 < max; i1++) {
            for (int i2 = 0; i2 < 2; i2++) {
                puvodni_polynom[ip][i1][i2] = 0;
            }
        }
    }

    int koeficient[pmax];

    for (int ip = 0; ip < pocet_clenu; ip++) {

        printf("Zadej pocet prvku %u.soucinu (max = 10): ", ip + 1);

        int pocet_prvku;

        cin >> pocet_prvku;

        printf("\n");

        printf("Zadej koeficient %u.soucinu: ", ip + 1);

        cin >> koeficient[ip];

        printf("\n");

        for (int i1 = 0; i1 < pocet_prvku; i1++) {
            string choice;
            printf("%u.clen: ", i1 + 1); cin >> choice; puvodni_polynom[ip][i1][0] = convert(choice);
            printf("mocnina: "); cin >> puvodni_polynom[ip][i1][1];
            printf("\n");
        }
    }

    monomial zero_mono = monomial(0, 0, 0);

    polynomial neusporadany_rozklad = polynomial(zero_mono, zero_mono, zero_mono);

    for (int ip = 0; ip < pocet_clenu; ip++) {

        int soucin[max][2];

        for (int i1 = 0; i1 < max; i1++) {
            for (int i2 = 0; i2 < 2; i2++) {
                soucin[i1][i2] = puvodni_polynom[ip][i1][i2];
            }
        }

        polynomial rozkladany_vyraz = polynomial(monomial(koeficient[ip], soucin), zero_mono, zero_mono);

        polynomial rozklad_soucinu = rozkladany_vyraz;

        rozklad_soucinu.decompose();

        neusporadany_rozklad = polynomial(neusporadany_rozklad, rozklad_soucinu, zero_mono);
    }

    polynomial konecny_rozklad = neusporadany_rozklad;

    konecny_rozklad.sorted();




/*
    int test = konecny_rozklad.part[0].number_factor - neusporadany_rozklad.part[0].number_factor;
    if (test == 0) test = abs(konecny_rozklad.part[1].number_factor);
    int l = 0;
    while ((test == 0) && (l < max)) {
        test = abs(konecny_rozklad.part[0].factor[l][0] - neusporadany_rozklad.part[0].factor[l][0]) + abs(konecny_rozklad.part[0].factor[l][1] - neusporadany_rozklad.part[0].factor[l][1]); l++;
    }

    polynomial finalni_vyraz;

    if (test == 0) {

        int puvodni_polynom1[pmax][max1][2];

        for (int jp = 0; jp < pmax; jp++) {
            for (int j = 0; j < max1; j++) {
                puvodni_polynom1[jp][j][0] = 0;
                puvodni_polynom1[jp][j][1] = 0;
            }
        }

        int pom = 0;

        for (int jp = 0; jp < pmax; jp++) {
            for (int i1 = 0; i1 < max; i1++) {
                for (int i2 = 0; i2 < puvodni_polynom[jp][i1][1]; i2++) {
                    puvodni_polynom1[jp][pom + i2][0] = puvodni_polynom[jp][i1][0];
                    puvodni_polynom1[jp][pom + i2][1] = puvodni_polynom1[jp][pom + i2][0] == 0 ? 0 : 1;
                }
                pom += puvodni_polynom[jp][i1][1];
            }
        }

        monomial zero_mono = monomial(0, 0, 0);

        finalni_vyraz = polynomial(monomial(1, soucin1), zero_mono, zero_mono);

    }
*/
    time_t my_time = time(NULL);
    time(&my_time);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "on-%d-%m-at-%H-%M", localtime(&my_time));

    char file_name[30];
    std::sprintf(file_name, "output-%s.txt", buffer);

    FILE* f = fopen(file_name, "w");

    for (int ip = 0; ip < pocet_clenu; ip++) {
        if (koeficient[ip] == -1) { printf(" - "); std::fprintf(f, " - "); }
        if (koeficient[ip] < -1) { printf(" - %u * ", -koeficient[ip]); std::fprintf(f, " - %u * ", -koeficient[ip]); }
        if ((ip == 0) && (koeficient[ip] > 1)) { printf(" %u * ", koeficient[ip]); std::fprintf(f, " %u * ", koeficient[ip]); }
        if (ip > 0) {
            if (koeficient[ip] == 1) { printf(" + "); std::fprintf(f, " + "); }
            if (koeficient[ip] > 1) { printf(" + %u * ", koeficient[ip]); std::fprintf(f, " + %u * ", koeficient[ip]); }
        }
        for (int i1 = 0; i1 < max; i1++) {
            if (puvodni_polynom[ip][i1][0] != 0) {
                string q = unconvert(puvodni_polynom[ip][i1][0]);
                int power = puvodni_polynom[ip][i1][1];
                std::cout << q << "^" << power; std::fprintf(f, "%s", q.c_str()); std::fprintf(f, "^"); std::fprintf(f, "%u", power);
            }
        }
    }

    std::cout << " = "; std::fprintf(f, " = ");
/*
    if (test == 0) {

        int jj = 0;

        while ((finalni_vyraz.part[jj].number_factor != 0) && (jj < max_parts)) {

            int q_factor = finalni_vyraz.part[jj].number_factor;

            if ((jj > 0) && (q_factor > 0)) { std::cout << " + "; std::fprintf(f, " + "); }

            int test = 0;
            if (abs(q_factor) > 1) {
                if (q_factor < 1) {
                    std::cout << " - " << -q_factor << " * ";
                    std::fprintf(f, " - "); std::fprintf(f, "%u", -q_factor); std::fprintf(f, " * ");
                }
                else {
                    std::cout << q_factor << " * ";
                    std::fprintf(f, "%u", q_factor); std::fprintf(f, " * ");
                }
                test = 1;
            }
            if (q_factor == -1) {
                std::cout << " - "; std::fprintf(f, " - "); test = 1;
            }
            if (q_factor == 1) test = 1;

            int power = 0;

            int i1 = 0;

            while ((i1 < max1) && (finalni_vyraz.part[jj].factor[i1][0] != 0)) {
                if (i1 > 0) {
                    if (finalni_vyraz.part[jj].factor[i1][0] != finalni_vyraz.part[jj].factor[i1 - 1][0]) {
                        if (test == 0) { std::cout << " * "; std::fprintf(f, " * "); } test = 0;
                        string q = unconvert(finalni_vyraz.part[jj].factor[i1 - 1][0]);
                        std::cout << q << "^" << power;
                        std::fprintf(f, "%s", q.c_str()); std::fprintf(f, "^"); std::fprintf(f, "%u", power);
                        power = 1;
                    }
                    else power++;
                }
                else power++;
                i1++;
            }

            if (power > 0) {
                if (test == 0) { std::cout << " * "; std::fprintf(f, " * "); }
                string q = unconvert(finalni_vyraz.part[jj].factor[i1 - 1][0]);
                std::cout << q << "^" << power;
                std::fprintf(f, "%s", q.c_str()); std::fprintf(f, "^"); std::fprintf(f, "%u", power);
            }
            jj++;
        }

    }
*/

        int jj = 0;

        int n_factor = konecny_rozklad.part[jj].number_factor;

        while ((n_factor != 0) && (jj < max_parts)) {

            if (n_factor < 0) {
                std::cout << " - "; std::fprintf(f, " - ");
                if (n_factor < -1) {
                    std::cout << -n_factor; std::fprintf(f, "%u", -n_factor);
                }
            }
            if (n_factor > 0) {
                if (jj > 0) { std::cout << " + "; std::fprintf(f, " + "); }
                if (n_factor > 1) { std::cout << n_factor; std::fprintf(f, "%u", n_factor); }
            }

            int i1 = 0;

            int qcode = konecny_rozklad.part[jj].factor[i1][0];

            while (qcode != 0) {

                string q = unconvert(qcode);
                int power = konecny_rozklad.part[jj].factor[i1][1];
                if ((i1 > 0) || (abs(n_factor) > 1)) { printf(" * "); std::fprintf(f, " * "); }
                std::cout << q << "^" << power; std::fprintf(f, "%s", q.c_str()); std::fprintf(f, "^"); std::fprintf(f, "%u", power);

                i1++;
                qcode = konecny_rozklad.part[jj].factor[i1][0];

            }

            jj++;
            n_factor = konecny_rozklad.part[jj].number_factor;

        }

    std::fclose(f);




    std::cout << "\n\n";

    std::system("pause");

    return 0;
}