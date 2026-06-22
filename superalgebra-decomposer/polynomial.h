/// <summary>
/// polynomial represented as a fixed-size array of monomials, provides decomposition, simplification and sorting routines;
/// the definitions of methods decompose() and decompose_part() differ for iterative and recursive algorithm, so,
/// for corresponding choice, the rows in class, decompose() and decompose_part() block must be appropriately
/// commented/uncommented
/// </summary>

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "monomial.h"

#endif

using namespace std;

extern const int monom_length;

extern const int p_len_reserved;

class polynomial
{
public:

	inline	polynomial();
	inline	polynomial(monomial the_part[p_len_reserved]);
	inline	~polynomial();

	inline polynomial(monomial mono0, monomial mono1, monomial mono2);
	inline polynomial(polynomial poly, monomial mono0, monomial mono1);
	inline polynomial(polynomial poly0, polynomial poly1, monomial mono);
	inline polynomial(polynomial poly0, polynomial poly1, polynomial poly2);

	inline void sorted(void);

	inline polynomial transform(int a, int b);

	//choice between versions for recursive and iterative algorithm:
	
	//iterative version
	inline polynomial decompose(void);
	//recursive version	
	//inline void decompose(void);

	inline polynomial decompose_part(monomial ordered_pair_part, int j);

	monomial part[p_len_reserved];
};

polynomial::polynomial()
	: part()

{

}

polynomial::polynomial(monomial the_part[p_len_reserved])
	: part()

{
	for (int i = 0; i < p_len_reserved; i++) {
		part[i] = monomial(the_part[i].number_factor, the_part[i].factor);
	}
}

