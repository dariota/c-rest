#include <stdlib.h>
#include <string.h>

#include "swagger_error.h"

#include "pet.h"

struct pet * pet_from_json(json_t * root, struct swagger_error * error) {
	struct pet * pet = malloc(sizeof(struct pet));
	pet_with_json(pet, root, error);
	return pet;
}

void pet_with_json(struct pet * pet, json_t * root, struct swagger_error * error) {
	struct swagger_error _error;
	if (!error) {
		error = &_error;
	}

	// validate parameters
	if (!pet) {
		ERR_RETURN(error, "pet must not be null", PET_NULL_PARAMETER);
	}
	if (!root) {
		ERR_RETURN(error, "pet root must not be null", PET_NULL_PARAMETER);
	}

	if (!json_is_object(root)) {
		ERR_RETURN(error, "pet root not object", PET_JSON_NOT_VALID);
	}

	// retrieve and validate id
	json_t * current_key = json_object_get(root, "id");
	if (!current_key) {
		ERR_RETURN(error, "pet id not present", PET_JSON_NOT_VALID);
	}
	if (!json_is_integer(current_key)) {
		ERR_RETURN(error, "pet id not valid", PET_JSON_NOT_VALID);
	}
	pet->id = json_integer_value(current_key);

	// retrieve and validate tags
	current_key = json_object_get(root, "tags");
	if (!current_key) {
		pet->tags_len = 0;
		pet->tags = NULL;
	} else {
		if (!json_is_array(current_key)) {
			ERR_RETURN(error, "pet tags not valid", PET_JSON_NOT_VALID);
		}

		pet->tags_len = json_array_size(current_key);
		pet->tags = malloc(sizeof(struct tag) * pet->tags_len);
		error->code = S_ERR_OK;
		for (int i = 0; i < pet->tags_len; i++) {
			json_t * current_tags = json_array_get(current_key, i);
			tag_with_json(&pet->tags[i], current_tags, error);
			if (error->code != S_ERR_OK) return;
		}
	}

	// retrieve and validate name
	current_key = json_object_get(root, "name");
	if (!current_key) {
		ERR_RETURN(error, "pet name not present", PET_JSON_NOT_VALID);
	}
	if (!json_is_string(current_key)) {
		ERR_RETURN(error, "pet name not valid", PET_JSON_NOT_VALID);
	}
	pet->name = json_string_value(current_key);

	// retrieve and validate status
	current_key = json_object_get(root, "status");
	if (!current_key) {
		pet->status = NULL;
	} else {
		if (!json_is_string(current_key)) {
			ERR_RETURN(error, "pet status not valid", PET_JSON_NOT_VALID);
		}

		const char * status = json_string_value(current_key);
		pet->status = &(pet->_status);
		if (!strcmp(status, "AVAILABLE")) {
			*(pet->status) = AVAILABLE;
		} else if (!strcmp(status, "PENDING")) {
			*(pet->status) = PENDING;
		} else if (!strcmp(status, "SOLD")) {
			*(pet->status) = SOLD;
		} else {
			ERR_RETURN(error, "pet status not valid", PET_JSON_NOT_VALID);
		}
	}

	// retrieve and validate category
	current_key = json_object_get(root, "category");
	if (!current_key) {
		pet->category = NULL;
	} else {
		pet->category = &(pet->_category);
		error->code = S_ERR_OK;
		category_with_json(pet->category, current_key, error);
		if (error->code != S_ERR_OK) return;
	}

	// retrieve and validate photo_urls
	current_key = json_object_get(root, "photoUrls");
	if (!current_key) {
		pet->photo_urls_len = 0;
		pet->photo_urls = NULL;
	} else {
		if (!json_is_array(current_key)) {
			ERR_RETURN(error, "pet photoUrls not valid", PET_JSON_NOT_VALID);
		}

		pet->photo_urls_len = json_array_size(current_key);
		pet->photo_urls = malloc(sizeof(char *) * pet->photo_urls_len);
		for (int i = 0; i < pet->photo_urls_len; i++) {
			json_t * current_photo_urls = json_array_get(current_key, i);
			if (!json_is_string(current_photo_urls)) {
				ERR_RETURN(error, "pet photoUrls content not valid", PET_JSON_NOT_VALID);
			}
			pet->photo_urls[i] = json_string_value(current_photo_urls);
		}
	}
}

void free_pet(struct pet * pet) {
	if (!pet) return;

	free_pet_contents(pet);

	free(pet);
}

void free_pet_contents(struct pet * pet) {
	if (!pet) return;

	if (pet->tags) {
		for (int i = 0; i < pet->tags_len; i++) {
			free_tag_contents(&pet->tags[i]);
		}
		free(pet->tags);
	}

	if (pet->category) {
		free_category_contents(pet->category);
	}

	if (pet->photo_urls) {
		free(pet->photo_urls);
	}
}
