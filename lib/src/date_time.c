#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "date_time.h"

static const char * date_find_separator(const char *);
static const char * date_find_end(const char *);
static const char * date_skip_chars(const char *, char, int);

struct date_time * new_date_from_str(const char * str,
                                     struct swagger_error * err) {
	struct date_time * d = malloc(sizeof(struct date_time));
	date_from_str(str, d, err);
	return d;
}

void date_from_str(const char * str, struct date_time * d,
                   struct swagger_error * err) {
	struct swagger_error null_save;
	if (err == NULL) err = &null_save;

	if (str == NULL) {
		ERR_RETURN(err, "Date string must not be null", S_ERR_DATE_STRING_NULL);
	}

	if (d == NULL) {
		ERR_RETURN(err, "Date struct must not be null", S_ERR_DATE_NULL);
	}

	int tokens = sscanf(str, "%d-%d-%d", &d->year, &d->month, &d->day);

	if (tokens != 3) {
		ERR_RETURN(err, "Date tokens malformed", S_ERR_DATE_TOKENS);
	}

	const char * start = date_find_separator(str);
	if (start == NULL) {
		ERR_RETURN(err, "Invalid separator", S_ERR_DATE_INVALID_SEPARATOR);
	} else if (*start == '\0') {
		d->hour = d->minute = d->second = d->offset_sign = d->hour_offset = d->minute_offset = 0;
	} else {
		tokens = sscanf(&start[1], "%d:%d:%d", &d->hour, &d->minute, &d->second);

		if (tokens != 3) {
			ERR_RETURN(err, "Time tokens malformed", S_ERR_TIME_TOKENS);
		}

		start = strpbrk(start, "+-Zz");

		if (start == NULL) {
			ERR_RETURN(err, "Offset start tokens missing", S_ERR_TZ_START);
		}

		if ((start[0] & ~32) == 'Z') {
			d->offset_sign = 1;
			d->hour_offset = d->minute_offset = 0;
		} else {
			tokens = sscanf(&start[1], "%d:%d", &d->hour_offset,
			                &d->minute_offset);

			if (tokens != 2) {
				ERR_RETURN(err, "Offset tokens malformed", S_ERR_TZ_OFFSET);
			}

			if (start[0] == '-') {
				d->offset_sign = -1;
			} else {
				d->offset_sign = 1;
			}
		}

		start = date_find_end(&start[1]);
		if (start == NULL || *start != '\0') {
			ERR_RETURN(err, "Spurious characters at end of date string",
			           S_ERR_DATE_SPURIOUS_CHARS);
		}
	}
}

void free_date_time(struct date_time * d) {
	if (d == NULL) return;

	free(d);
}

static const char * date_find_separator(const char * str) {
	const char * separator = date_skip_chars(str, '-', 2);

	if (separator != NULL) {
		char c = *separator;
		if (c == 'T' || c == 't' || c == '\0') {
			return separator;
		}
	}

	return NULL;
}

static const char * date_find_end(const char * str) {
	const char * tz = str;

	if (tz[0] == 'z' || tz[0] == 'Z') {
		return tz + 1;
	}

	return date_skip_chars(tz, ':', 1);
}

static const char * date_skip_chars(const char * p, char expected, int occurs) {
	int actual_occurs = 0;

	for (char curr_char = *p; curr_char != '\0'; curr_char = *++p) {
		if (curr_char == expected) {
			actual_occurs++;
			if (actual_occurs > occurs) {
				return NULL;
			}
		} else if (curr_char < '0' || curr_char > '9') {
			return p;
		}
	}

	return p;
}
