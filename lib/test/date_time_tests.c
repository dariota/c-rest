#include <stdlib.h>

#include "../include/date_time.h"
#include "include/sunit.h"

#define SETUP() \
struct date_time date;\
struct swagger_error err;\
init_error(&err)

SU_SETUP

void parse_date_only_correctly() {
	SETUP();

	date_from_str("1970-12-23", &date, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("Year should be 1970", date.year == 1970);
	su_assert("Month should be 12", date.month == 12);
	su_assert("Day should be 23", date.day == 23);
	su_assert("Hour should be 0", date.hour == 0);
	su_assert("Minute should be 0", date.minute == 0);
	su_assert("Second should be 0", date.second == 0);
	su_assert("Offset sign should be 0", date.offset_sign == 0);
	su_assert("Hour offset should be 0", date.hour_offset == 0);
	su_assert("Minute offset should be 0", date.minute_offset == 0);
}

void parse_date_and_time() {
	SETUP();

	date_from_str("1970-12-23T08:32:10Z", &date, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("Year should be 1970", date.year == 1970);
	su_assert("Month should be 12", date.month == 12);
	su_assert("Day should be 23", date.day == 23);
	su_assert("Hour should be 8", date.hour == 8);
	su_assert("Minute should be 32", date.minute == 32);
	su_assert("Second should be 10", date.second == 10);
	su_assert("Offset sign should be 1", date.offset_sign == 1);
	su_assert("Hour offset should be 0", date.hour_offset == 0);
	su_assert("Minute offset should be 0", date.minute_offset == 0);
}

void parse_negative_offset() {
	SETUP();

	date_from_str("1970-12-23T08:32:10-01:23", &date, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("Year should be 1970", date.year == 1970);
	su_assert("Month should be 12", date.month == 12);
	su_assert("Day should be 23", date.day == 23);
	su_assert("Hour should be 8", date.hour == 8);
	su_assert("Minute should be 32", date.minute == 32);
	su_assert("Second should be 10", date.second == 10);
	su_assert("Offset sign should be -1", date.offset_sign == -1);
	su_assert("Hour offset should be 1", date.hour_offset == 1);
	su_assert("Minute offset should be 23", date.minute_offset == 23);
}

void parse_positive_offset() {
	SETUP();

	date_from_str("1970-12-23T08:32:10+02:34", &date, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("Year should be 1970", date.year == 1970);
	su_assert("Month should be 12", date.month == 12);
	su_assert("Day should be 23", date.day == 23);
	su_assert("Hour should be 8", date.hour == 8);
	su_assert("Minute should be 32", date.minute == 32);
	su_assert("Second should be 10", date.second == 10);
	su_assert("Offset sign should be 1", date.offset_sign == 1);
	su_assert("Hour offset should be 2", date.hour_offset == 2);
	su_assert("Minute offset should be 34", date.minute_offset == 34);
}

void parse_utc_offset() {
	SETUP();

	date_from_str("1970-12-23T08:32:10Z", &date, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("Year should be 1970", date.year == 1970);
	su_assert("Month should be 12", date.month == 12);
	su_assert("Day should be 23", date.day == 23);
	su_assert("Hour should be 8", date.hour == 8);
	su_assert("Minute should be 32", date.minute == 32);
	su_assert("Second should be 10", date.second == 10);
	su_assert("Offset sign should be 1", date.offset_sign == 1);
	su_assert("Hour offset should be 0", date.hour_offset == 0);
	su_assert("Minute offset should be 0", date.minute_offset == 0);
}

void parse_date_and_time_with_null_error_should_work() {
	SETUP();

	date_from_str("1970-12-23T08:32:10Z", &date, NULL);

	su_assert("Year should be 1970", date.year == 1970);
	su_assert("Month should be 12", date.month == 12);
	su_assert("Day should be 23", date.day == 23);
	su_assert("Hour should be 8", date.hour == 8);
	su_assert("Minute should be 32", date.minute == 32);
	su_assert("Second should be 10", date.second == 10);
	su_assert("Offset sign should be 1", date.offset_sign == 1);
	su_assert("Hour offset should be 0", date.hour_offset == 0);
	su_assert("Minute offset should be 0", date.minute_offset == 0);
}

void fail_null_date_string() {
	SETUP();

	date_from_str(NULL, &date, &err);

	su_assert("Should be date string error", err.code == S_ERR_DATE_STRING_NULL);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_null_date_struct() {
	SETUP();

	date_from_str("1970-01-01", NULL, &err);

	su_assert("Should be date struct error", err.code == S_ERR_DATE_NULL);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_invalid_date_portion() {
	SETUP();

	date_from_str("Hello", &date, &err);

	su_assert("Should be date error", err.code == S_ERR_DATE_TOKENS);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_invalid_time_portion() {
	SETUP();

	date_from_str("2015-01-02THello", &date, &err);

	su_assert("Should be time error", err.code == S_ERR_TIME_TOKENS);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_invalid_time_offset_sign() {
	SETUP();

	date_from_str("2015-01-02T01:02:03|01:00", &date, &err);

	su_assert("Should be timezone start error", err.code == S_ERR_TZ_START);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_invalid_time_offset_part() {
	SETUP();

	date_from_str("2015-01-02T01:02:03+00", &date, &err);

	su_assert("Should be time offset error", err.code == S_ERR_TZ_OFFSET);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_invalid_trailing_chars() {
	SETUP();

	date_from_str("1234-01-02T03:04:05+06:07Z", &date, &err);

	su_assert("Should be spurious characters error",
	          err.code == S_ERR_DATE_SPURIOUS_CHARS);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_invalid_separator_char() {
	SETUP();

	date_from_str("1234-01-02A03:04:05Z", &date, &err);

	su_assert("Should be invalid separator error",
	          err.code == S_ERR_DATE_INVALID_SEPARATOR);
	su_assert("Error description should not be NULL", err.description != NULL);
}

int main() {
	su_run_test(parse_date_only_correctly);
	su_run_test(parse_date_and_time);
	su_run_test(parse_negative_offset);
	su_run_test(parse_positive_offset);
	su_run_test(parse_utc_offset);
	su_run_test(parse_date_and_time_with_null_error_should_work);
	su_run_test(fail_null_date_string);
	su_run_test(fail_null_date_struct);
	su_run_test(fail_invalid_date_portion);
	su_run_test(fail_invalid_time_portion);
	su_run_test(fail_invalid_time_offset_sign);
	su_run_test(fail_invalid_time_offset_part);
	su_run_test(fail_invalid_trailing_chars);
	su_run_test(fail_invalid_separator_char);

	su_test_summary();
}
