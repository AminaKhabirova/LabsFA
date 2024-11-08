#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "task4.h"

int GetInfo(Array *result) {
	int ch;
	while ((ch = getchar()) <= ' ') {}
	if (AddToArray(result, (char)ch)) return 1;
	ch = getchar();
	while (ch != '\n') {
		if (AddToArray(result, (char)ch)) return 1;
		ch = getchar();
	}
    return 0;
}

int IsAllDigits(const char *str) {
	if (str == NULL) {
		return 0;
	}
	while (*str) {
		if (!isdigit(*str)) {
			return 0;
		}
		str++;
	}
	return 1;
}

int IsCorrectDate(char *date) {
	int day, month, year;
	if (sscanf(date, "%2d:%2d:%4d", &day, &month, &year) != 3) {
		return 0;
	}
	if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0) {
		return 0;
	}
	if ((year % 4 != 0 && month == 2 && day > 28) || (year % 4 == 0 && month == 2 && day > 29) ||
		((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)) {
		return 0;
	}
	return 1;
}

int IsCorrectTime(char *time) {
	int hour, minute, second;
	if (sscanf(time, "%2d:%2d:%2d", &hour, &minute, &second) != 3) {
		return 0;
	}
	if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
		return 0;
	}
	return 1;
}

void Concatenation(char *dest, const char *src) {
	while (*dest) {
		dest++;
	}
	while (*src) {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
}

int MyStrcmp(const char *str1, const char *str2) {
	while (*str1 && (*str1 == *str2)) {
		str1++;
		str2++;
	}
	return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int TimeParser(const String *str, Time *time) {
	int day, month, year, hour, minute, second;
	if (sscanf(str->data, "%2d:%2d:%4d %2d:%2d:%2d", &day, &month, &year, &hour, &minute, &second) != 6) {
		return 2;
	}
	if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0 ||
		hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
		return 2;
		}
	if ((year % 4 != 0 && month == 2 && day > 28) || (year % 4 == 0 && month == 2 && day > 29) ||
		((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)) {
		return 2;
		}

	time->day = day;
	time->month = month;
	time->year = year;
	time->hour = hour;
	time->minute = minute;
	time->second = second;

	return 0;
}

int CompareTime(const void *a, const void *b) {
	Time t1 = *((Time *)a);
	Time t2 = *((Time *)b);

	if (t1.year != t2.year) return t1.year - t2.year;
	if (t1.month != t2.month) return t1.month - t2.month;
	if (t1.day != t2.day) return t1.day - t2.day;
	if (t1.hour != t2.hour) return t1.hour - t2.hour;
	if (t1.minute != t2.minute) return t1.minute - t2.minute;
	return t1.second - t2.second;
}

int CompareCreationTime(const void *a, const void *b) {
	Mail *m1 = (Mail *)a;
	Mail *m2 = (Mail *)b;

	Time t1, t2;
	if (TimeParser(&m1->creation_time, &t1) || TimeParser(&m2->creation_time, &t2)) {
		return 2;
	}

	return CompareTime(&t1, &t2);
}

int CreateAddress(Address *addr, char* city, char* street, unsigned int house, char* building, unsigned int apartment,
					char* index) {
	if (CreateString(&addr->city, city)) return 1; // Ошибка выделения памяти
	if (CreateString(&addr->street, street)) return 1; // Ошибка выделения памяти
	addr->house = house;
	if (CreateString(&addr->building, building)) return 1; // Ошибка выделения памяти
	addr->apartment = apartment;
	if (CreateString(&addr->index, index)) return 1; // Ошибка выделения памяти
	if (addr->city.length == 0 || addr->street.length == 0 || house < 1 || apartment < 1 || addr->index.length != 6 ||
		!IsAllDigits(addr->index.data)) {
		return 2; // Некорректные входные данные
	}
	return 0;
}

int CopyAddress(Address *dest, const Address *src) {
	if (CopyStringToNew(&dest->city, &src->city)) return 1;
	if (CopyStringToNew(&dest->street, &src->street)) return 1;
	dest->house = src->house;
	if (CopyStringToNew(&dest->building, &src->building)) return 1;
	dest->apartment = src->apartment;
	if (CopyStringToNew(&dest->index, &src->index)) return 1;
	return 0;
}

void ClearAddress(Address *addr) {
	ClearString(&addr->city);
	ClearString(&addr->street);
	addr->house = 0;
	ClearString(&addr->building);
	addr->apartment = 0;
	ClearString(&addr->index);
}

int CreateMail(Mail *mail, Address addr, double weight, char *identifier, char *creation_time, char *delivery_time) {
	CopyAddress(&mail->address, &addr);
	mail->weight = weight;
	if (CreateString(&mail->identifier, identifier)) return 1;
	if (CreateString(&mail->creation_time, creation_time)) return 1;
	if (CreateString(&mail->delivery_time, delivery_time)) return 1;

	Time time;
	if (mail->identifier.length != 14 || !IsAllDigits(mail->identifier.data) ||
		TimeParser(&mail->creation_time, &time) || TimeParser(&mail->delivery_time, &time) ||
		weight < 0) {
		return 2;
	}

	return 0;
}

void ClearMail(Mail *mail) {
	ClearAddress(&mail->address);
	mail->weight = 0;
	ClearString(&mail->identifier);
	ClearString(&mail->creation_time);
	ClearString(&mail->delivery_time);
}

int CreatePost(Post *post, Address *addr) {
	if (!addr) return 2;
	post->address = addr;
	post->mails = (Mail *)malloc(sizeof(Mail) * 10);
	if (!post->mails) return 1;
	post->length = 0;
	post->capacity = 10;
	return 0;
}

int ResizePost(Post *post, int add_capacity) {
	Mail *new_addr = (Mail *)realloc(post->mails, post->capacity + add_capacity);

	if (!new_addr) {
		free(post->mails);
		return 1;
	}

	post->mails = new_addr;
	post->capacity += add_capacity;

	return 0;
}

int AddLetterToPost(Post *post, Mail mail) {
	if (post->length + 1 >= post->capacity) {
		if (ResizePost(post, post->capacity)) return 1;
	}
	Mail *current = &post->mails[post->length++];
	CopyAddress(&current->address, &mail.address);
	current->weight = mail.weight;
	CopyStringToNew(&current->identifier, &mail.identifier);
	CopyStringToNew(&current->creation_time, &mail.creation_time);
	CopyStringToNew(&current->delivery_time, &mail.delivery_time);
	return 0;
}

int SearchForMailByIdentifier(Post *post, String mail_id, Mail **found, int *index) {
	for (int i = 0; i < post->length; ++i) {
		if (StringEquality(&post->mails[i].identifier, &mail_id)) {
			*found = &post->mails[i];
			*index = i;
			return 0;
		}
	}

	return 1;
}

int DeleteMailFromPost(Post *post, String mail_id) {
	Mail *mail;
	int index = 0;

	if (!SearchForMailByIdentifier(post, mail_id, &mail, &index)) {
		ClearMail(mail);
	} else {
		return 1;
	}

	for (int i = index; i < post->length - 1; ++i) {
		post->mails[i] = post->mails[i + 1];
	}

	post->length--;

	return 0;
}

int SearchForMailsInTime(Post *post, Mail *found, int *index) {
	if (found == NULL) return 1;
	time_t cur_time;
	time(&cur_time);
	struct tm *time_info = localtime(&cur_time);

	Time time1 = {time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900,
		time_info->tm_hour, time_info->tm_min, time_info->tm_sec};

	for (int i = 0; i < post->length; ++i) {
		Time temp;
		if (TimeParser(&post->mails[i].delivery_time, &temp))
			continue;

		if (CompareTime(&temp, &time1) > 0)
			found[(*index)++] = post->mails[i];

	}

	qsort(found, *index, sizeof(Mail), CompareCreationTime);

	return 0;
}

int SearchForMailsExpired(Post *post, Mail *found, int *index) {
	time_t cur_time;
	time(&cur_time);
	struct tm *time_info = localtime(&cur_time);

	Time time1 = {time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900,
		time_info->tm_hour, time_info->tm_min, time_info->tm_sec};

	for (int i = 0; i < post->length; ++i) {
		Time temp;
		if (TimeParser(&post->mails[i].delivery_time, &temp))
			continue;

		if (CompareTime(&temp, &time1) <= 0)
			found[(*index)++] = post->mails[i];

	}

	qsort(found, *index, sizeof(Mail), CompareCreationTime);

	return 0;
}

void print_mail(Mail mail) {
	printf("Mail:\n\tWeight: %f\n\tID: %s\n\tCreation date: %s\n\tDelivery date: %s\n\t", mail.weight,
		mail.identifier.data, mail.creation_time.data, mail.delivery_time.data);
	printf("Address:\n\tPost Index: %s\n\tCity: %s\n\tStreet: %s\n\tBuilding: %s\n\tHouse: %u\n\tApartment: %u\n",
		mail.address.index.data, mail.address.city.data, mail.address.street.data, mail.address.building.data,
		mail.address.house, mail.address.apartment);
}

int compare_mails(const void *a, const void *b) {
	Mail *mail1 = (Mail *) a;
	Mail *mail2 = (Mail *) b;

	String index1 = mail1->address.index;
	String index2 = mail2->address.index;
	if (!StringEquality(&index1, &index2))
		return StringComparison(&index1, &index2);

	return StringComparison(&mail1->identifier, &mail2->identifier);
}

void ClearPost(Post *post) {
	ClearAddress(post->address);
	free(post->address);
	post->address = NULL;
	if (post->mails) {
		for (int i = 0; i < post->length; ++i) {
			ClearMail(&post->mails[i]);
		}
		free(post->mails);
		post->mails = NULL;
	}
	post->length = 0;
	post->capacity = 0;
}

int GetInfoAddress(Array *city, Array *street, Array *building, Array *index, int *house, int *apartment) {
	if (!city || !street || !building || !index || !house || !apartment) return 1;
	if (CreateArray(city, 10)) return 1;

    if (CreateArray(street, 10)) {
        ClearArray(city);
        return 1;
    }

	if (CreateArray(building, 10)) {
		ClearArray(city);
		ClearArray(street);
		return 1;
	}

	if (CreateArray(index, 10)) {
		ClearArray(city);
		ClearArray(street);
		ClearArray(building);
		return 1;
	}

    printf("City: ");
	if (GetInfo(city)) {
		ClearArray(city);
		ClearArray(street);
		ClearArray(building);
		ClearArray(index);
		return 1;
	}

	printf("Street: ");
	if (GetInfo(street)) {
		ClearArray(city);
		ClearArray(street);
		ClearArray(building);
		ClearArray(index);
		return 1;
	}

	int ch;
	printf("House number: ");
	while (*house == 0) {
		while ((ch = getchar()) <= ' ') {}
		ungetc(ch, stdin);
		if (scanf(" %u", house) != 1 || *house <= 0) {
			printf("Incorrect house number\nEnter correct house number: ");
			*house = 0;
			while ((ch = getchar()) != '\n' && ch != EOF) {}
			continue;
		}
		while ((ch = getchar()) != '\n' && ch != EOF) {
			if (ch != ' ' && ch != '\t') {
				printf("Incorrect house number\nEnter correct house number: ");
				*house = 0;
				while ((ch = getchar()) != '\n' && ch != EOF) {}
				ungetc(ch, stdin);
			}
		}
	}

	printf("Building (or enter '-'): ");
	if (GetInfo(building)) {
		ClearArray(city);
		ClearArray(street);
		ClearArray(building);
		ClearArray(index);
		return 1;
	}

	printf("Apartment number: ");
	while (*apartment == 0) {
		while ((ch = getchar()) <= ' ') {}
		ungetc(ch, stdin);
		if (scanf(" %u", apartment) != 1 || *apartment <= 0) {
			printf("Incorrect apartment number\nEnter correct apartment number: ");
			*apartment = 0;
			while ((ch = getchar()) != '\n' && ch != EOF) {}
			continue;
		}
		while ((ch = getchar()) != '\n' && ch != EOF) {
			if (ch != ' ' && ch != '\t') {
				printf("Incorrect apartment number\nEnter correct apartment number: ");
				*apartment = 0;
				while ((ch = getchar()) != '\n' && ch != EOF) {}
				ungetc(ch, stdin);
			}
		}
	}

	printf("Index (6 digits): ");
	while (1) {
		if (GetInfo(index)) {
			ClearArray(city);
			ClearArray(street);
			ClearArray(building);
			ClearArray(index);
			return 1;
		}
		if (index->length != 6 || !IsAllDigits(index->value)) {
			index->value[0] = '\0';
			index->length = 0;
			printf("Incorrect index\nEnter correct index: ");
			continue;
		}
		break;
	}
	return 0;
}

int GetInfoPost(Post *post) {
	if (!post) return 1;
    Array city, street, building, index;
	int house = 0, apartment = 0;

	printf("Enter post address details:\n");
	if (GetInfoAddress(&city, &street, &building, &index, &house, &apartment)) return 1;

    Address *addr = (Address *)malloc(sizeof(Address));
    if (!addr) {
    	ClearArray(&city);
    	ClearArray(&street);
    	ClearArray(&building);
    	ClearArray(&index);
        return 1;
    }

    int error = CreateAddress(addr, city.value, street.value, house, building.value, apartment,
    	index.value);

	ClearArray(&city);
	ClearArray(&street);
	ClearArray(&building);
	ClearArray(&index);

    if (error) {
    	ClearAddress(addr);
        free(addr);
        return error;
    }

    CreatePost(post, addr);

	printf("Post initialised successfully!\n\n");
    return 0;
}

int GetInfoMail(Post *post) {
	if (!post) return 1;
	Array city, street, building, index, id;
	int house = 0, apartment = 0;

	printf("Enter mail details:\n");

	if (GetInfoAddress(&city, &street, &building, &index, &house, &apartment)) return 1;

    Address *addr = (Address *)malloc(sizeof(Address));
    if (!addr) {
    	ClearArray(&city);
    	ClearArray(&street);
    	ClearArray(&building);
    	ClearArray(&index);
        return 1;
    }

    int error = CreateAddress(addr, city.value, street.value, house, building.value, apartment,
    	index.value);

	ClearArray(&city);
	ClearArray(&street);
	ClearArray(&building);
	ClearArray(&index);

    if (error) {
    	ClearAddress(addr);
    	free(addr);
        return error;
    }

	printf("Weight: ");
	char ch;
	double weight = -1;
	while (weight < 0) {
		while ((ch = getchar()) <= ' ') {}
		ungetc(ch, stdin);
		if (scanf(" %lf", &weight) != 1 || weight < 0) {
			printf("Incorrect weight\nEnter correct weight: ");
			weight = -1;
			while ((ch = getchar()) != '\n' && ch != EOF) {}
			continue;
		}
		while ((ch = getchar()) != '\n' && ch != EOF) {
			if (ch != ' ' && ch != '\t') {
				printf("Incorrect weight\nEnter correct weight: ");
				weight = -1;
				while ((ch = getchar()) != '\n' && ch != EOF) {}
				ungetc(ch, stdin);
			}
		}
	}

	if (CreateArray(&id, 10)) {
		ClearAddress(addr);
		free(addr);
		return 1;
	}
	id.value[0] = '\0';

	printf("ID (14 digits): ");
	while (id.value[0] == '\0') {
		if (GetInfo(&id)) {
			ClearAddress(addr);
			free(addr);
			return 1;
		}
		if (id.length != 14 || !IsAllDigits(id.value)) {
			id.value[0] = '\0';
			id.length = 0;
			printf("Incorrect ID\nEnter correct ID: ");
			continue;
		}
		for (int i = 0; i < post->length; i++) {
			if (MyStrcmp(post->mails[i].identifier.data, id.value) == 0) {
				id.value[0] = '\0';
				id.length = 0;
				printf("Incorrect ID\nEnter correct ID: ");
				i = post->length;
			}
		}
	}

	printf("Creation time (dd:mm:yyyy hh:mm:ss): ");
	char creation_date[15];
	creation_date[0] = '\0';
	char creation_time[15];
	creation_time[0] = '\0';
	while (creation_date[0] == '\0' || creation_time[0] == '\0') {
		while ((ch = getchar()) <= ' ') {}
		ungetc(ch, stdin);
		if (scanf(" %10s %8s", creation_date, creation_time) != 2 || MyStrlen(creation_date) != 10 ||
			MyStrlen(creation_time) != 8 || !IsCorrectDate(creation_date) || !IsCorrectTime(creation_time)) {
			printf("Incorrect time\nEnter correct time: ");
			creation_date[0] = '\0';
			creation_time[0] = '\0';
			while ((ch = getchar()) != '\n' && ch != EOF) {}
			continue;
		}
		while ((ch = getchar()) != '\n' && ch != EOF) {
			if (ch != ' ' && ch != '\t') {
				printf("Incorrect time\nEnter correct time: ");
				creation_date[0] = '\0';
				creation_time[0] = '\0';
				while ((ch = getchar()) != '\n' && ch != EOF) {}
				ungetc(ch, stdin);
			}
		}
	}
	creation_date[10] = '\0';
	creation_time[8] = '\0';

	printf("Delivery time (dd:mm:yyyy hh:mm:ss): ");
	char delivery_date[15];
	delivery_date[0] = '\0';
	char delivery_time[15];
	delivery_time[0] = '\0';
	while (delivery_date[0] == '\0' || delivery_time[0] == '\0') {
		while ((ch = getchar()) <= ' ') {}
		ungetc(ch, stdin);
		if (scanf(" %10s %8s", delivery_date, delivery_time) != 2 || MyStrlen(delivery_date) != 10 ||
			MyStrlen(delivery_time) != 8 || !IsCorrectDate(delivery_date) || !IsCorrectTime(delivery_time)) {
			printf("Incorrect time\nEnter correct time: ");
			delivery_date[0] = '\0';
			delivery_time[0] = '\0';
			while ((ch = getchar()) != '\n' && ch != EOF) {}
			continue;
		}
		while ((ch = getchar()) != '\n' && ch != EOF) {
			if (ch != ' ' && ch != '\t') {
				printf("Incorrect time\nEnter correct time: ");
				delivery_date[0] = '\0';
				delivery_time[0] = '\0';
				while ((ch = getchar()) != '\n' && ch != EOF) {}
				ungetc(ch, stdin);
			}
		}
	}
	delivery_date[10] = '\0';
	delivery_time[8] = '\0';


	char full_creation_time[30] = "";
	Concatenation(full_creation_time, creation_date);
	Concatenation(full_creation_time, " ");
	Concatenation(full_creation_time, creation_time);
	full_creation_time[19] = '\0';

	char full_delivery_time[30] = "";
	Concatenation(full_delivery_time, delivery_date);
	Concatenation(full_delivery_time, " ");
	Concatenation(full_delivery_time, delivery_time);
	full_delivery_time[19] = '\0';

	Mail mail;
	int err = CreateMail(&mail, *addr, weight, id.value, full_creation_time, full_delivery_time);

	ClearAddress(addr);
	free(addr);
	ClearArray(&id);

	if (err) return err;


	err = AddLetterToPost(post, mail);
	ClearMail(&mail);
	if (err) return err;


	printf("Mail added successfully!\n\n");

	return 0;
}

int main() {
	Post post;
	int error = GetInfoPost(&post);
	if (error) {
		ClearPost(&post);
		return 1;
	}

	printf("Available commands:\n1. Add mail\n2. Delete mail by ID\n3. Get information about mail by ID\n"
		"4. Search for mails delivered in time\n5. Search for mails that have expired\n6. Exit the program\n"
		"Enter the number of command: ");

	Array comm;
	if (CreateArray(&comm, 10)) {
		ClearPost(&post);
		return 1;
	}
	while (1) {
		if (GetInfo(&comm)) {
			ClearPost(&post);
			ClearArray(&comm);
			return 1;
		}
		if (!(comm.value[0] == '1' || comm.value[0] == '2' || comm.value[0] == '3' || comm.value[0] == '4' ||
			comm.value[0] == '5' || comm.value[0] == '6') || comm.value[1] != '\0') {
			comm.value[0] = '\0';
			comm.length = 0;
			printf("Incorrect command\n\nAvailable commands:\n1. Add mail\n2. Delete mail by ID\n"
				"3. Get information about mail by ID\n4. Search for mails delivered in time\n"
				"5. Search for mails that have expired\n6. Exit the program\nEnter the number of command: ");
			continue;
		}
		switch (comm.value[0]) {
			case '1':
				error = GetInfoMail(&post);
			if (error) {
				ClearPost(&post);
				ClearArray(&comm);
				return error;
			}
			qsort(post.mails, post.length, sizeof(Mail), compare_mails);
			ResetArray(&comm);
			break;
			case '2':
				printf("ID (14 digits): ");
			Array id;
			if (CreateArray(&id, 10)) {
				ClearPost(&post);
				ClearArray(&comm);
				return 1;
			}
			while (1) {
				if (GetInfo(&id)) {
					ClearPost(&post);
					ClearArray(&comm);
					return 1;
				}
				if (id.length != 14 || !IsAllDigits(id.value)) {
					id.value[0] = '\0';
					id.length = 0;
					printf("Incorrect ID\nEnter correct ID: ");
					continue;
				}
				break;
			}
			String mail_id;
			error = CreateString(&mail_id, id.value);
			ClearArray(&id);
			if (error) {
				ClearPost(&post);
				ClearArray(&comm);
				return 1;
			}
			if (DeleteMailFromPost(&post, mail_id)) {
				printf("Couldn't find any mail with this ID to delete\n\n");
			} else {
				printf("Successfully deleted mail with ID: %s\n\n", mail_id.data);
			}
			ClearString(&mail_id);
			ResetArray(&comm);
			break;
			case '3':
				printf("ID (14 digits): ");
			if (CreateArray(&id, 10)) {
				ClearPost(&post);
				ClearArray(&comm);
				return 1;
			}
			while (1) {
				if (GetInfo(&id)) {
					ClearPost(&post);
					ClearArray(&comm);
					return 1;
				}
				if (id.length != 14 || !IsAllDigits(id.value)) {
					id.value[0] = '\0';
					id.length = 0;
					printf("Incorrect ID\nEnter correct ID: ");
					continue;
				}
				break;
			}
			error = CreateString(&mail_id, id.value);
			ClearArray(&id);
			if (error) {
				ClearPost(&post);
				ClearArray(&comm);
				return 1;
			}
			Mail *mail;
			int index = 0;
			if (SearchForMailByIdentifier(&post, mail_id, &mail, &index)) {
				printf("Couldn't find any mail with this ID\n\n");
			} else {
				print_mail(*mail);
			}
			ClearString(&mail_id);
			ResetArray(&comm);
			break;
			case '4':
				Mail *found = (Mail *)malloc(sizeof(Mail) * post.length);
			index = 0;
			SearchForMailsInTime(&post, found, &index);
			for (int i = 0; i < index; ++i) {
				print_mail(found[i]);
			}
			if (index == 0) {
				printf("Couldn't find any mail delivered in time\n\n");
			}
			free(found);
			ResetArray(&comm);
			break;
			case '5':
				found = (Mail *)malloc(sizeof(Mail) * post.length);
			index = 0;
			SearchForMailsExpired(&post, found, &index);
			for (int i = 0; i < index; ++i) {
				print_mail(found[i]);
			}
			if (index == 0) {
				printf("Couldn't find any expired mail\n\n");
			}
			free(found);
			ResetArray(&comm);
			break;
			case '6':
				printf("Exiting the program\n");
			ClearPost(&post);
			ClearArray(&comm);
			return 0;
			default:
				break;
		}
		printf("Available commands:\n1. Add mail\n2. Delete mail by ID\n3. Get information about mail by ID\n"
				"4. Search for mails delivered in time\n5. Search for mails that have expired\n6. Exit the program\n"
				"Enter the number of command: ");
	}
}