#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

typedef enum kOpts { OPT_L, OPT_R, OPT_U, OPT_N, OPT_C } kOpts;

int GetOpts(int argc, char** argv, kOpts* option, unsigned int* number, char** strings) {
	if (argc < 3) {
		return 1;
	}
	const char* proceeding_option = argv[1];
	strings[0] = argv[2];
	if (proceeding_option[0] == '-' && !proceeding_option[2]) {
		switch (proceeding_option[1]) {
			case 'l':
				if (argc != 3) return 1;
				*option = OPT_L;
				break;
			case 'r':
				if (argc != 3) return 1;
				*option = OPT_R;
				break;
			case 'u':
				if (argc != 3) return 1;
				*option = OPT_U;
				break;
			case 'n':
				if (argc != 3) return 1;
				*option = OPT_N;
				break;
			case 'c':
				if (argc < 4) return 1;
				long long int limit_control = 0;
				*option = OPT_C;
				for (int i = 0; argv[3][i]; i++) {
					if (!isdigit(argv[3][i])) return 1;
					limit_control = limit_control * 10 + (argv[3][i] - '0');
					if (limit_control <= UINT_MAX) {
						*number *= 10;
						*number += argv[3][i] - '0';
					} else {
						return 1;
					}
				}
				if (argc > 4) {
					for (int i = 4; i < argc; i++) {
						strings[i - 3] = argv[i];
					}
				}
				break;
			default:
				return 1;
		}
	} else {
		return 1;
	}
	return 0;
}

void HandlerOptL(const char* str, int* len) {
	const char* ptr = str;
	while (*ptr != '\0') {
		ptr++;
	}
	*len = ptr - str;
}

char* HandlerOptR(char* st) {
	int len = 0;
	HandlerOptL(st, &len);
	char* reversed_st = (char*)malloc((len + 1) * sizeof(char));
	if (reversed_st == NULL) {
		return NULL;
	}
	for (int i = 0; i < len; i++) {
		reversed_st[i] = st[len - i - 1];
	}
	reversed_st[len] = '\0';
	for (int i = 0; i < len; i++) {
		st[i] = reversed_st[i];
	}
	free(reversed_st);
	return st;
}

char* HandlerOptU(char* st) {
	int len = 0;
	HandlerOptL(st, &len);
	char* res_st = (char*)malloc((len + 1) * sizeof(char));
	if (res_st == NULL) {
		return NULL;
	}
	for (int i = 0; i < len; i++) {
		if (i % 2 && isalpha(st[i])) {
			res_st[i] = toupper(st[i]);
		} else {
			res_st[i] = st[i];
		}
	}
	for (int i = 0; i < len; i++) {
		st[i] = res_st[i];
	}
	free(res_st);
	return st;
}

char* HandlerOptN(char* st) {
	int len = 0;
	HandlerOptL(st, &len);
	char* res_st = (char*)malloc((len + 1) * sizeof(char));
	if (res_st == NULL) {
		return NULL;
	}
	int digits = 0;
	int letters = 0;
	int others = 0;
	for (int i = 0; i < len; i++) {
		if (isdigit(st[i])) {
			res_st[digits++] = st[i];
		} else if (isalpha(st[i])) {
			letters++;
		}
	}
	others = digits + letters;
	letters = digits;
	for (int i = 0; i < len; i++) {
		if (isalpha(st[i])) {
			res_st[letters++] = st[i];
		} else if (!isdigit(st[i])) {
			res_st[others++] = st[i];
		}
	}
	res_st[len] = '\0';
	for (int i = 0; i < len; i++) {
		st[i] = res_st[i];
	}
	free(res_st);
	return st;
}

char* my_memcpy(char* dest, char* src, unsigned int n) {
	for (unsigned int i = 0; i < n; i++) {
		dest[i] = src[i];
	}
	return dest;
}

int HandlerOptC(size_t argc, char** argv, unsigned int seed, char** res_st) {
	int len = 0;
	HandlerOptL(argv[2], &len);
	int len_st = 0;
	for (int i = 4; i < argc; i++) {
		HandlerOptL(argv[i], &len_st);
		len += len_st;
	}
	*res_st = (char*)malloc((len + 1));
	if (*res_st == NULL) {
		return -1;
	}
	srand(seed);
	int *indices = (int *)malloc((argc - 3) * sizeof(int));
	if (indices == NULL) {
		free(*res_st);
		return -1;
	}
	indices[0] = 2;
	for (int i = 4; i < argc; i++) {
		indices[i - 3] = i;
	}
	for (int i = argc - 4; i >= 0; i--) {
		int j = rand() % (i + 1);
		int temp = indices[i];
		indices[i] = indices[j];
		indices[j] = temp;
	}
	int vsp = 0;
	for (int i = 0; i < argc - 3; i++) {
		HandlerOptL(argv[indices[i]], &len_st);
		my_memcpy(*res_st + vsp, argv[indices[i]], len_st);
		vsp += len_st;
	}
	(*res_st)[len] = '\0';
	free(indices);
	return 0;
}

int main(int argc, char** argv) {
	kOpts opt;
	char* st;
	char* res_st;
	int len = 0;
	unsigned int seed = 0;
	if (GetOpts(argc, argv, &opt, &seed, &st)) {
		printf("Incorrect option\n");
		return 1;
	}
	if (opt == OPT_L) {
		HandlerOptL(st, &len);
		printf("%d\n", len);
	} else if (opt != OPT_C) {
		char* (*handlers[3])(char*) = {HandlerOptR, HandlerOptU, HandlerOptN};
		char* result = handlers[opt - 1](st);
		if (result == NULL) {
			printf("Memory error\n");
			return 1;
		}
		printf("%s\n", result);
	} else {
		if (HandlerOptC(argc, argv, seed, &res_st) == -1) {
			printf("Memory error\n");
			free(res_st);
			return 1;
		}
		printf("%s\n", res_st);
		free(res_st);
	}

	return 0;
}