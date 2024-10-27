#include <stdio.h>

int Add(int a, int b) {
	while (b != 0) {
		int carry = a & b;
		a = a ^ b;
		b = carry << 1;
	}
	return a;
}

int IsCorrectBase(int base) {
	for (int i = 1; i <= 5;) {
		int power_of_two = 1 << i;
		if (power_of_two == base) return i;
		i = Add(i, 1);
	}
	return 0;
}

int Conversion(int number, int base, char* result) {
	int power_of_two = IsCorrectBase(base);
	if (!power_of_two) return -1;

	int negative = number < 0;
	if (negative) {
		number = Add(~number, 1);
	}

	char vsp[34];
	int index = 0;
	while (number > 0) {
		int digit = number & Add(base, -1);
		vsp[index] = (digit >= 0 && digit <= 9) ? (char)Add(digit, (int)'0') :
		(char)Add(Add( digit, (int)'A'), -10);
		index = Add(index, 1);
		number >>= power_of_two;
	}

	if (negative) {
		vsp[index] = '-';
		index = Add(index, 1);
	}

	for (int i = Add(index, -1); i >= 0;) {
		result[Add(Add(index, -i), -1)] = vsp[i];
		i = Add(i, -1);
	}

	result[Add(index, 1)] = '\0';
	return 0;
}

int main() {
	int number = 2147483647;
	int base = 32;
	char result[34];
	if (Conversion(number, base, result)) {
		printf("Incorrect base\n");
		return 1;
	}
	printf("The number %d in the base %d number system is %s\n", number, base, result);
	return 0;
}