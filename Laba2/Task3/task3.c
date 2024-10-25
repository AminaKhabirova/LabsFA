#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

char* my_strstr(const char *buffer, const char *substring, size_t substring_len) {
	for (size_t i = 0; buffer[i] != '\0'; i++) {
		size_t j;
		for (j = 0; j < substring_len && buffer[i + j] == substring[j]; j++);
		if (j == substring_len) return (char*)(buffer + i);
	}
	return NULL;
}

void find_substring_in_files(const char *substring, size_t number_of_files, ...) {
	if (substring == NULL || number_of_files == 0) {
		printf("Invalid input data\n");
		return;
	}

	va_list args;
	va_start(args, number_of_files);

	// количество символов в подстроке
	size_t substring_len = 0;
	size_t n_count = 0;
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

	for (int i = 0; i < number_of_files; i++) {
		const char *filename = va_arg(args, const char*);
		FILE *file = fopen(filename, "r");
		if (file == NULL) {
			printf("Error opening file\n");
			continue;
		}

		// создание буфера
		char *buffer = malloc((substring_len + 1) * sizeof(char));
		if (buffer == NULL) {
			printf("Error allocating memory\n");
			fclose(file);
			continue;
		}

		// подсчет количества строк в файле
		char c;
		size_t line_count = 1;
		while ((c = fgetc(file)) != EOF) {
			if (c == '\n') {
				line_count++;
			}
		}
		rewind(file);

		// создание массива с количеством символов в строках
		size_t *lengths = (size_t*)malloc(line_count * sizeof(size_t));
		if (lengths == NULL) {
			printf("Error allocating memory\n");
			fclose(file);
			continue;
		}

		// подсчет количества символов в текущей строке + символы в предыдущих строках
		size_t index = 0;
		size_t size = 0;
		while ((c = fgetc(file)) != EOF) {
			if (c != '\n') {
				size++;
			} else {
				if (index == 0) {
					lengths[index++] = size;
				} else {
					lengths[index++] = lengths[index - 1] + size;
				}
				size = 0;
			}
		}
		if (size) {
			if (index == 0) {
				lengths[index] = size;
			} else {
				lengths[index] = lengths[index - 1] + size;
			}
		}
		rewind(file);

		size_t line_number = 1;
		size_t char_number = 1;
		size_t buffer_index = 0;
		size_t vsp = 0;
		while ((c = fgetc(file)) != EOF) {
			if (c != '\r') buffer[buffer_index++] = c;
			if (buffer_index == substring_len) {
				buffer[buffer_index] = '\0';
				if (my_strstr(buffer, substring, substring_len) != NULL) {
					if (line_number - n_count == 1) {
						vsp = 0;
					} else {
						vsp = lengths[line_number - n_count - 2];
					}
					printf("File: %s, Line: %lu, Char: %lu\n", filename, line_number - n_count,
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
		if (buffer_index == substring_len - 1) {
			buffer[buffer_index] = '\0';
			if (my_strstr(buffer, substring, substring_len) != NULL) {
				vsp = lengths[line_number - n_count - 2];
				printf("%lu\n", vsp);
				printf("File: %s, Line: %lu, Char: %lu\n", filename, line_number - n_count,
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
	size_t length = 0;
	while (str[length] != '\0') {
		length++;
	}

	for (size_t i = length + 1; str[i] != '\0'; i++) {
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
	find_substring_in_files(substring, 2, "test.txt", "test3.txt");
	return 0;
}