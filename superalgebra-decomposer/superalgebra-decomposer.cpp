 /*
  * superalgebra-decomposer.cpp
  *
  * Iterative symbolic polynomial normalizer.
  *
  * The program transforms arbitrary polynomial expressions
  * in the generators
  *
  *     E31 E21 E32 H12 H23 E23 E12 E13
  *
  * into a canonical ordering using predefined commutation
  * relations of the underlying Lie algebra.
  *
  * Besides the final result, the program can optionally
  * generate a complete step-by-step derivation in plain text
  * and LaTeX/PDF.
  */

#include <iostream>
#include <string>

#include <fstream>
#include <sstream>

#include <limits>

#include <time.h>

#include "polynomial.h"

using namespace std;

extern const int monom_length;

extern const int poly_length_init;

extern const int p_len_reserved;

/// <summary>
/// conversion of generator names to internal indices 
/// </summary>
/// <param name="element">generator name; 
/// H13 has a special meaning - it is not a basis element and is later completely expanded</param>
/// <returns>internal index of the generator (9 stands for H13)</returns>
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

/// <summary>
/// conversion of intenal indices back to generator names
/// </summary>
/// <param name="id">internal generator index</param>
/// <returns>generator name corresponding to the given index</returns>
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

/// <summary>
/// output of current polynomial to console, text file and optionally LaTeX 
/// </summary>
/// <param name="konecny_rozklad">current polynomial</param>
/// <param name="f_txt">pointer to output text file</param>
/// <param name="f_tex">pointer to output tex file (later converted to pdf)</param>
/// <param name="texmode">enables or disables LaTeX output</param>
void vypis(polynomial konecny_rozklad, FILE* f_txt, FILE* f_tex, bool texmode) {

    int jj = 0;

    int n_factor;

    cout << "\n\n"; fprintf(f_txt, "\n\n");

    while ((jj < p_len_reserved) && (konecny_rozklad.part[jj].number_factor != 0)) {

        n_factor = konecny_rozklad.part[jj].number_factor;

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
    }

    if (texmode) {
 
        jj = 0;

        fprintf(f_tex, "\\begin{dmath*}\n");

        while ((jj < p_len_reserved) && (konecny_rozklad.part[jj].number_factor != 0)) {

            n_factor = konecny_rozklad.part[jj].number_factor;

            if (n_factor < 0) {
                fprintf(f_tex, " - ");
                if (n_factor < -1) fprintf(f_tex, "%u", -n_factor);
            }
            if (n_factor > 0) {
                if (jj > 0) fprintf(f_tex, " + ");
                if (n_factor > 1) fprintf(f_tex, "%u", n_factor);
            }

            int i1 = 0;

            int qcode = konecny_rozklad.part[jj].factor[i1][0];

            while (qcode != 0) {

                string q = unconvert(qcode);
                string q_tex = q.substr(0, 1) + "_{" + q.substr(1, 2) + "}";
                int power = konecny_rozklad.part[jj].factor[i1][1];
                if ((i1 > 0) || (abs(n_factor) > 1)) fprintf(f_tex, " \\cdot ");
                fprintf(f_tex, "%s", q_tex.c_str());
                if (power > 1) fprintf(f_tex, "^{%u}", power);
                i1++;
                qcode = konecny_rozklad.part[jj].factor[i1][0];

            }

            jj++;
        }

        fprintf(f_tex, "\n\\end{dmath*}\n");
    }
}

