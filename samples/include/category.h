#ifndef CATEGORY_H
#define CATEGORY_H

#define CATEGORY_NULL_PARAMETER 104
#define CATEGORY_JSON_NOT_VALID 105

#include <stdint.h>
#include <jansson.h>

struct category {
	const char * name;
	int64_t id;
};

struct category * category_from_json(json_t *, struct swagger_error *);

void category_with_json(struct category *, json_t *, struct swagger_error *);

void free_category(struct category *);

void free_category_contents(struct category *);

#endif
