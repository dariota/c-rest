#ifndef PET_H
#define PET_H

#define PET_NULL_PARAMETER 102
#define PET_JSON_NOT_VALID 103

#include <stdint.h>
#include <jansson.h>

#include "swagger_error.h"

#include "tag.h"
#include "category.h"

enum pet_status { AVAILABLE, PENDING, SOLD };

// A pet is a person's best friend
struct pet {
	int64_t id;
	struct tag * tags;
	size_t tags_len;
	const char * name;
	enum pet_status status;
	struct category category;
	const char ** photo_urls;
	size_t photo_urls_len;
};

struct pet * pet_from_json(json_t *, struct swagger_error *);

void pet_with_json(struct pet *, json_t *, struct swagger_error *);

void free_pet(struct pet *);

void free_pet_contents(struct pet *);

#endif
