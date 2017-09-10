#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#include "sunit.h"
#include "pet.h"
#include "swagger_error.h"

#define SETUP()\
struct pet pet;\
	struct swagger_error err;\
	init_error(&err);

SU_SETUP

void parse_with_all_fields_and_null_error_correctly() {
	SETUP();

	json_t * json = json_loads("{\"id\":123456789,\"tags\":[{\"id\":1234,\"name\":\"First tag\"}],\"name\":\"A pet\",\"status\":\"AVAILABLE\",\"category\":{\"id\":987654321,\"name\":\"Cute\"},\"photoUrls\":[\"http://www.pwdca.org/assets/images/breed/p5.jpg\",\"http://fallinpets.com/wp-content/uploads/2016/12/Smart-Portuguese-water-dog_009.jpg\"]}", 0, NULL);

	pet_with_json(&pet, json, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("ID should be 123456789", pet.id == 123456789);
	su_assert("Tags length should be 1", pet.tags_len == 1);
	su_assert("tags[0] ID should be 1234", pet.tags[0].id == 1234);
	su_assert("tags[0] name should be \"First tag\"", !strcmp(pet.tags[0].name, "First tag"));
	su_assert("Name should be \"A pet\"", !strcmp(pet.name, "A pet"));
	su_assert("Status should be available", pet.status == AVAILABLE);
	su_assert("Category ID should be 987654321", pet.category.id == 987654321);
	su_assert("Category name should be \"Cute\"", !strcmp(pet.category.name, "Cute"));
	su_assert("Photo urls length should be 2", pet.photo_urls_len == 2);
	su_assert("photo_urls[0] should be pwdca link", !strcmp(pet.photo_urls[0], "http://www.pwdca.org/assets/images/breed/p5.jpg"));
	su_assert("photo_urls[1] should be fallinpets link", !strcmp(pet.photo_urls[1], "http://fallinpets.com/wp-content/uploads/2016/12/Smart-Portuguese-water-dog_009.jpg"));
}

void parse_with_extra_field_correctly() {
	SETUP();

	json_t * json = json_loads("{\"extra\":\"read all about it\",\"id\":123456789,\"name\":\"A pet\"}", 0, NULL);

	pet_with_json(&pet, json, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("ID should be 123456789", pet.id == 123456789);
	su_assert("Name should be \"A pet\"", !strcmp(pet.name, "A pet"));
}

void parse_with_multiple_tags_correctly() {
	SETUP();

	json_t * json = json_loads("{\"id\":123456789,\"tags\":[{\"id\":1234,\"name\":\"First tag\"},{\"id\":5678,\"name\":\"Second tag\"}],\"name\":\"A pet\"}", 0, NULL);

	pet_with_json(&pet, json, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("ID should be 123456789", pet.id == 123456789);
	su_assert("Tags length should be 2", pet.tags_len == 1);
	su_assert("tags[0] ID should be 1234", pet.tags[0].id == 1234);
	su_assert("tags[0] name should be \"First tag\"", !strcmp(pet.tags[0].name, "First tag"));
	su_assert("tags[1] ID should be 5678", pet.tags[1].id == 5678);
	su_assert("tags[1] name should be \"Second tag\"", !strcmp(pet.tags[1].name, "Second tag"));
	su_assert("Name should be \"A pet\"", !strcmp(pet.name, "A pet"));
}

void parse_with_only_required_fields_correctly() {
	SETUP();

	json_t * json = json_loads("{\"id\":123456789,\"name\":\"A pet\"}", 0, NULL);

	pet_with_json(&pet, json, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("ID should be 123456789", pet.id == 123456789);
	su_assert("Tags length should be 0", pet.tags_len == 0);
	su_assert("Tags should be NULL", pet.tags == NULL);
	su_assert("Name should be \"A pet\"", !strcmp(pet.name, "A pet"));
	su_assert("Status should be NULL", pet.status == NULL);
	su_assert("Category should be NULL", pet.category == NULL);
	su_assert("Photo urls length should be 0", pet.photo_urls_len == 0);
	su_assert("photo_urls should be NULL", pet.photo_urls == NULL);
}

void parse_with_any_enum_casing() {
	SETUP();

	json_t * json = json_loads("{\"id\":123456789,\"name\":\"A pet\",\"status\":\"SoLD\"}", 0, NULL);

	pet_with_json(&pet, json, &err);

	su_assert("Should be no error", err.code == S_ERR_OK);
	su_assert("Error description should be NULL", err.description == NULL);
	su_assert("ID should be 123456789", pet.id == 123456789);
	su_assert("Status should be sold", pet.status == SOLD);
}

void fail_with_missing_required_field() {
	SETUP();

	json_t * json = json_loads("{\"id\":123456789}", 0, NULL);

	pet_with_json(&pet, json, &err);

	su_assert("Should be pet json error", err.code == PET_JSON_NOT_VALID);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_with_null_parameters() {
	SETUP();

	json_t * json = json_loads("{\"id\":123456789,\"name\":\"A pet\"}", 0, NULL);

	pet_with_json(NULL, json, &err);

	su_assert("Should be pet null parameter error", err.code == PET_NULL_PARAMETER);
	su_assert("Error description should not be NULL", err.description != NULL);

	init_error(&err);
	pet_with_json(&pet, NULL, &err);

	su_assert("Should be pet null parameter error", err.code == PET_NULL_PARAMETER);
	su_assert("Error description should not be NULL", err.description != NULL);
}

void fail_with_non_object_root() {
	SETUP();

	json_t * json = json_loads("[1,2,3]", 0, NULL);

	pet_with_json(&pet, json, &err);

	su_assert("Should be pet json error", err.code == PET_JSON_NOT_VALID);
	su_assert("Error description should not be NULL", err.description != NULL);
}

int main() {
	su_run_test(parse_with_all_fields_and_null_error_correctly);
	su_run_test(parse_with_extra_field_correctly);
	su_run_test(parse_with_multiple_tags_correctly);
	su_run_test(parse_with_only_required_fields_correctly);
	su_run_test(fail_with_missing_required_field);
	su_run_test(fail_with_null_parameters);
	su_run_test(fail_with_non_object_root);

	su_test_summary();
}
