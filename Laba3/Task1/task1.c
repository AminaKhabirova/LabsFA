#include <stdio.h>

size_t Add(size_t a, size_t b) {
	while (b != 0) {
		size_t carry = a & b;
		a = a ^ b;
		b = carry << 1;
	}
	return a;
}

int IsCorrectBase(int base) {
	for (int i = 1; i <= 5;) {
		size_t power_of_two = 1 << i;
		if (power_of_two == base) return i;
		i = (int)Add(i, 1);
	}
	return 0;
}

int Conversion(int number, int base, char* result) {
	int power_of_two = IsCorrectBase(base);
	if (!power_of_two) return -1;

	int negative = number < 0;

	char* res_ptr;
	size_t index = 0;
	if (number == 0) {
		res_ptr = (char*)Add((size_t)result, index);
		*res_ptr = '0';
		index = Add(index, 1);
		res_ptr = (char*)Add((size_t)result, index);
		*res_ptr = '\0';
		return 0;
	}

	size_t new_number;
	if (negative) {
		number = (int)Add(number, 1);
		new_number = ~number;
		new_number = Add(new_number, 2);
	} else {
		new_number = number;
	}

	char vsp[34];
	char* ptr = vsp;
	char *new_ptr;
	while (new_number > 0) {
		size_t digit = new_number & Add(base, -1);
		new_ptr = (char*)(Add((size_t)ptr, index));
		*new_ptr = (digit >= 0 && digit <= 9) ? (char)Add(digit, (int)'0') :
		(char)Add(Add( digit, (int)'A'), -10);
		index = Add(index, 1);
		new_number >>= power_of_two;
	}

	if (negative) {
		new_ptr = (char*)(Add((size_t)ptr, index));
		*new_ptr = '-';
		index = Add(index, 1);
	}


	for (int i = Add(index, -1); i >= 0;) {
		int res_index = Add(Add(index, Add(~i, 1)), -1);
		res_ptr = (char*)Add((size_t)result, res_index);
		new_ptr = (char*)(Add((size_t)ptr, i));
		*res_ptr = *new_ptr;
		i = Add(i, -1);
	}

	res_ptr = (char*)Add((size_t)result, index);
	*res_ptr = '\0';
	return 0;
}

int main() {
	int number = 1234;
	int base = 16;
	char result[34];
	if (Conversion(number, base, result)) {
		printf("Incorrect base\n");
		return 1;
	}
	printf("The number %d in the base %d number system is %s\n", number, base, result);
	return 0;
}