polynomial::polynomial(monomial mono0, monomial mono1, monomial mono2)
{
	part[0] = monomial(mono0.number_factor, mono0.factor);
	part[1] = monomial(mono1.number_factor, mono1.factor);
	part[2] = monomial(mono2.number_factor, mono2.factor);

	monomial zero_mono(0, 0, 0);

	for (int i = 3; i < p_len_reserved; i++) {
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

	while (i != p_len_reserved) {
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

	while (i != p_len_reserved) {
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

	while (i != p_len_reserved) {
		part[i] = zero_mono;
		i++;
	}
}

polynomial::~polynomial()
{

}

/// <summary>
/// summation of identical monomials and moving zero monomials to the end of the polynomial
/// </summary>
void polynomial::sorted(void)
{
	monomial zero_mono(0, 0, 0);

	for (int i = 1; i < p_len_reserved; i++) {
		if (part[p_len_reserved - i].number_factor != 0) {

			int j = 1;

			while ((part[p_len_reserved - i].number_factor != 0) && (j < p_len_reserved - i + 1)) {

				int k = 0;

				while ( ( ( (part[p_len_reserved - i - j].factor[k][0] == part[p_len_reserved - i].factor[k][0]) && 
					(part[p_len_reserved - i - j].factor[k][1] == part[p_len_reserved - i].factor[k][1]) ) || 
					( (part[p_len_reserved - i - j].factor[k][0] == 0) && (part[p_len_reserved - i].factor[k][0] == 0) ) ) && 
					(k != monom_length) ) k++;
				if (k == monom_length) {
					part[p_len_reserved - i - j].number_factor += part[p_len_reserved - i].number_factor;
					if (part[p_len_reserved - i - j].number_factor == 0) part[p_len_reserved - i - j] = zero_mono;
					part[p_len_reserved - i] = zero_mono;
				}

				j++;

			}
		}

	}

	int jj = 0;
	while (jj < p_len_reserved) {
		int jj1 = jj;
		while ((jj1 < p_len_reserved) && (part[jj1].number_factor == 0)) jj1++;
		if (jj1 < p_len_reserved) {
			part[jj] = part[jj1];
			if (jj1 > jj) part[jj1] = zero_mono;
			jj++;
		}
		else jj = p_len_reserved;
	}

}

/// <summary>
/// transformation of one unordered pair of generators (in fact, together with the list of generators
/// in functions convert() and unconvert(), they define the algebra)
/// </summary>
/// <param name="a">first element of pair</param>
/// <param name="b">second element of pair</param>
/// <returns>polynomial representing the transformed pair</returns>
polynomial polynomial::transform(int a, int b)
{
	monomial zero_mono(0, 0, 0);

	// H13 = H12 + H23;
	if (a == 9) { return  polynomial(monomial(1, 5, b), monomial(1, 4, b), zero_mono); }
	if (b == 9) { return  polynomial(monomial(1, a, 5), monomial(1, a, 4), zero_mono); }

	if ((a == 1) && (b == 2)) { return  polynomial(monomial(1, 2, 1), zero_mono, zero_mono); } // E13 * E12 = E12 * E13;
	if ((a == 1) && (b == 3)) { return  polynomial(monomial(1, 3, 1), zero_mono, zero_mono); } // E13 * E23 = E23 * E13;
	if ((a == 1) && (b == 4)) { return  polynomial(monomial(1, 4, 1), monomial(-2, 1, 0), zero_mono); } // E13 * H23 = H23 * E13 - 2E13;
	if ((a == 1) && (b == 5)) { return  polynomial(monomial(1, 5, 1), monomial(-1, 1, 0), zero_mono); } // E13 * H12 = H12 * E13 - E13;
	if ((a == 1) && (b == 6)) { return  polynomial(monomial(1, 6, 1), monomial(1, 2, 0), zero_mono); } // E13 * E32 = E32 * E13 + E12;
	if ((a == 1) && (b == 7)) { return  polynomial(monomial(1, 7, 1), monomial(-1, 3, 0), zero_mono); } // E13 * E21 = E21 * E13 - E23;
	if ((a == 1) && (b == 8)) { return  polynomial(monomial(1, 8, 1), monomial(1, 4, 0), zero_mono); } // E13 * E31 = E31 * E13 + H23;
	if ((a == 2) && (b == 3)) { return  polynomial(monomial(1, 3, 2), monomial(1, 1, 0), zero_mono); } // E12 * E23 = E23 * E12 + E13;
	if ((a == 2) && (b == 4)) { return  polynomial(monomial(1, 4, 2), monomial(-1, 2, 0), zero_mono); } // E12 * H23 = H23 * E12 - E12;
	if ((a == 2) && (b == 5)) { return  polynomial(monomial(1, 5, 2), monomial(-2, 2, 0), zero_mono); } // E12 * H12 = H12 * E12 - 2E12;
	if ((a == 2) && (b == 6)) { return  polynomial(monomial(1, 6, 2), zero_mono, zero_mono); } // E12 * E32 = E32 * E12;
	if ((a == 2) && (b == 7)) { return  polynomial(monomial(1, 7, 2), monomial(1, 5, 0), zero_mono); } // E12 * E21 = E21 * E12 + H12;
	if ((a == 2) && (b == 8)) { return  polynomial(monomial(1, 8, 2), monomial(-1, 6, 0), zero_mono); } // E12 * E31 = E31 * E12 - E32;
	if ((a == 3) && (b == 4)) { return  polynomial(monomial(1, 4, 3), monomial(-1, 3, 0), zero_mono); } // E23 * H23 = H23 * E23 - E23;
	if ((a == 3) && (b == 5)) { return  polynomial(monomial(1, 5, 3), monomial(1, 3, 0), zero_mono); } // E23 * H12 = H12 * E23 + E23;
	if ((a == 3) && (b == 6)) { return  polynomial(monomial(1, 6, 3), monomial(1, 5, 0), monomial(1, 4, 0)); } // E23 * E32 = E32 * E23 + H12 + H23;
	if ((a == 3) && (b == 7)) { return  polynomial(monomial(1, 7, 3), zero_mono, zero_mono); } // E23 * E21 = E21 * E23;
	if ((a == 3) && (b == 8)) { return  polynomial(monomial(1, 8, 3), monomial(1, 7, 0), zero_mono); } // E23 * E31 = E31 * E23 + E21;
	if ((a == 4) && (b == 5)) { return  polynomial(monomial(1, 5, 4), zero_mono, zero_mono); } // H23 * H12 = H12 * H23;
	if ((a == 4) && (b == 6)) { return  polynomial(monomial(1, 6, 4), monomial(-1, 6, 0), zero_mono); } // H23 * E32 = E32 * H23 - E32;
	if ((a == 4) && (b == 7)) { return  polynomial(monomial(1, 7, 4), monomial(-1, 7, 0), zero_mono); } // H23 * E21 = E21 * H23 - E21;
	if ((a == 4) && (b == 8)) { return  polynomial(monomial(1, 8, 4), monomial(-2, 8, 0), zero_mono); } // H23 * E31 = E31 * H23 - 2E31;
	if ((a == 5) && (b == 6)) { return  polynomial(monomial(1, 6, 5), monomial(1, 6, 0), zero_mono); } // H12 * E32 = E32 * H12 + E32;
	if ((a == 5) && (b == 7)) { return  polynomial(monomial(1, 7, 5), monomial(-2, 7, 0), zero_mono); } // H12 * E21 = E21 * H12 - 2E21;
	if ((a == 5) && (b == 8)) { return  polynomial(monomial(1, 8, 5), monomial(-1, 8, 0), zero_mono); } // H12 * E31 = E31 * H12 - E31;
	if ((a == 6) && (b == 7)) { return  polynomial(monomial(1, 7, 6), monomial(1, 8, 0), zero_mono); } // E32 * E21 = E21 * E32 + E31;
	if ((a == 6) && (b == 8)) { return  polynomial(monomial(1, 8, 6), zero_mono, zero_mono); } // E32 * E31 = E31 * E32;
	if ((a == 7) && (b == 8)) { return  polynomial(monomial(1, 8, 7), zero_mono, zero_mono); } // E21 * E31 = E31 * E21;
}

/// <summary>
/// reconstruction of one branch of the polynomial generated by decompose()
/// </summary>
/// <param name="ordered_pair_part">selected branch of the decomposition</param>
/// <param name="j">order number of part of decomposition; based on the possibilities in transform which generate from 1 to 3 parts,
/// the values can be from 0 to 2 </param>
/// <returns>reconstructed part of new polynomial</returns>
polynomial polynomial::decompose_part(monomial ordered_pair_part, int j)
{
	monomial zero_mono(0, 0, 0);

	int the_number_factor = ordered_pair_part.number_factor * part[0].number_factor;

	int the_factor[monom_length + 2][2];

	for (int ii = 0; ii < monom_length - j - 1; ii++) {
		the_factor[ii][0] = part[0].factor[ii][0];
		the_factor[ii][1] = part[0].factor[ii][1];
	}

	the_factor[monom_length - j - 1][1] = part[0].factor[monom_length - j - 1][1] - 1; 
	the_factor[monom_length - j - 1][0] = the_factor[monom_length - j - 1][1] == 0 ? 0 : part[0].factor[monom_length - j - 1][0];
	the_factor[monom_length - j][0] = ordered_pair_part.factor[0][0]; the_factor[monom_length - j][1] = 1;
	the_factor[monom_length - j + 1][0] = ordered_pair_part.factor[1][0]; the_factor[monom_length - j + 1][1] = 1;
	the_factor[monom_length - j + 2][1] = part[0].factor[monom_length - j][1] - 1; 
	the_factor[monom_length - j + 2][0] = the_factor[monom_length - j + 2][1] == 0 ? 0 : part[0].factor[monom_length - j][0];

	for (int ii = monom_length - j + 1; ii < monom_length; ii++) {
		the_factor[ii + 2][0] = part[0].factor[ii][0];
		the_factor[ii + 2][1] = part[0].factor[ii][1];
	}

	//reordering of zero elements
	int jj = 0;
	while (jj < monom_length + 2) {
		int jj1 = jj;
		while ((jj1 < monom_length + 2) && ((the_factor[jj1][0] == 0) || (the_factor[jj1][1] == 0))) jj1++;
		if (jj1 < monom_length + 2) {
			if ((jj > 0) && (the_factor[jj1][0] == the_factor[jj - 1][0])) {
				the_factor[jj - 1][1] += the_factor[jj1][1];
				the_factor[jj1][0] = 0;
				the_factor[jj1][1] = 0;
				if (the_factor[jj - 1][1] == 0) {
					the_factor[jj - 1][0] = 0;
					jj--;
				}
			}
			else {
				if (jj1 > jj) {
					the_factor[jj][0] = the_factor[jj1][0];
					the_factor[jj][1] = the_factor[jj1][1];
					the_factor[jj1][0] = 0;
					the_factor[jj1][1] = 0;
				}
				jj++;
			}
		}
		else jj = monom_length + 2;
	}

	polynomial newpol = polynomial(monomial(the_number_factor, the_factor), zero_mono, zero_mono);

	//choice between versions for recursive and iterative algorithm:
	
	//iterative version: comment row below
	//recursive version: uncomment row below
	//newpol.decompose();

	return newpol;
}

/// <summary>
/// performing one decomposition step by rewriting the first unordered neighbouring pair of generators from the end
/// </summary>
/// <returns>
/// polynomial after one decomposition step; depending on the decomposition structure (1, 2 or 3 branches), 
/// the resuting polynomial is composed of 1 (newpol1), 2 (newpol12) or 3 (newpol123) new monomials
/// </returns>

//choice between versions for recursive and iterative algorithm:

//iterative version
polynomial polynomial::decompose(void)
//recursive version	
//void polynomial::decompose(void)
{
	for (int j = 1; j < monom_length; j++) {
		if (((part[0].factor[monom_length - j][0] > 0) && (part[0].factor[monom_length - j][1] > 0)) || (part[0].factor[monom_length - j - 1][0] == 9)) {
			if ((part[0].factor[monom_length - j][0] > part[0].factor[monom_length - j - 1][0]) || (part[0].factor[monom_length - j - 1][0] == 9)) {
				
				monomial zero_mono(0, 0, 0);

				//decomposition of first unordered pair from the end
				polynomial ordered_pair = transform(part[0].factor[monom_length - j - 1][0], part[0].factor[monom_length - j][0]);

				//composing new polynomial from first part of decomposition
				polynomial newpol1 = decompose_part(ordered_pair.part[0], j);

				if (ordered_pair.part[1].number_factor == 0) {
					//structure of new polynomial: 1 monomial
					newpol1.sorted();

					//choice between versions for recursive and iterative algorithm:

					//iterative version
					return newpol1;
					//recursive version	
					//*this = polynomial(newpol1, zero_mono, zero_mono);
				}
				else {

					//composing new polynomial from second part of decomposition
					polynomial newpol2 = decompose_part(ordered_pair.part[1], j);

					if (ordered_pair.part[2].number_factor == 0) {
						//structure of new polynomial: 2 monomials
						polynomial newpol12(newpol1, newpol2, zero_mono);
						newpol12.sorted();

						//choice between versions for recursive and iterative algorithm:

						//iterative version
						return newpol12;
						//recursive version	
						//*this = polynomial(newpol12, zero_mono, zero_mono);
					}
					else {

						//composing new polynomial from third part of decomposition
						polynomial newpol3 = decompose_part(ordered_pair.part[2], j);

						//structure of new polynomial: 3 monomials
						polynomial newpol123(newpol1, newpol2, newpol3);
						newpol123.sorted();

						//choice between versions for recursive and iterative algorithm:

						//iterative version
						return newpol123;
						//recursive version	
						//*this = polynomial(newpol123, zero_mono, zero_mono);
					}
				}
			}
		}

	}

	//choice between versions for recursive and iterative algorithm:

	//iterative version: uncomment row below
	//recursive version: comment row below
	return *this;
}