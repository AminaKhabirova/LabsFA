#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

char* my_strstr(const char *haystack, const char *needle, int needle_len) {
	for (int i = 0; haystack[i] != '\0'; i++) {
		int j;
		for (j = 0; j < needle_len && haystack[i + j] == needle[j]; j++);
		if (j == needle_len) return (char*)(haystack + i);
	}
	return NULL;
}

void SymbolsCount(char* input_file) {
	FILE* input = NULL;
	char sym;
	int count = 0;
	if ((input = fopen(input_file, "r")) == NULL) {
		printf("Failed to open input file\n");
		return;
	}
	while ((sym = fgetc(input)) != EOF) {
		if (sym != '\n') {
			count++;
		} else {
			count = 0;
		}
	}
	fclose(input);
}

void find_substring_in_files(const char *substring, size_t num_files, ...) {
	if (substring == NULL || num_files == 0) {
		printf("Invalid input data\n");
		return;
	}
	va_list args;
	va_start(args, num_files);

	int substring_len = 0;
	int n_count = 0;
	while (substring[substring_len] != '\0') {
		if (substring[substring_len] == '\n' && substring[substring_len + 1] != '\0') {
			n_count++;
		}
		substring_len++;
	}
	if (!substring_len) {
		va_end(args);
		return;
	}

	for (int i = 0; i < num_files; i++) {
		const char *filename = va_arg(args, const char*);
		FILE *file = fopen(filename, "r");
		if (file == NULL) {
			perror("Error opening file");
			continue;
		}

		int line_number = 1;
		int line_count = 1;
		int char_number = 1;
		char *buffer = malloc((substring_len + 1) * sizeof(char));
		if (buffer == NULL) {
			perror("Error allocating memory");
			fclose(file);
			continue;
		}

		int buffer_index = 0;
		char c;
		int found = 0;
		size_t count = 0;


		while ((c = fgetc(file)) != EOF) {
			if (c == '\n') {
				line_count++;
			}
		}
		rewind(file);

		size_t *lengths = (size_t *)malloc(line_count * sizeof(size_t));
		if (lengths == NULL) {
			perror("Ошибка выделения памяти");
			fclose(file);
			continue;
		}

		size_t index = 0;
		while ((c = fgetc(file)) != EOF) {
			if (c != '\n') {
				count++;
			} else {
				if (index == 0) {
					lengths[index++] = count;
				} else {
					lengths[index++] = lengths[index - 1] + count;
				}
				count = 0;
			}
		}
		if (count) {
			if (index == 0) {
				lengths[index] = count;
			} else {
				lengths[index] = lengths[index - 1] + count;
			}
		}
		rewind(file);

		size_t vsp = 0;
		while ((c = fgetc(file)) != EOF) {
			if (c != '\r') buffer[buffer_index++] = c;
			if (buffer_index == substring_len) {
				buffer[buffer_index] = '\0';
				if (my_strstr(buffer, substring, substring_len) != NULL) {
					found = 1;
					if (line_number - n_count == 1) {
						vsp = 0;
					} else {
						vsp = lengths[line_number - n_count - 2];
					}
					printf("File: %s, Line: %d, Char: %lu\n", filename, line_number - n_count,
						char_number - vsp - substring_len + 1);
				}
				for (int j = 0; j < substring_len - 1; j++) {
					buffer[j] = buffer[j + 1];
				}
				buffer_index--;
			}

			if (c == '\n') {
				line_number++;
			}
			if (c != '\r') char_number++;
		}

		// Проверка последнего символа
		if (found == 0 && buffer_index == substring_len - 1) {
			buffer[buffer_index] = '\0';
			if (my_strstr(buffer, substring, substring_len) != NULL) {
				vsp = lengths[line_number - n_count - 2];
				printf("%lu\n", vsp);
				printf("File: %s, Line: %d, Char: %lu\n", filename, line_number - n_count,
					char_number - vsp - substring_len + 1);
			}
		}

		free(buffer);
		free(lengths);
		fclose(file);
	}

	va_end(args);
}

int check_string(const char *str) {
	int length = 0;
	while (str[length] != '\0') {
		length++;
	}

	for (int i = length + 1; str[i] != '\0'; i++) {
		if (str[i] != '\0') {
			return -1;
		}
	}
	return 0;
}

int main() {
	// Используем строку с нулевым символом
	const char substring[] = "\naa";
	if (check_string(substring) == -1) {
		printf("Invalid input data\n");
		return 1;
	}
	find_substring_in_files(substring, 1, "test.txt");
	return 0;
}