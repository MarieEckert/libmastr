#include <stdio.h>

#include "mastr.h"

int
main(void)
{
	String *str1 = mastr_string_from_cstr("Hello, World!");
	printf("str1: %s\n", str1->data);

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

	return 0;
}
