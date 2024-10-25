#include <stdio.h>
#include <math.h>

double Equation(double x){ return tan(x); }

double Dichotomy(double a, double b, double epsilon, double (*f)(double)) {
	double c;
	if (a > b) {
		return INFINITY;
	}
	if (f(a) * f(b) >= 0 || !isfinite(f(a) * f(b))) {
		return NAN;
	}
	while (b - a > epsilon) {
		c = (a + b) / 2;
		if (!isfinite(f(c))) return NAN;
		if (f(c) == 0.0) {
			return c;
		}
		if (f(a) * f(c) > 0) {
			a = c;
		} else {
			b = c;
		}
	}
	return (a + b) / 2;
}

int main() {
	const double a = 0.01;
	const double b = 2;
	const double epsilon = 0.001;
	double x = Dichotomy(a, b, epsilon, Equation);
	if (isnan(x)) {
		printf("Функция должна иметь разные знаки на границах интервала и быть непрерывной на всем интервале.\n");
		return 1;
	} else if (isinf(x)) {
		printf("Левая граница должна быть меньше правой.\n");
		return 1;
	} else {
		printf("x = %.10lf\n", x);
	}
	return 0;
}
