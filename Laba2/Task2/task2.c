#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>

void GeometricMean(double* res, size_t n, ...) {
	if (res == NULL) return;
	va_list ap;
	va_start(ap, n);
	*res = 1.0;
	double vsp;
	for (int i = 0; i < n; i++) {
		vsp = va_arg(ap, double);
		if (vsp < DBL_EPSILON) {
			*res = NAN;
			va_end(ap);
			return;
		}
		*res *= vsp;
	}
	va_end(ap);
	*res = pow(*res, 1.0 / n);
}

double Exponentiation(double base, int exp) {
	if (exp == 0) {
		return 1;
	}
	if (exp < 0) {
		if (base == 0) { return NAN; }
		return Exponentiation(1 / base, -exp);
	}
	if (exp % 2) {
		return base * Exponentiation(base * base, (exp - 1) / 2);
	}
	return Exponentiation(base * base, exp / 2);
}

int main() {
	int number_of_numbers = 3;
	double result_gm;
	double result_e = 2;
	int exp = -2;
	GeometricMean(&result_gm, number_of_numbers, 1.0, 2.0, 3.0);
	if(isnan(result_gm)) {
		printf("Incorrect numbers\n");
		return 1;
	}
	printf("Geometric mean: %.5lf\n", result_gm);
	if(isnan(Exponentiation(result_e, exp))) {
		printf("Incorrect numbers\n");
		return 1;
	}
	printf("Exponentiation: %.5lf\n", Exponentiation(result_e, exp));
	return 0;
}