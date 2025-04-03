#include <stdio.h>
#include <string.h>

#include "mastr.h"

#include "testing_shared.c"

#define TEST_STEPS 3

int
main(void)
{
	{ /* mastr_rcstring_new */
		BEGIN_STEP("mastr_rcstring_new() with 512 bytes");

		RCStringResult str_res = mastr_rcstring_new(512);

		if(!str_res.hasValue) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER
					"mastr_rcstring_new returned a value-less result\n");
			return 1;
		}

		RCString str = str_res.value;

		if(str.string == NULL) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER
					"mastr_rcstring_new returned a NULL string but the result "
					"claimed to have a value\n");
			return 1;
		}

		STEP_SUCCESS;
	}

	RCString mastr_string = MASTR_CONSTRUCT_RCSTRING(NULL);

	{ /* mastr_rcstring_from_cstr */
		BEGIN_STEP("mastr_rcstring_from_cstr() / MASTR_STRING");

		const char original[] = "0123 4567 89ab cdef";
		RCStringResult mastr_string_res = MASTR_RCSTRING(original);

		if(!mastr_string_res.hasValue) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER
					"MASTR_RCSTRING returned a value-less result\n");
			return 1;
		}

		mastr_string = mastr_string_res.value;

		if(mastr_string.string == NULL) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER "MASTR_STRING returned NULL\n");
			return 1;
		}

		if(mastr_string.string->length != sizeof(original) - 1) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER
					"length mismatch between original and mastr String\n");
			fprintf(stderr,
					STEP_LOG_PRIMER "original length: %zu; str->length: %u\n",
					sizeof(original) - 1, mastr_string.string->length);
			return 1;
		}

		if(strcmp(mastr_string.string->data, original) != 0) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER "strcmp returned non-zero value\n");
			fprintf(stderr, STEP_LOG_PRIMER "original = \"%s\"\n", original);
			fprintf(stderr, STEP_LOG_PRIMER "str->data = \"%s\"\n",
					mastr_string.string->data);
			return 1;
		}

		STEP_SUCCESS;
	}

	{ /* mastr_rcstring_from */
		BEGIN_STEP("mastr_rcstring_from()");

		RCStringResult str_res = mastr_rcstring_from(mastr_string.string);

		if(!str_res.hasValue) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER
					"mastr_rcstring_from() returned a value-less result\n");
			return 1;
		}

		RCString str = str_res.value;

		if(str.string == NULL) {
			STEP_FAIL;
			fprintf(stderr,
					STEP_LOG_PRIMER "mastr_string_from() returned NULL\n");
			return 1;
		}

		if(str.string->length != mastr_string.string->length) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER
					"length mismatch between original and mastr String\n");
			fprintf(stderr,
					STEP_LOG_PRIMER "original length: %u; str->length: %u\n",
					mastr_string.string->length, str.string->length);
			return 1;
		}

		if(strcmp(str.string->data, mastr_string.string->data) != 0) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER "strcmp returned non-zero value\n");
			fprintf(stderr, STEP_LOG_PRIMER "original = \"%s\"\n",
					mastr_string.string->data);
			fprintf(stderr, STEP_LOG_PRIMER "str->data = \"%s\"\n",
					str.string->data);
			return 1;
		}

		STEP_SUCCESS;
	}

	return 0;
}
