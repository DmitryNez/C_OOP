#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <cassert>
using namespace std;

/*----------2-------------*/
template<class T>
bool close_enough(T a, T b) {
	T alfa = 0.0001;
	if (is_same<T, int>::value || is_same<T, unsigned int>::value) {
		return a == b;
	}
	else {
		double res = a - b;
		return (res >= alfa);
	}
}
/*------------------------*/


/*-----------3-------------*/
template <class T>
double sum1(T t) {
	return t;
}
template <class T, class ...Args>
double sum1(T t, Args... args) {
	return t + sum1(args...);
}
template <class... Args>
double sum2(Args... arg) {
	return (arg + ...);
}
/*--------------------------*/


/*-----------4-------------*/

template <class Op,class... Args>
auto reduce(Op f, Args... arg) {
	return f(arg...);
}
/*--------------------------*/

/*-----------5--------------*/
template <class T, class U>
class type_traits_ {
public:
	decltype(T) typeT;
	decltype(U) typeU;
};

double f1(int x,int y) {
	return x + y;
}

double f2(int x, int y) {
	return x/y;
}

template <class A,class B>
auto multiply_functions(A func1,B func2) {

	static auto func3 = [&]() {return func1(1, 2) * func2(1, 2); };
	/*Здесь вылетает*/
	return func3;
}

int main()
{

	/*-----------3--------------*/
	double res = sum1(1.0,10,2.f);
	cout << res << endl;
	/*-----------4-------------*/
	auto lambdaPlus = [](int x, int y) {return x + y; };
	auto lambdaMinus = [](double x, int y) {return x - y; };

	cout << reduce(lambdaMinus, 4, 1);
	/*------------5-------------*/
	auto func = multiply_functions(f1, f2);
	//cout << func();
	/*--------6--------*/
	assert(close_enough(1,1) == true);
	assert(close_enough(1.01, 1.0) == true);
	assert(sum1(1.0, 10, 2.f),13);
	/*-----------------*/

	return 0;
}
