#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

int validate_num(const char* num, int base) {
	for (int i = 0; num[i] != '\0'; i++) {
		if (!isdigit(num[i]) && !isalpha(num[i])) {
			return 1;
		}
		int digit = isdigit(num[i]) ? num[i] - '0' : toupper(num[i]) - 'A' + 10;
		if (digit >= base) {
			return 1;
		}
	}
	return 0;
}

int add_in_base(char** result, const char* num1, const char* num2, int base) {
    while (*num1 == '0' && *(num1 + 1) != '\0') num1++;
    while (*num2 == '0' && *(num2 + 1) != '\0') num2++;

    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);
    size_t max_len = len1 > len2 ? len1 : len2;
    *result = (char*)malloc((max_len + 2) * sizeof(char));
    if (*result == NULL) {
        return -2;
    }
    (*result)[max_len + 1] = '\0';

    int carry = 0;
    for (int i = 0; i < max_len; i++) {
        int digit1 = (i < len1) ? (isdigit(num1[len1 - 1 - i]) ? num1[len1 - 1 - i] - '0' :
        	toupper(num1[len1 - 1 - i]) - 'A' + 10) : 0;
        int digit2 = (i < len2) ? (isdigit(num2[len2 - 1 - i]) ? num2[len2 - 1 - i] - '0' :
        	toupper(num2[len2 - 1 - i]) - 'A' + 10) : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        (*result)[max_len - i] = (sum % base) < 10 ? (sum % base) + '0' : (sum % base) - 10 + 'A';
    }

    if (carry > 0) {
        (*result)[0] = carry < 10 ? carry + '0' : carry - 10 + 'A';
        return 0;
    }
	for (int j = 0; j < max_len; j++) {
		(*result)[j] = (*result)[j + 1];
	}
	(*result)[max_len] = '\0';
    return 0;
}


int sum_in_base(char** result, int base, size_t count, ...) {
    if (base < 2 || base > 36 || count == 0) {
        return -1;
    }

    va_list args;
    va_start(args, count);

    char* current = va_arg(args, char*);
	if (current == NULL || strlen(current) == 0 || validate_num(current, base)) {
		va_end(args);
		return -1;
	}
	if (count == 1) {
		*result = current;
		return 0;
	}

    for (int i = 1; i < count; i++) {
        char* next = va_arg(args, char*);
    	if (next == NULL || strlen(next) == 0 || validate_num(next, base)) {
    		va_end(args);
    		return -1;
    	}
        char* temp = *result;
        if (add_in_base(result, current, next, base) == -2) {
            va_end(args);
            free(temp);
            return -2;
        }
        free(temp);
        current = *result;
    }

    va_end(args);
    return 0;
}

int main() {
    char* result = NULL;
	int base = 17;
	size_t count = 4;
	int res = sum_in_base(&result, base, count, "8G", "0000001Aa0", "100", "000000000");
	if (res == -1) {
		printf("Invalid input\n");
		return 1;
	}
	if (res == -2) {
		printf("Error allocating memory\n");
		return 1;
	}
	if (res == 0) {
	    printf("Sum: %s\n", result);
    	if (count > 1) free(result);
    }

    return 0;
}