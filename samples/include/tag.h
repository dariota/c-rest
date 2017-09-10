#ifndef TAG_H
#define TAG_H

#define TAG_NULL_PARAMETER 100
#define TAG_JSON_NOT_VALID 101

#include <stdint.h>
#include <jansson.h>

struct tag {
	const char * name;
	int64_t * id;
	int64_t _id;
};

struct tag * tag_from_json(json_t *, struct swagger_error *);

void tag_with_json(struct tag *, json_t *, struct swagger_error *);

void free_tag(struct tag *);

void free_tag_contents(struct tag *);

#endif
