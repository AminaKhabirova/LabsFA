#include <stdlib.h>
#include "string.h"

int MyStrlen(const char* str) {
	int length = 0;
	while (str[length] != '\0') {
		length++;
	}
	return length;
}

void MyStrcpy(char* dest, const char* src) {
	while (*src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
}

int CreateString(String *arr, char *str) {
	arr->length = MyStrlen(str);
	arr->data = (char *)malloc(sizeof(char) * (arr->length + 1));
	if (!arr->data) return 1;
	int i = 0;
	for (i = 0; i < arr->length; ++i) {
		arr->data[i] = str[i];
	}
	arr->data[i] = '\0';
	return 0;
}

void ClearString(String* str) {
	if (str->data != NULL) {
		free(str->data);
		str->data = NULL;
		str->length = 0;
	}
}

int StringEquality(const String* str1, const String* str2) {
	if ((str1 == NULL && str2 != NULL) || (str1 != NULL && str2 == NULL)) {
		return 0;
	}

	if (str1 == NULL && str2 == NULL) {
		return 1;
	}

	const char* data1 = str1->data;
	const char* data2 = str2->data;

	while (*data1 != '\0' && *data2 != '\0') {
		if (*data1 != *data2) {
			return 0;
		}
		data1++;
		data2++;
	}

	if (*data1 != '\0' || *data2 != '\0') {
		return 0;
	}

	return 1;
}

int StringComparison(const String* str1, const String* str2) {
	if (str1 == NULL && str2 == NULL) {
		return 0;
	}
	if (str1 == NULL) {
		return -1;
	}
	if (str2 == NULL) {
		return 1;
	}

	if (str1->length != str2->length) {
		return (str1->length < str2->length) ? -1 : 1;
	}

	const char* data1 = str1->data;
	const char* data2 = str2->data;

	while (*data1 != '\0' && *data2 != '\0') {
		if (*data1 != *data2) {
			return *data1 < *data2 ? -1 : 1;
		}
		data1++;
		data2++;
	}

	if (*data1 != '\0') {
		return 1;
	}
	if (*data2 != '\0') {
		return -1;
	}

	return 0;
}

int CopyStringToExisting(String* dest, const String* src) {
	if (dest == NULL || src == NULL) {
		return 1;
	}

	ClearString(dest);

	dest->data = (char*)malloc((src->length + 1) * sizeof(char));
	if (dest->data == NULL) {
		return 1;
	}

	MyStrcpy(dest->data, src->data);
	dest->length = src->length;
	return 0;
}

int CopyStringToNew(String *dest, const String *src) {
	if (CreateString(dest, src->data)) {
		return 1;
	}
	return 0;
}

void StringСoncatenation(String* str1, const String* str2) {
	if (str1 == NULL || str2 == NULL) {
		return;
	}

	int new_length = str1->length + str2->length;

	char* new_str = (char*)malloc((new_length + 1) * sizeof(char));
	if (new_str == NULL) {
		return;
	}

	MyStrcpy(new_str, str1->data);

	MyStrcpy(new_str + str1->length, str2->data);

	free(str1->data);

	str1->data = new_str;
	str1->length = new_length;
}

