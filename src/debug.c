#include <stdio.h>

#include "mastr.h"

#define RCSTRINGRESULT_CHECK(rcs, dest, tag)   \
	do {                                       \
		if(!rcs.hasValue) {                    \
			fprintf(stderr, tag " failed!\n"); \
			return 1;                          \
		}                                      \
		dest = rcs.value;                      \
	} while(0)

int
main(void)
{
	String *str1 = mastr_string_from_cstr("►Hello, World!");
	printf("str1: %s\n> length = %zu\n", str1->data, mastr_utf8_strlen(str1));

	String *str2 = mastr_string_from_cstr("Test");
	printf("str2: %s\n", str2->data);

	String *str3 = mastr_string_new(8);
	str3 = mastr_string_append(str3, str1);
	if(str3 == NULL) {
		fprintf(stderr, "append 1 failed\n");
		return 1;
	}
	str3 = mastr_string_append_cstr(str3, " ");
	if(str3 == NULL) {
		fprintf(stderr, "append 2 failed\n");
		return 1;
	}
	str3 = mastr_string_append(str3, str2);
	if(str3 == NULL) {
		fprintf(stderr, "append 3 failed\n");
		return 1;
	}

	printf("str3: %s\n", str3->data);

	RCString rcstr1;
	RCStringResult maybe_rcstr1 = mastr_rcstring_new(16);
	RCSTRINGRESULT_CHECK(maybe_rcstr1, rcstr1, "rcstring_new 1");

	maybe_rcstr1 =
		mastr_rcstring_append(rcstr1, MASTR_CONSTRUCT_RCSTRING(str3));
	RCSTRINGRESULT_CHECK(maybe_rcstr1, rcstr1, "rcstring_append 1");

	printf("rcstr1: %s\n", rcstr1.string->data);

	MASTR_RCSTRING_DONE(rcstr1);
	free(str1);
	free(str2);
	free(str3);

	return 0;
}
