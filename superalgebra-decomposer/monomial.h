/// <summary>
/// monomial consisting of an integer coefficient and
/// an ordered sequence of generators with exponents
/// </summary>

#ifndef MONOMIAL_H
#define MONOMIAL_H

using namespace std;

//the global constants:

//maximum number of elements in each monomial (length of array the_factor - in fact there are 2 elements more 
//due to enough space during transformations)
const int monom_length = 10;

//maximum number of monomials in input polynomial
const int poly_length_init = 10;

//maximum number of monomials in output polynomial(length of array the_part in polynomial.h)
const int p_len_reserved = 40;

class monomial
{
public:

	inline	monomial();
	inline	monomial(int the_number_factor, int the_factor[monom_length + 2][2]);
	inline	monomial(int the_number_factor, int a, int b);
	inline	~monomial();

	int number_factor;
	int factor[monom_length + 2][2];
};

monomial::monomial()
	: number_factor()
	, factor()
{

}

monomial::monomial(int the_number_factor, int the_factor[monom_length][2])
	: number_factor(the_number_factor)
	, factor()

{
	for (int i = 0; i < monom_length + 2; i++) {
		factor[i][0] = the_factor[i][0];
		factor[i][1] = the_factor[i][0] == 0 ? 0 : the_factor[i][1];
	}
}

monomial::monomial(int the_number_factor, int a, int b)
	: number_factor(the_number_factor)
	, factor()

{

	factor[0][0] = a;
	factor[0][1] = a == 0 ? 0 : 1;

	factor[1][0] = b;
	factor[1][1] = b == 0 ? 0 : 1;

	for (int i = 2; i < monom_length + 2; i++) {
		factor[i][0] = 0;
		factor[i][1] = 0;
	}
}

monomial::~monomial()
{

}

#endif