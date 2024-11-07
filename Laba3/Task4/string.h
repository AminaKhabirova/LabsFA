#ifndef STRING_H
#define STRING_H

typedef struct String {
	char* data;
	int length;
} String;

int MyStrlen(const char* str);
void MyStrcpy(char* dest, const char* src);
int CreateString(String* res, char* str);
void ClearString(String* str);
int StringEquality(const String* str1, const String* str2);
int StringComparison(const String* str1, const String* str2);
int CopyStringToExisting(String* dest, const String* src);
int CopyStringToNew(String *dest, const String *src);
void StringСoncatenation(String* str1, const String* str2);

#endif //STRING_H
