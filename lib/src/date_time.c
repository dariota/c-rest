#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "date_time.h"

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

	char * start = strpbrk(str, "Tt");
	if (start == NULL) {
		d->hour = d->minute = d->second = d->hour_offset = d->minute_offset = 0;
	} else {
		tokens = sscanf(start, "T%d:%d:%d", &d->hour, &d->minute, &d->second);

		if (tokens != 3) {
			ERR_RETURN(err, "Time tokens malformed", S_ERR_TIME_TOKENS);
		}

		start = strpbrk(start, "+-Zz");

		if (start == NULL) {
			ERR_RETURN(err, "Offset start tokens missing", S_ERR_TZ_START);
		}

		if ((start[0] & ~32) == 'Z') {
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
	}
}

void free_date_time(struct date_time * d) {
	if (d == NULL) return;

	free(d);
}
