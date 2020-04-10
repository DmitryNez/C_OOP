#include "pch.h"
#include <iostream>
#include <exception>
#include <vector>
using namespace std;

const vector<vector<double>> weights_and_roots = {
	{1.0,-0.577350269,1.0,0.577350269}, //n = 2
	{0.555555556,-0.774596669, 0.888888889, 0, 0.555555556, 0.774596669}, //n = 3
	{0.347854845,-0.861136312, 0.652145155, -0.339981044, 0.652145155, 0.339981044, 0.347854845, 0.861136312 }, //n = 4
	{0.236926885,-0.906179846, 0.478628670, -0.538469310, 0.568888889, 0.0, 0.478628670, 0.538469310, 0.236926885, 0.906179846 }, //n = 5
	{0.171324492,-0.932469514, 0.360761573, -0.661209386, 0.467913935, -0.238619186, 0.467913935, 0.238619186, 0.360761573, 0.661209386,0.171324492,0.932469514 } // n = 6
};


double Function(double x) {
	return x * cos(x);
}

constexpr double RectangleMethod(double(*function)(double), double a, double b, int n) {
	const double h = (b - a) / n;
	double result = (function(a) + function(b)) / 2;

	for (int i = 1; i < n; ++i) {
		result += function(a + h * i);
	}

	result = result * h;

	return result;
}


constexpr double SimpsonMethod(double(*function)(double), double a, double b, int n) {
	const double h = (b - a) / n;
	double k1 = 0, k2 = 0;
	for (int i = 1; i < n / 2; i++) {
		k1 += function(a + 2 * i * h); // четные
	}
	for (int i = 1; i <= n / 2; i++) {
		k2 += function(a + (2 * i - 1) * h); // нечетные
	}

	return h / 3 * (function(a) + 4 * k2 + 2 * k1 + function(b));
}

constexpr double GaussMethod(double(*function)(double), double a, double b, int n) {
	double result = (b - a) / 2;
	double sum = 0;
	if (n < 2 || n > 6) {
		throw runtime_error("Degree of approximation must be greater than 2 and less than 6");
	}
	for (int i = 0; i <= n / 2; ++i) {
		sum += weights_and_roots[n - 2][2 * i] * function(result + a + result * weights_and_roots[n - 2][2 * i + 1]);
	}

	return result * sum;
}

template<typename IntMeth, typename Function>
void Integrate(Function func, IntMeth method, double a, double b, int n) {
	if (n <= 0) {
		throw runtime_error("Partition or degree of approximation must be greater zero");
	}
	cout << method(func, a, b, n) << endl;
}

int main(int argc, char* argv[]) {

	Integrate(Function, GaussMethod, 0, 1, 2);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(TestAnyException, TestGaussMethodException) {
	ASSERT_ANY_THROW(
		{  GaussMethod(Function, 0, 1, 0); }
	);
}

TEST(TestAnyException, TestIntegrateException) {
	ASSERT_ANY_THROW(
		{ Integrate(Function, GaussMethod, 0, 1, -1); }
	);
}

TEST(TestIntegrateLinearFunc, TestGaussMethod) {

	double result = GaussMethod([](double x) {return x; }, 0, 1, 2);
    EXPECT_EQ(result, 0.5);
}

TEST(TestIntegrateLinearFunc, TestSimpsonMethod) {
	double result = SimpsonMethod([](double x) {return x; }, 0, 1, 100);
	EXPECT_EQ(result, 0.5);
}

TEST(TestIntegrateLinearFunc, TestRectangleMethod) {
	double result = RectangleMethod([](double x) {return x; }, 0, 1, 100);
	EXPECT_EQ(result, 0.5);
}