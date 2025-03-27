# libmastr

libmastr is a library that brings datastructures and functions for creating and managing
Pascal-Style strings and reference counted strings (RCStrings).

## Building & Installation

libmastr is built using [mariebuild](https://github.com/MarieEckert/mariebuild) and clang by default.

libmastr can be installed using the `install` target. The default installation prefix for this is `/usr`,
but it can be overriden by setting the `INSTALL_PREFIX` environment variable.

## Example
```c
#include <mastr.h>
#include <stdio.h>

int
main(void)
{
	/* Create a String from a C-String */
	String *hello = MASTR_STRING("Hello, ");
	hello = mastr_string_append_cstr(hello, "World!\n");

	/* To directly access the NULL-Terminated string data, simply access
	 * the data field of a String.
	 * The current length in bytes can be accessed using the length field
	 * and the capacity in bytes can be accessed using the capacity field.
	 */
	puts(hello);

	/* Create an empty string with an initial capacity of 8 bytes */
	String *sized_example = mastr_string_new(8);
	sized_example = mastr_string_append(sized_example, hello);

	puts(sized_example);
	free(sized_example); /* Strings can be freed using the regular free() function */

	/* Using reference counted strings */

	/* The initial reference count of a RCString is 1 if created by this function
	 * or using the MASTR_CONSTRUCT_RCSTRING macro.
	 */
	RCStringResult maybe_rcstr1 = mastr_rcstring_new(16);

	/* Functions which return a RCString typically use the RCStringResult container,
	 * it is recommended to check if hasValue is true before using the RCString contained
	 * within value to avoid NULL dereferences.
	 */
	if(!maybe_rcstr1.hasValue) {
		return 1;
	}

	RCString rcstr1 = maybe_rcstr1.value;

	/* As seen below, RCStrings can be constructed from existing regular strings
	 * using the MASTR_CONSTRUCT_RCSTRING macro. This does little more than create
	 * an RCString with the given String pointer as its value
	 */
	maybe_rcstr1 =
		mastr_rcstring_append(rcstr1, MASTR_CONSTRUCT_RCSTRING(hello));
	if(!maybe_rcstr1.hasValue) {
		return 2;
	}

	puts(rcstr1.string->data);

	/* When leaving a scope where a rcstring is used it typically makes sense
	 * to pass it to the MASTR_RCSTRING_DONE macro. This macro decrements the
	 * reference count and frees the string if it reaches or is already 0.
	 *
	 * When entering a scope use the MASTR_RCSTRING_USE macro which increments
	 * the reference count by 1.
	 */
	MASTR_RCSTRING_DONE(rcstr1);
	free(hello);

	return 0;
}
````
