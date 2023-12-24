
using namespace std;

const int _max = 10;
//const int max1 = 1000;
const int max1 = 40;

//const int max_parts = 100;
const int max_parts = 40;

const int pmax = 10;

//const int nn = pmax * max1;
const int nn = 40;

class monomial
{
public:

	inline	monomial();
	inline	monomial(int the_number_factor, int the_factor[_max][2]);
	inline	monomial(int the_number_factor, int a, int b);
	inline	~monomial();

	int number_factor;
	int factor[_max][2];
};

monomial::monomial()
	: number_factor()
	, factor()
{

}

monomial::monomial(int the_number_factor, int the_factor[_max][2])
	: number_factor(the_number_factor)
	, factor()

{
	for (int i = 0; i < _max; i++) {
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

	for (int i = 2; i < _max; i++) {
		factor[i][0] = 0;
		factor[i][1] = 0;
	}
}

monomial::~monomial()
{

}