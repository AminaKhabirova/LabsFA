#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

typedef struct Array {
	char* value;
	unsigned int length;
	unsigned int capacity;
} Array;

int CreateArray(Array* arr, unsigned int length);
void ClearArray(Array* arr);
int ResizeArray(Array* arr, unsigned int size_delta);
int AddToArray(Array* arr, char symbol);
void ResetArray(Array* arr);

#endif //ARRAY_H
