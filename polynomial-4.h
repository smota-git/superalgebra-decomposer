#include "monomial-4.h"

using namespace std;

extern const int max;
extern const int max1;

extern const int max_parts;

extern const int pmax;

extern const int nn;

class polynomial
{
public:

	inline	polynomial();
	inline	polynomial(monomial the_part[nn]);
	inline	~polynomial();

	inline polynomial(monomial mono0, monomial mono1, monomial mono2);
	inline polynomial(polynomial poly, monomial mono0, monomial mono1);
	inline polynomial(polynomial poly0, polynomial poly1, monomial mono);
	inline polynomial(polynomial poly0, polynomial poly1, polynomial poly2);

	inline void sorted(void);

	inline polynomial transform(int a, int b);
	inline void decompose(void);

	monomial part[nn];
};

polynomial::polynomial()
	: part()

{

}

polynomial::polynomial(monomial the_part[nn])
	: part()

{
	for (int i = 0; i < nn; i++) {
		part[i] = monomial(the_part[i].number_factor, the_part[i].factor);
	}
}

polynomial::polynomial(monomial mono0, monomial mono1, monomial mono2)
{
	part[0] = monomial(mono0.number_factor, mono0.factor);
	part[1] = monomial(mono1.number_factor, mono1.factor);
	part[2] = monomial(mono2.number_factor, mono2.factor);

	monomial zero_mono(0, 0, 0);

	for (int i = 3; i < nn; i++) {
		part[i] = zero_mono;
	}
}

polynomial::polynomial(polynomial poly, monomial mono0, monomial mono1)
{
	int i = 0;

	while (poly.part[i].number_factor != 0) {
		part[i] = monomial(poly.part[i].number_factor, poly.part[i].factor);
		i++;
	}

	part[i] = monomial(mono0.number_factor, mono0.factor); i++;
	part[i] = monomial(mono1.number_factor, mono1.factor); i++;

	monomial zero_mono(0, 0, 0);

	while (i != nn) {
		part[i] = zero_mono;
		i++;
	}
}

polynomial::polynomial(polynomial poly0, polynomial poly1, monomial mono)
{
	int i = 0;

	while (poly0.part[i].number_factor != 0) {
		part[i] = monomial(poly0.part[i].number_factor, poly0.part[i].factor);
		i++;
	}

	int poly0_length = i;

	while (poly1.part[i - poly0_length].number_factor != 0) {
		part[i] = monomial(poly1.part[i - poly0_length].number_factor, poly1.part[i - poly0_length].factor);
		i++;
	}

	part[i] = monomial(mono.number_factor, mono.factor); i++;

	monomial zero_mono(0, 0, 0);

	while (i != nn) {
		part[i] = zero_mono;
		i++;
	}
}

polynomial::polynomial(polynomial poly0, polynomial poly1, polynomial poly2)
{
	int i = 0;

	while (poly0.part[i].number_factor != 0) {
		part[i] = monomial(poly0.part[i].number_factor, poly0.part[i].factor);
		i++;
	}

	int poly0_length = i;

	while (poly1.part[i - poly0_length].number_factor != 0) {
		part[i] = monomial(poly1.part[i - poly0_length].number_factor, poly1.part[i - poly0_length].factor);
		i++;
	}

	int poly0and1_length = i;

	while (poly2.part[i - poly0and1_length].number_factor != 0) {
		part[i] = monomial(poly2.part[i - poly0and1_length].number_factor, poly2.part[i - poly0and1_length].factor);
		i++;
	}

	monomial zero_mono(0, 0, 0);

	while (i != nn) {
		part[i] = zero_mono;
		i++;
	}
}

polynomial::~polynomial()
{

}

