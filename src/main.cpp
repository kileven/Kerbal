#include <iostream>

#include "advanced_math_private.h"

#include "kerbal/exe_serve.hpp"
#include "kerbal/sort.hpp"
#include "kerbal/range.hpp"

#include "kerbal/math/complex.hpp"
#include "kerbal/array_serve.hpp"
#include "kerbal/math/randnum.hpp"
#include "kerbal/math/matrix.hpp"
#include "kerbal/math/basic_math.hpp"
#include "kerbal/math/complexor.hpp"
#include "kerbal/math/integral.hpp"
#include "kerbal/math/mapminmax.hpp"
#include "kerbal/math/statistics.hpp"

#include "kerbal/choose.hpp"
#include "kerbal/dbstream.hpp"
#include "kerbal/spherical.hpp"
#include "kerbal/string_serve.hpp"

#define is_debug true

using namespace std;
using namespace _choose;
using namespace kerbal::math::matrix;
using namespace kerbal::math::complexor;

void conv_test()
{
	Matrix t = {

	{ 17, 24, 1, 8, 15 },

	{ 23, 5, 7, 14, 16 },

	{ 4, 6, 13, 20, 22 },

	{ 10, 12, 19, 21, 3 },

	{ 11, 18, 25, 2, 9 } };

	t.print();

	Matrix core = { { 1, 3, 1 }, { 0, 5, 0 }, { 2, 1, 2 } };

	//	Matrix t(1500, 1500);
	//	Matrix core(100, 100);
	//	conv2(core, t, 2); //.print();
	//
	//	Matrix a = { 0, 1, 2, 3, 4, 5 };
	//	Matrix core = { -1, -2, -3 };
	kerbal::math::matrix::conv_2d<Matrix::max>(core, t).print();
}

int main()
{
	program_start(is_debug);

//	conv_test();

	Matrix::vander( { 4, 7, 8 }).print();

	Complexor<double>::softmax( { 1, 2, 3, 4, 1, 2, 3 }).print();

//	unsigned long t = GetTickCount();
//	::omp_set_num_threads(2);
//	Matrix m([]() ->int {
//		return rand_between(-5,5);
//	}, 1200, 1200);
//	cout << GetTickCount() - t << endl;
//
//	Matrix m2([]() ->int {
//		return rand_between(-5,5);
//	}, 1200, 1200);
//
////	while (1) {
//	t = GetTickCount();
//	::omp_set_num_threads(3);
//	m * m2;
//	cout << GetTickCount() - t << endl;
//	}

	program_will_end();
//	system("pause");
	return 0;
}

