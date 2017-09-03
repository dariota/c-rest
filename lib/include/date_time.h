#ifndef DATE_TIME_H
#define DATE_TIME_H

#define S_ERR_DATE_STRING_NULL       1
#define S_ERR_DATE_NULL              2
#define S_ERR_DATE_TOKENS            3
#define S_ERR_TIME_TOKENS            4
#define S_ERR_TZ_START               5
#define S_ERR_TZ_OFFSET              6
#define S_ERR_DATE_SPURIOUS_CHARS    7
#define S_ERR_DATE_INVALID_SEPARATOR 8

#include <stdbool.h>

#include "swagger_error.h"

struct date_time {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int offset_sign;
	int hour_offset;
	int minute_offset;
};

struct date_time * new_date_from_str(const char *, struct swagger_error *);

void date_from_str(const char *, struct date_time *, struct swagger_error *);

void free_date_time(struct date_time *);

#endif