void polynomial::sorted(void)
{
	monomial zero_mono(0, 0, 0);

	for (int i = 1; i < nn; i++) {
		if (part[nn - i].number_factor != 0) {

			int j = 1;

			while ((part[nn - i].number_factor != 0) && (j < nn - i + 1)) {

				int k = 0;

				while ((((part[nn - i - j].factor[k][0] == part[nn - i].factor[k][0]) && (part[nn - i - j].factor[k][1] == part[nn - i].factor[k][1])) || ((part[nn - i - j].factor[k][0] == 0) && (part[nn - i].factor[k][0] == 0))) && (k != max)) k++;
				if (k == max) {
					part[nn - i - j].number_factor += part[nn - i].number_factor;
					if (part[nn - i - j].number_factor == 0) part[nn - i - j] = zero_mono;
					part[nn - i] = zero_mono;
				}

				j++;

			}
		}

	}

	int jj = 0;
	while (jj < nn) {
		int jj1 = jj;
		while ((part[jj1].number_factor == 0) || (jj1 == nn)) jj1++;
		if (jj1 < nn) {
			part[jj] = part[jj1];
			if (jj1 > jj) part[jj1] = zero_mono;
			jj++;
		}
		else jj = nn;
	}

}

polynomial polynomial::transform(int a, int b)
{
	monomial zero_mono(0, 0, 0);

	if ((a == 1) && (b == 2)) { return  polynomial(monomial(1, 2, 1), zero_mono, zero_mono); } // (E12*E13);
	if ((a == 1) && (b == 3)) { return  polynomial(monomial(1, 3, 1), zero_mono, zero_mono); } // (E23*E13);
	if ((a == 1) && (b == 4)) { return  polynomial(monomial(1, 4, 1), monomial(-2, 1, 0), zero_mono); } // (H23*E13-2E13);
	if ((a == 1) && (b == 5)) { return  polynomial(monomial(1, 5, 1), monomial(-1, 1, 0), zero_mono); } // (H12 * E13 - E13);
	if ((a == 1) && (b == 6)) { return  polynomial(monomial(1, 6, 1), monomial(1, 2, 0), zero_mono); } // (E32 * E13 + E12);
	if ((a == 1) && (b == 7)) { return  polynomial(monomial(1, 7, 1), monomial(-1, 3, 0), zero_mono); } // (E21 * E13 - E23);
	if ((a == 1) && (b == 8)) { return  polynomial(monomial(1, 8, 1), monomial(1, 4, 0), zero_mono); } // (E31 * E13 + H23);
	if ((a == 2) && (b == 3)) { return  polynomial(monomial(1, 3, 2), monomial(1, 1, 0), zero_mono); } // (E23 * E12 + E13);
	if ((a == 2) && (b == 4)) { return  polynomial(monomial(1, 4, 2), monomial(-1, 2, 0), zero_mono); } // (H23 * E12 - E12);
	if ((a == 2) && (b == 5)) { return  polynomial(monomial(1, 5, 2), monomial(-2, 2, 0), zero_mono); } // (H12 * E12 - 2E12);
	if ((a == 2) && (b == 6)) { return  polynomial(monomial(1, 6, 2), zero_mono, zero_mono); } // (E32*E12);
	if ((a == 2) && (b == 7)) { return  polynomial(monomial(1, 7, 2), monomial(1, 5, 0), zero_mono); } // (E21 * E12 + H12);
	if ((a == 2) && (b == 8)) { return  polynomial(monomial(1, 8, 2), monomial(-1, 6, 0), zero_mono); } // (E31 * E12 - E32);
	if ((a == 3) && (b == 4)) { return  polynomial(monomial(1, 4, 3), monomial(-1, 3, 0), zero_mono); } // (H23 * E23 - E23);
	if ((a == 3) && (b == 5)) { return  polynomial(monomial(1, 5, 3), monomial(1, 3, 0), zero_mono); } // (H12 * E23 + E23);
	if ((a == 3) && (b == 6)) { return  polynomial(monomial(1, 6, 3), monomial(1, 5, 0), monomial(1, 4, 0)); } // (E32 * E23 + H12 + H23);
	if ((a == 3) && (b == 7)) { return  polynomial(monomial(1, 7, 3), zero_mono, zero_mono); } // (E21 * E23);
	if ((a == 3) && (b == 8)) { return  polynomial(monomial(1, 8, 3), monomial(1, 7, 0), zero_mono); } // (E31 * E23 + E21);
	if ((a == 4) && (b == 5)) { return  polynomial(monomial(1, 5, 4), zero_mono, zero_mono); } // (H12 * H23);
	if ((a == 4) && (b == 6)) { return  polynomial(monomial(1, 6, 4), monomial(-1, 6, 0), zero_mono); } // (E32 * H23 - E32);
	if ((a == 4) && (b == 7)) { return  polynomial(monomial(1, 7, 4), monomial(-1, 7, 0), zero_mono); } // (E21 * H23 - E21);
	if ((a == 4) && (b == 8)) { return  polynomial(monomial(1, 8, 4), monomial(-2, 8, 0), zero_mono); } // (E31 * H23 - 2E31);
	if ((a == 5) && (b == 6)) { return  polynomial(monomial(1, 6, 5), monomial(1, 6, 0), zero_mono); } // (E32 * H12 + E32);
	if ((a == 5) && (b == 7)) { return  polynomial(monomial(1, 7, 5), monomial(-2, 7, 0), zero_mono); } // (E21 * H12 - 2E21);
	if ((a == 5) && (b == 8)) { return  polynomial(monomial(1, 8, 5), monomial(-1, 8, 0), zero_mono); } // (E31 * H12 - E31);
	if ((a == 6) && (b == 7)) { return  polynomial(monomial(1, 7, 6), monomial(1, 8, 0), zero_mono); } // (E21 * E32 + E31);
	if ((a == 6) && (b == 8)) { return  polynomial(monomial(1, 8, 6), zero_mono, zero_mono); } // (E31 * E32);
	if ((a == 7) && (b == 8)) { return  polynomial(monomial(1, 8, 7), zero_mono, zero_mono); } // (E31 * E21);
}

