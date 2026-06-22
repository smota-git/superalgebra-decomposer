/*
 * superalgebra-decomposer-rec.cpp
 *
 * Recursive version of the normal form decomposer.
 * Kept as an alternative implementation and for comparison
 * with the iterative version.
 */

#include <iostream>
#include <string>

#include <fstream>
#include <sstream>

#include <time.h>

#include "polynomial.h"

using namespace std;

extern const int monom_length;

extern const int poly_length_init;

extern const int p_len_reserved;

int convert(string element) {

    int value = 0;

    if (element.compare("H13") == 0) value = 9;

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

    if (id == 9) return "H13";

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

    printf("Budete zadavat polynom z klavesnice nebo ze souboru (1 - klavesnice, 2 - soubor )? ");

    int mode;
    cin >> mode;
    printf("\n");

    int pocet_clenu;
    int puvodni_polynom[poly_length_init][monom_length + 2][2];
    int koeficient[poly_length_init];

    for (int ip = 0; ip < poly_length_init; ip++) {
        for (int i1 = 0; i1 < monom_length + 2; i1++) {
            for (int i2 = 0; i2 < 2; i2++) {
                puvodni_polynom[ip][i1][i2] = 0;
            }
        }
    }

    if (mode == 1) {

        printf("Zadej pocet clenu polynomu (max = %u): ", poly_length_init);

        cin >> pocet_clenu;

        printf("\n");

        for (int ip = 0; ip < pocet_clenu; ip++) {

            printf("Zadej pocet prvku %u.soucinu (max = %u): ", ip + 1, monom_length);

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
    }

    if (mode == 2) {

        ifstream file("input.txt");

        if (!file) {
            cout << "Soubor se nepodarilo otevrit.\n";
            return 1;
        }

        pocet_clenu = 0;

        string line;

        while (getline(file, line)) {

            pocet_clenu++;

            if (pocet_clenu >= poly_length_init) {
                cout << "Prilis mnoho clenu polynomu.\n";
                return 1;
            }

            stringstream ss(line);

            ss >> koeficient[pocet_clenu - 1];

            string choice;
            int power;
            int pocet_prvku = 0;

            while (ss >> choice >> power) {
                pocet_prvku++;
                puvodni_polynom[pocet_clenu - 1][pocet_prvku - 1][0] = convert(choice);
                puvodni_polynom[pocet_clenu - 1][pocet_prvku - 1][1] = power;
            }
        }
    }

    monomial zero_mono = monomial(0, 0, 0);

    polynomial neusporadany_rozklad = polynomial(zero_mono, zero_mono, zero_mono);

    for (int ip = 0; ip < pocet_clenu; ip++) {

        int soucin[monom_length + 2][2];

        for (int i1 = 0; i1 < monom_length + 2; i1++) {
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



    time_t my_time = time(NULL);
    time(&my_time);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "file-%d-%m-at-%H-%M-%S", localtime(&my_time));

    char output_txt[30];
    sprintf(output_txt, "%s.txt", buffer);
    FILE* f_txt = fopen(output_txt, "w");

    for (int ip = 0; ip < pocet_clenu; ip++) {
        if (koeficient[ip] == -1) { printf(" - "); fprintf(f_txt, " - "); }
        if (koeficient[ip] < -1) { printf(" - %u * ", -koeficient[ip]); fprintf(f_txt, " - %u * ", -koeficient[ip]); }
        if ((ip == 0) && (koeficient[ip] > 1)) { printf(" %u * ", koeficient[ip]); fprintf(f_txt, " %u * ", koeficient[ip]); }
        if (ip > 0) {
            if (koeficient[ip] == 1) { printf(" + "); fprintf(f_txt, " + "); }
            if (koeficient[ip] > 1) { printf(" + %u * ", koeficient[ip]); fprintf(f_txt, " + %u * ", koeficient[ip]); }
        }
        for (int i1 = 0; i1 < monom_length; i1++) {
            if (puvodni_polynom[ip][i1][0] != 0) {
                string q = unconvert(puvodni_polynom[ip][i1][0]);
                int power = puvodni_polynom[ip][i1][1];
                cout << q; fprintf(f_txt, "%s", q.c_str());
                if (power > 1) {
                    cout << "^" << power; fprintf(f_txt, "^"); fprintf(f_txt, "%u", power);
                }
                if (puvodni_polynom[ip][i1 + 1][0] != 0) {
                    cout << " * "; fprintf(f_txt, " * ");
                }
            }
        }
    }

    cout << " = "; fprintf(f_txt, " = ");


    int jj = 0;

    int n_factor = konecny_rozklad.part[jj].number_factor;

    while ((n_factor != 0) && (jj < p_len_reserved)) {

        if (n_factor < 0) {
            cout << " - "; fprintf(f_txt, " - ");
            if (n_factor < -1) {
                cout << -n_factor; fprintf(f_txt, "%u", -n_factor);
            }
        }
        if (n_factor > 0) {
            if (jj > 0) { cout << " + "; fprintf(f_txt, " + "); }
            if (n_factor > 1) { cout << n_factor; fprintf(f_txt, "%u", n_factor); }
        }

        int i1 = 0;

        int qcode = konecny_rozklad.part[jj].factor[i1][0];

        while (qcode != 0) {

            string q = unconvert(qcode);
            int power = konecny_rozklad.part[jj].factor[i1][1];
            if ((i1 > 0) || (abs(n_factor) > 1)) { printf(" * "); fprintf(f_txt, " * "); }
            cout << q; fprintf(f_txt, "%s", q.c_str());
            if (power > 1) {
                cout << "^" << power; fprintf(f_txt, "^"); fprintf(f_txt, "%u", power);
            }
            i1++;
            qcode = konecny_rozklad.part[jj].factor[i1][0];

        }

        jj++;
        n_factor = konecny_rozklad.part[jj].number_factor;

    }

    fclose(f_txt);

    cout << "\n\n";

    system("pause");

    return 0;
}