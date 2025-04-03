#include <stdio.h>
#include <string.h>

#include "mastr.h"

#include "testing_shared.c"

#define TEST_STEPS 2

int
main(void)
{
#define PART_1 "Hello, "
#define PART_2 "World!"
	const char expected[] = PART_1 PART_2;

	{
		BEGIN_STEP("mastr_string_append()");

		String *str1 = MASTR_STRING(PART_1);
		if(str1 == NULL) {
			STEP_FAIL;
			fprintf(stderr,
					STEP_LOG_PRIMER "MASTR_STRING returned NULL for str1\n");
			return 1;
		}

		String *str2 = MASTR_STRING(PART_2);
		if(str2 == NULL) {
			STEP_FAIL;
			fprintf(stderr,
					STEP_LOG_PRIMER "MASTR_STRING returned NULL for str2\n");
			return 1;
		}

		String *str3 = mastr_string_append(str1, str2);
		if(str3 == NULL) {
			STEP_FAIL;
			fprintf(stderr,
					STEP_LOG_PRIMER "mastr_string_append() returned NULL\n");
			return 1;
		}

		if(strcmp(str3->data, expected) != 0) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER "strcmp returned non-zero value\n");
			fprintf(stderr, STEP_LOG_PRIMER "expected = \"%s\"\n", expected);
			fprintf(stderr, STEP_LOG_PRIMER "str3->data = \"%s\"\n",
					str3->data);
			return 1;
		}

		STEP_SUCCESS;

		if(str3 != str1) {
			fprintf(stderr, STEP_LOG_PRIMER
					"info: mastr_string_append reallocated str1\n");
		}
	}

	{
		BEGIN_STEP("mastr_string_append_cstr()");

		String *str1 = MASTR_STRING(PART_1);
		if(str1 == NULL) {
			STEP_FAIL;
			fprintf(stderr,
					STEP_LOG_PRIMER "MASTR_STRING returned NULL for str1\n");
			return 1;
		}

		String *str2 = mastr_string_append_cstr(str1, PART_2);
		if(str2 == NULL) {
			STEP_FAIL;
			fprintf(stderr,
					STEP_LOG_PRIMER "mastr_string_append() returned NULL\n");
			return 1;
		}

		if(strcmp(str2->data, expected) != 0) {
			STEP_FAIL;
			fprintf(stderr, STEP_LOG_PRIMER "strcmp returned non-zero value\n");
			fprintf(stderr, STEP_LOG_PRIMER "expected = \"%s\"\n", expected);
			fprintf(stderr, STEP_LOG_PRIMER "str3->data = \"%s\"\n",
					str2->data);
			return 1;
		}

		STEP_SUCCESS;

		if(str2 != str1) {
			fprintf(stderr, STEP_LOG_PRIMER
					"info: mastr_string_append reallocated str1\n");
		}
	}

	return 0;
}