//polynomial polynomial::decompose(void)
void polynomial::decompose(void)
{
	for (int j = 1; j < max; j++) {
		if ((part[0].factor[max - j][0] > 0) && (part[0].factor[max - j][1] > 0)) {
			if (part[0].factor[max - j][0] > part[0].factor[max - j - 1][0]) {
				polynomial ordered_pair = transform(part[0].factor[max - j - 1][0], part[0].factor[max - j][0]);

				monomial zero_mono(0, 0, 0);

				int the_number_factor1 = ordered_pair.part[0].number_factor * part[0].number_factor;

				int the_prefactor1[max + 2][2];

				for (int ii = 0; ii < max - j - 1; ii++) {
					the_prefactor1[ii][0] = part[0].factor[ii][0];
					the_prefactor1[ii][1] = part[0].factor[ii][1];
				}

				the_prefactor1[max - j - 1][1] = part[0].factor[max - j - 1][1] - 1; the_prefactor1[max - j - 1][0] = (the_prefactor1[max - j - 1][1] == 0) ? 0 : part[0].factor[max - j - 1][0];
				the_prefactor1[max - j][0] = ordered_pair.part[0].factor[0][0]; the_prefactor1[max - j][1] = 1;
				the_prefactor1[max - j + 1][0] = ordered_pair.part[0].factor[1][0]; the_prefactor1[max - j + 1][1] = 1;
				the_prefactor1[max - j + 2][1] = part[0].factor[max - j][1] - 1; the_prefactor1[max - j + 2][0] = (the_prefactor1[max - j + 2][1] == 0) ? 0 : part[0].factor[max - j][0];

				for (int ii = max - j + 1; ii < max; ii++) {
					the_prefactor1[ii + 2][0] = part[0].factor[ii][0];
					the_prefactor1[ii + 2][1] = part[0].factor[ii][1];
				}

				int jj = 0;
				while (jj < max + 2) {
					int jj1 = jj;
					while ((the_prefactor1[jj1][0] == 0) || (the_prefactor1[jj1][1] == 0) || (jj1 == max + 2)) jj1++;
					if (jj1 < max + 2) {
						if (the_prefactor1[jj1][0] == the_prefactor1[jj - 1][0]) {
							the_prefactor1[jj - 1][1] += the_prefactor1[jj1][1];
							the_prefactor1[jj1][0] = 0;
							the_prefactor1[jj1][1] = 0;
							if (the_prefactor1[jj - 1][1] == 0) {
								the_prefactor1[jj - 1][0] = 0;
								jj--;
							}
						}
						else {
							if (jj1 > jj) {
								the_prefactor1[jj][0] = the_prefactor1[jj1][0];
								the_prefactor1[jj][1] = the_prefactor1[jj1][1];
								the_prefactor1[jj1][0] = 0;
								the_prefactor1[jj1][1] = 0;
							}
							jj++;
						}
					}
					else jj = max1;
				}

				int the_factor1[max][2];

				for (int ii = 0; ii < max; ii++) {
					the_factor1[ii][0] = the_prefactor1[ii][0];
					the_factor1[ii][1] = the_prefactor1[ii][1];
				}

				polynomial newpol1 = polynomial(monomial(the_number_factor1, the_factor1), zero_mono, zero_mono);
				newpol1.decompose();
				if (ordered_pair.part[1].number_factor == 0) {
					newpol1.sorted();
					//return newpol1;
					*this = polynomial(newpol1, zero_mono, zero_mono);
				}
				else {
					int the_number_factor2 = ordered_pair.part[1].number_factor * part[0].number_factor;

					int the_prefactor2[max + 2][2];

					for (int ii = 0; ii < max - j - 1; ii++) {
						the_prefactor2[ii][0] = part[0].factor[ii][0];
						the_prefactor2[ii][1] = part[0].factor[ii][1];
					}

					the_prefactor2[max - j - 1][1] = part[0].factor[max - j - 1][1] - 1; the_prefactor2[max - j - 1][0] = (the_prefactor2[max - j - 1][1] == 0) ? 0 : part[0].factor[max - j - 1][0];
					the_prefactor2[max - j][0] = ordered_pair.part[1].factor[0][0]; the_prefactor2[max - j][1] = 1;
					the_prefactor2[max - j + 1][0] = ordered_pair.part[1].factor[1][0]; the_prefactor2[max - j + 1][1] = 1;
					the_prefactor2[max - j + 2][1] = part[0].factor[max - j][1] - 1; the_prefactor2[max - j + 2][0] = (the_prefactor2[max - j + 2][1] == 0) ? 0 : part[0].factor[max - j][0];

					for (int ii = max - j + 1; ii < max; ii++) {
						the_prefactor2[ii + 2][0] = part[0].factor[ii][0];
						the_prefactor2[ii + 2][1] = part[0].factor[ii][1];
					}

					int jj = 0;
					while (jj < max + 2) {
						int jj1 = jj;
						while ((the_prefactor2[jj1][0] == 0) || (the_prefactor2[jj1][1] == 0) || (jj1 == max + 2)) jj1++;
						if (jj1 < max + 2) {
							if (the_prefactor2[jj1][0] == the_prefactor2[jj - 1][0]) {
								the_prefactor2[jj - 1][1] += the_prefactor2[jj1][1];
								the_prefactor2[jj1][0] = 0;
								the_prefactor2[jj1][1] = 0;
								if (the_prefactor2[jj - 1][1] == 0) {
									the_prefactor2[jj - 1][0] = 0;
									jj--;
								}
							}
							else {
								if (jj1 > jj) {
									the_prefactor2[jj][0] = the_prefactor2[jj1][0];
									the_prefactor2[jj][1] = the_prefactor2[jj1][1];
									the_prefactor2[jj1][0] = 0;
									the_prefactor2[jj1][1] = 0;
								}
								jj++;
							}
						}
						else jj = max1;
					}

					int the_factor2[max][2];

					for (int ii = 0; ii < max; ii++) {
						the_factor2[ii][0] = the_prefactor2[ii][0];
						the_factor2[ii][1] = the_prefactor2[ii][1];
					}

					polynomial newpol2 = polynomial(monomial(the_number_factor2, the_factor2), zero_mono, zero_mono);
					newpol2.decompose();
					if (ordered_pair.part[2].number_factor == 0) {
						polynomial newpol12(newpol1, newpol2, zero_mono);
						newpol12.sorted();
						//return newpol12;
						*this = polynomial(newpol12, zero_mono, zero_mono);
					}
					else {
						int the_number_factor3 = ordered_pair.part[2].number_factor * part[0].number_factor;

						int the_prefactor3[max + 2][2];

						for (int ii = 0; ii < max - j - 1; ii++) {
							the_prefactor3[ii][0] = part[0].factor[ii][0];
							the_prefactor3[ii][1] = part[0].factor[ii][1];
						}

						the_prefactor3[max - j - 1][1] = part[0].factor[max - j - 1][1] - 1; the_prefactor3[max - j - 1][0] = (the_prefactor3[max - j - 1][1] == 0) ? 0 : part[0].factor[max - j - 1][0];
						the_prefactor3[max - j][0] = ordered_pair.part[2].factor[0][0]; the_prefactor3[max - j][1] = 1;
						the_prefactor3[max - j + 1][0] = ordered_pair.part[3].factor[1][0]; the_prefactor3[max - j + 1][1] = 1;
						the_prefactor3[max - j + 2][1] = part[0].factor[max - j][1] - 1; the_prefactor3[max - j + 2][0] = (the_prefactor3[max - j + 2][1] == 0) ? 0 : part[0].factor[max - j][0];

						for (int ii = max - j + 1; ii < max; ii++) {
							the_prefactor3[ii + 2][0] = part[0].factor[ii][0];
							the_prefactor3[ii + 2][1] = part[0].factor[ii][1];
						}

						int jj = 0;
						while (jj < max + 2) {
							int jj1 = jj;
							while ((the_prefactor3[jj1][0] == 0) || (the_prefactor3[jj1][1] == 0) || (jj1 == max + 2)) jj1++;
							if (jj1 < max + 2) {
								if (the_prefactor3[jj1][0] == the_prefactor3[jj - 1][0]) {
									the_prefactor3[jj - 1][1] += the_prefactor3[jj1][1];
									the_prefactor3[jj1][0] = 0;
									the_prefactor3[jj1][1] = 0;
									if (the_prefactor3[jj - 1][1] == 0) {
										the_prefactor3[jj - 1][0] = 0;
										jj--;
									}
								}
								else {
									if (jj1 > jj) {
										the_prefactor3[jj][0] = the_prefactor3[jj1][0];
										the_prefactor3[jj][1] = the_prefactor3[jj1][1];
										the_prefactor3[jj1][0] = 0;
										the_prefactor3[jj1][1] = 0;
									}
									jj++;
								}
							}
							else jj = max1;
						}

						int the_factor3[max][2];

						for (int ii = 0; ii < max; ii++) {
							the_factor3[ii][0] = the_prefactor3[ii][0];
							the_factor3[ii][1] = the_prefactor3[ii][1];
						}

						polynomial newpol3 = polynomial(monomial(the_number_factor3, the_factor3), zero_mono, zero_mono);
						newpol3.decompose();
						polynomial newpol123(newpol1, newpol2, newpol3);
						newpol123.sorted();
						//return newpol123;
						*this = polynomial(newpol123, zero_mono, zero_mono);
					}
				}
			}
		}

	}
}

