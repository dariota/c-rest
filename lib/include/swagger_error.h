#ifndef SWAGGER_ERROR_H
#define SWAGGER_ERROR_H

#define S_ERR_OK 0

#define ERR_RETURN(X, Y, Z) \
do {\
	struct swagger_error * _err = X;\
	_err->description = Y;\
	_err->code = Z;\
	return;\
} while (0)

struct swagger_error {
	char * description;
	int code;
};

struct swagger_error * new_error();

void init_error(struct swagger_error * error);

void free_error(struct swagger_error * error);

#endif
