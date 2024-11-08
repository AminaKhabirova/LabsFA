#ifndef TASK4_H
#define TASK4_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "string.h"
#include "array.h"

typedef struct Address {
	String city;
	String street;
	unsigned int house;
	String building;
	unsigned int apartment;
	String index;
} Address;

typedef struct Mail {
	Address address;
	double weight;
	String identifier;
	String creation_time;
	String delivery_time;
} Mail;

typedef struct Post {
	Address* address;
	Mail* mails;
	int length;
	int capacity;
} Post;

typedef struct Time {
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;
} Time;

int GetInfo(Array *result);
int IsAllDigits(const char *str);
int IsCorrectDate(char *date);
int IsCorrectTime(char *time);
void Concatenation(char *dest, const char *src);
int MyStrcmp(const char *str1, const char *str2);
int TimeParser(const String *str, Time *time);
int CompareTime(const void *a, const void *b);
int CompareCreationTime(const void *a, const void *b);
int CreateAddress(Address *addr, char* city, char* street, unsigned int house, char* building, unsigned int apartment,
	char* index);
int CopyAddress(Address *dest, const Address *src);
void ClearAddress(Address *addr);
int CreateMail(Mail *mail, Address addr, double weight, char *identifier, char *creation_time, char *delivery_time);
void ClearMail(Mail *mail);
int CreatePost(Post *post, Address *addr);
int ResizePost(Post *post, int add_capacity);
int AddLetterToPost(Post *post, Mail mail);
int SearchForMailByIdentifier(Post *post, String mail_id, Mail **found, int *index);
int DeleteMailFromPost(Post *post, String mail_id);
int SearchForMailsInTime(Post *post, Mail *found, int *index);
int SearchForMailsExpired(Post *post, Mail *found, int *index);
void PrintMail(Mail mail);
int CompareMailsByIdentifier(const void *a, const void *b);
void ClearPost(Post *post);
int GetInfoAddress(Array *city, Array *street, Array *building, Array *index, int *house, int *apartment);
int GetInfoPost(Post *post);
int GetInfoMail(Post *post);

#endif //TASK4_H
