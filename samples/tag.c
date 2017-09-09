#include <stdlib.h>

#include "swagger_error.h"

#include "tag.h"

struct tag * tag_from_json(json_t * root, struct swagger_error * error) {
	struct tag * tag = malloc(sizeof(struct tag));
	tag_with_json(tag, root, error);
	return tag;
}

void tag_with_json(struct tag * tag, json_t * root, struct swagger_error * error) {
	struct swagger_error _error;
	if (!error) {
		error = &_error;
	}

	// validate parameters
	if (!tag) {
		ERR_RETURN(error, "tag must not be null", TAG_NULL_PARAMETER);
	}
	if (!root) {
		ERR_RETURN(error, "root must not be null", TAG_NULL_PARAMETER);
	}

	if (!json_is_object(root)) {
		ERR_RETURN(error, "root not object", TAG_JSON_NOT_VALID);
	}

	// retrieve and validate name
	json_t * current_key = json_object_get(root, "name");
	if (!current_key) {
		// TODO this shouldn't fail
		ERR_RETURN(error, "name not present", TAG_JSON_NOT_VALID);
	}
	if (!json_is_string(current_key)) {
		ERR_RETURN(error, "name not valid", TAG_JSON_NOT_VALID);
	}
	tag->name = json_string_value(current_key);

	// retrieve and validate id
	current_key = json_object_get(root, "id");
	if (!current_key) {
		// TODO this shouldn't fail
		ERR_RETURN(error, "id not present", TAG_JSON_NOT_VALID);
	}
	if (!json_is_integer(current_key)) {
		ERR_RETURN(error, "id not valid", TAG_JSON_NOT_VALID);
	}
	tag->id = json_integer_value(current_key);
}

void free_tag(struct tag * tag) {
	if (tag) {
		free_tag_contents(tag);
		free(tag);
	}
}

void free_tag_contents(struct tag * tag) {
	// nothing to FREE here :D
}
