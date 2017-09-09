#include <stdlib.h>

#include "swagger_error.h"

#include "category.h"

struct category * category_from_json(json_t * root, struct swagger_error * error) {
	struct category * category = malloc(sizeof(struct category));
	category_with_json(category, root, error);
	return category;
}

void category_with_json(struct category * category, json_t * root, struct swagger_error * error) {
	struct swagger_error _error;
	if (!error) {
		error = &_error;
	}

	// validate parameters
	if (!category) {
		ERR_RETURN(error, "category must not be null", CATEGORY_NULL_PARAMETER);
	}
	if (!root) {
		ERR_RETURN(error, "root must not be null", CATEGORY_NULL_PARAMETER);
	}

	if (!json_is_object(root)) {
		ERR_RETURN(error, "root not object", CATEGORY_JSON_NOT_VALID);
	}

	// retrieve and validate name
	json_t * current_key = json_object_get(root, "name");
	if (!current_key) {
		// TODO this shouldn't fail
		ERR_RETURN(error, "name not present", CATEGORY_JSON_NOT_VALID);
	}
	if (!json_is_string(current_key)) {
		ERR_RETURN(error, "name not valid", CATEGORY_JSON_NOT_VALID);
	}
	category->name = json_string_value(current_key);

	// retrieve and validate id
	current_key = json_object_get(root, "id");
	if (!current_key) {
		// TODO this shouldn't fail
		ERR_RETURN(error, "id not present", CATEGORY_JSON_NOT_VALID);
	}
	if (!json_is_integer(current_key)) {
		ERR_RETURN(error, "id not valid", CATEGORY_JSON_NOT_VALID);
	}
	category->id = json_integer_value(current_key);
}

void free_category(struct category * category) {
	if (category) {
		free_category_contents(category);
		free(category);
	}
}

void free_category_contents(struct category * category) {
	// nothing to FREE here :D
}
