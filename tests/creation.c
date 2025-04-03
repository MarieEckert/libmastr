#include <stdio.h>
#include <string.h>

#include "mastr.h"

#include "testing_shared.c"

#define TEST_STEPS 3

int
main(void)
{
	{ /* mastr_string_new */
		BEGIN_STEP("mastr_string_new() with 512 bytes");

		String *str = mastr_string_new(512);

		if(str == NULL) {
			STEP_FAIL;
			return 1;
		}

		STEP_SUCCESS;
	}

	String *mastr_string = NULL;

	{ /* mastr_string_from */
		BEGIN_STEP("mastr_string_from_cstr() / MASTR_STRING");

		const char original[] = "0123 4567 89ab cdef";
		mastr_string = MASTR_STRING(original);

		if(mastr_string == NULL) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER "MASTR_STRING returned NULL\n");
			return 1;
		}

		if(mastr_string->length != sizeof(original) - 1) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER
					"length mismatch between original and mastr String\n");
			fprintf(stderr,
					STEP_LOG_PRIMER "original length: %zu; str->length: %u\n",
					sizeof(original) - 1, mastr_string->length);
			return 1;
		}

		if(strcmp(mastr_string->data, original) != 0) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER "strcmp returned non-zero value\n");
			fprintf(stderr, STEP_LOG_PRIMER "original = \"%s\"\n", original);
			fprintf(stderr, STEP_LOG_PRIMER "str->data = \"%s\"\n",
					mastr_string->data);
			return 1;
		}

		STEP_SUCCESS;
	}

	{ /* mastr_string_from */
		BEGIN_STEP("mastr_string_from()");

		String *str = mastr_string_from(mastr_string);

		if(str == NULL) {
			STEP_FAIL;
			fprintf(stderr,
					STEP_LOG_PRIMER "mastr_string_from() returned NULL\n");
			return 1;
		}

		if(str->length != mastr_string->length) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER
					"length mismatch between original and mastr String\n");
			fprintf(stderr,
					STEP_LOG_PRIMER "original length: %u; str->length: %u\n",
					mastr_string->length, str->length);
			return 1;
		}

		if(strcmp(str->data, mastr_string->data) != 0) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER "strcmp returned non-zero value\n");
			fprintf(stderr, STEP_LOG_PRIMER "original = \"%s\"\n",
					mastr_string->data);
			fprintf(stderr, STEP_LOG_PRIMER "str->data = \"%s\"\n", str->data);
			return 1;
		}

		STEP_SUCCESS;
	}

	return 0;
}