/// <summary>
/// main block: we choose between keyboard or (early prepared) text file input and between extraction/non-extraction the result to tex file
/// and in case of keyboard input, we assign the form of the input polynomial, appropriate monomials and values of their factors; on this base,
/// the algorithm generates using header files polynomial.h and monomial.h the input polynomial and using the methods in polynomial.h (sorted(), 
/// transform(), decompose(), decompose_part()), it modifies the form of the input polynomial, unless the order of elements in monomials 
/// satisfies the descending sequence given by the appropriate numerical values in functions convert() and unconvert(), resp.;
/// each intermediate polynomial is written to the console and output files using function vypis()
/// </summary>
/// <returns></returns>
int main() {

    printf("Budete zadavat polynom z klavesnice nebo ze souboru (1 - klavesnice, 2 - soubor )? ");

    int mode;
    cin >> mode;
    printf("\n");

    printf("Budete vystup zapisovat do texu (a/n)? ");

    bool texmode = true;
    string fillmode;
    cin >> fillmode;
    if (fillmode.compare("n") == 0) texmode = false;
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

    time_t my_time = time(NULL);
    time(&my_time);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "file-%d-%m-at-%H-%M-%S", localtime(&my_time));

    char output_txt[30];
    sprintf(output_txt, "%s.txt", buffer);
    FILE* f_txt = fopen(output_txt, "w");

    char output_tex[30];
    FILE* f_tex = nullptr;

    if (texmode) {
        sprintf(output_tex, "%s.tex", buffer);
        f_tex = fopen(output_tex, "w");
 
        fprintf(f_tex, "\\documentclass{article}\n");
        fprintf(f_tex, "\\usepackage{breqn}\n\n");
        fprintf(f_tex, "\\begin{document}\n");
        fprintf(f_tex, "\\begin{dmath*}\n");
    }

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

        if (texmode) {

            if (koeficient[ip] == -1) fprintf(f_tex, " - ");
            if (koeficient[ip] < -1) fprintf(f_tex, " - %u \\cdot ", -koeficient[ip]);
            if ((ip == 0) && (koeficient[ip] > 1)) fprintf(f_tex, " %u \\cdot ", koeficient[ip]);
            if (ip > 0) {
                if (koeficient[ip] == 1) fprintf(f_tex, " + ");
                if (koeficient[ip] > 1) fprintf(f_tex, " + %u \\cdot ", koeficient[ip]);
            }
            for (int i1 = 0; i1 < monom_length; i1++) {
                if (puvodni_polynom[ip][i1][0] != 0) {
                    string q = unconvert(puvodni_polynom[ip][i1][0]);
                    string q_tex = q.substr(0, 1) + "_{" + q.substr(1, 2) + "}";
                    int power = puvodni_polynom[ip][i1][1];
                    fprintf(f_tex, "%s", q_tex.c_str());
                    if (power > 1) {
                        fprintf(f_tex, "^"); fprintf(f_tex, "{%u}", power);
                    }
                    if (puvodni_polynom[ip][i1 + 1][0] != 0) {
                        fprintf(f_tex, " \\cdot ");
                    }
                }
            }
        }
    }

    cout << " = "; fprintf(f_txt, " = ");

    if (texmode) {
        fprintf(f_tex, "\n\\end{dmath*}\n");
        fprintf(f_tex, "\\vspace{2mm}\n\\begin{center}$=$\\end{center}\n\\vspace{2mm}\n");
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

        rozklad_soucinu = rozklad_soucinu.decompose();

        neusporadany_rozklad = polynomial(neusporadany_rozklad, rozklad_soucinu, zero_mono);
    }

    polynomial konecny_rozklad = neusporadany_rozklad;

    vypis(konecny_rozklad, f_txt, f_tex, texmode);

    polynomial konecny_rozklad_init = konecny_rozklad;
    konecny_rozklad.sorted();

    int i = 0;
    bool ident_poly = true;
    while (ident_poly && i < p_len_reserved) {
        ident_poly = konecny_rozklad_init.part[i].number_factor - konecny_rozklad.part[i].number_factor == 0;
        i++;
    }

    if (!ident_poly) {
        cout << " = "; fprintf(f_txt, " = ");
        if (texmode) fprintf(f_tex, "\\vspace{2mm}\n\\begin{center}$=$\\end{center}\n\\vspace{2mm}\n");
        vypis(konecny_rozklad, f_txt, f_tex, texmode);
    }

    neusporadany_rozklad = polynomial(zero_mono, zero_mono, zero_mono);

    int ip = 0;
    bool well_sorted = false;

    while (! well_sorted) {

        ip = 0;

        while (konecny_rozklad.part[ip].number_factor != 0) {

            polynomial rozklad_soucinu = polynomial(konecny_rozklad.part[ip], zero_mono, zero_mono);

            rozklad_soucinu = rozklad_soucinu.decompose();

            neusporadany_rozklad = polynomial(neusporadany_rozklad, rozklad_soucinu, zero_mono);

            ip++;
        }

        konecny_rozklad = neusporadany_rozklad;

        cout << " = "; fprintf(f_txt, " = ");
        if (texmode) fprintf(f_tex, "\\vspace{2mm}\n\\begin{center}$=$\\end{center}\n\\vspace{2mm}\n");

        vypis(konecny_rozklad, f_txt, f_tex, texmode);

        polynomial konecny_rozklad_init = konecny_rozklad;
        konecny_rozklad.sorted();

        int i = 0;
        bool ident_poly = true;
        while (ident_poly && i < p_len_reserved) {
            ident_poly = konecny_rozklad_init.part[i].number_factor - konecny_rozklad.part[i].number_factor == 0;
            i++;
        }

        if (!ident_poly) {
            cout << " = "; fprintf(f_txt, " = ");
            if (texmode) fprintf(f_tex, "\\vspace{2mm}\n\\begin{center}$=$\\end{center}\n\\vspace{2mm}\n");
            vypis(konecny_rozklad, f_txt, f_tex, texmode);
        }

        neusporadany_rozklad = polynomial(zero_mono, zero_mono, zero_mono);

        ip = 0;

        well_sorted = true;

        while (ip < p_len_reserved && well_sorted && konecny_rozklad.part[ip].number_factor != 0) {

            int n = 0;

            int q = konecny_rozklad.part[ip].factor[n][0];

            while (well_sorted && q != 0 && n < monom_length) {

                int q1 = konecny_rozklad.part[ip].factor[n + 1][0];

                well_sorted = q != 9 && q > q1;

                n++;
                q = q1;
            }

            ip++;
        }
    }

    fclose(f_txt);

    if (texmode) {

        printf("\n\nStiskni Enter pro zapis do texu...\n");
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        fprintf(f_tex, "\\end{document}\n");
        fclose(f_tex);

        char command[300];
        sprintf(command, "pdflatex -interaction=nonstopmode %s", output_tex);
        system(command);
    }

    cout << "\n\n";

    system("pause");

    return 0;
}