#ifndef TASK4_H
#define TASK4_H

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

#endif //TASK4_H
