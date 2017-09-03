#include <stdlib.h>

#include "swagger_error.h"

struct swagger_error * new_error() {
	struct swagger_error * error = malloc(sizeof(struct swagger_error));
	init_error(error);
	return error;
}

void init_error(struct swagger_error * error) {
	if (error == NULL) return;

	error->code = S_ERR_OK;
	error->description = NULL;
}

void free_error(struct swagger_error * error) {
	if (error == NULL) return;

	free(error);
}
