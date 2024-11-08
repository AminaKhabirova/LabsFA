#include <stdlib.h>
#include "array.h""

int CreateArray(Array* arr, unsigned int length) {
	arr->capacity = length;
	arr->length = 0;
	arr->value = (char*)malloc(length);
	if (!arr->value) return 1;
	arr->value[0] = '\0';
	return 0;
}

void ClearArray(Array* arr) {
	if (arr->value)
		free(arr->value);
	arr->value = NULL;
	arr->length = 0;
	arr->capacity = 0;
}

int ResizeArray(Array* arr, unsigned int add_capacity) {
	char* new_addr = (char*)realloc(arr->value, arr->capacity + add_capacity);
	if (!new_addr) {
		free(arr->value);
		return 1;
	}
	arr->value = new_addr;
	arr->capacity += add_capacity;
	return 0;
}

int AddToArray(Array* arr, char symbol) {
	if (arr->length >= arr->capacity - 2) {
		if (ResizeArray(arr, arr->capacity)) return 1;
	}
	arr->value[arr->length++] = symbol;
	arr->value[arr->length] = '\0';
	return 0;
}

void ResetArray(Array* arr) {
	arr->value[0] = '\0';
	arr->length = 0;
}