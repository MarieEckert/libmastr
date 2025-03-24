/* libmastr -- a string library
 *
 * This library provides a dynamic, pascal-style, string type
 * (which also contains a NULL-terminator to be compatible with most of C)
 * and functions for working with it. Functions for simple UTF-8 handling are
 * also planned.
 *
 * libmastr is written in c17 and should not rely on any extensions to this
 * standard.
 *
 * ============================================================================
 *
 * Copyright (C) 2025, Marie Eckert
 * Licensed under the BSD 3-Clause License.
 */

#ifndef ME_LIBMASTR_H
#define ME_LIBMASTR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MASTR_PP_STR(x)		  #x
#define MASTR_PP_STRINGIFY(x) STR(x)

#define MASTR_VERSION_MINOR	  0
#define MASTR_VERSION_MAJOR	  0
#define MASTR_VERSION_STRING                   \
	MASTR_PP_STRINGIFY(LIBMASTR_VERSION_MAJOR) \
	"." MASTR_PP_STRINGIFY(LIBMASTR_VERSION_MINOR)

/* Regular Dynamic String */

/**
 * @brief Utility macro for constructing a heap-allocated String
 * from a string-literal.
 */
#define MASTR_STRING(str) mastr_string_from_cstr(str)

typedef struct {
	uint32_t capacity;
	uint32_t length;
	char data[];
} String;

/**
 * @brief Create a new String with the given byte capacity.
 */
String *mastr_string_new(uint32_t byteCapacity);

/**
 * @brief Constructs a String with the contents of the given String.
 */
String *mastr_string_from(const String *original);

/**
 * @brief Constructs a String with the contents of the given C-String.
 */
String *mastr_string_from_cstr(const char *original);

/**
 * @brief Appends the contents of String b to String a.
 */
String *mastr_string_append(String *a, const String *b);

/**
 * @brief Appends the contents of C-String b to String a.
 */
String *mastr_string_append_cstr(String *a, const char *b);

/**
 * @brief Resize the given string so that its capacity is equal to
 * or greater than newSize. The newSize will be aligned to the value
 * of the MASTR_RESIZE_ALIGNMENT preprocessor macro.
 */
String *mastr_string_resize(String *a, uint32_t newSize);

/* refcounted strings */

#define MASTR_RCSTRING_USE(str) \
	do {                        \
		str.refcount++;         \
	} while(0)

#define MASTR_RCSTRING_DONE(str) \
	do {                         \
		if(str.refcount > 0) {   \
			str.refcount--;      \
		}                        \
		if(str.refcount == 0) {  \
			free(str.string);    \
		}                        \
	} while(0)

#define MASTR_CONSTRUCT_RCSTRING(str) \
	(RCString)                        \
	{                                 \
		.refcount = 0, .string = str, \
	}

#define MASTR_CONSTRUCT_RCSTRING_RESULT(rcstring)               \
	(RCStringResult)                                            \
	{                                                           \
		.hasValue = rcstring.string != NULL, .value = rcstring, \
	}

/**
 * @brief Utility macro to construct a RCString with heap allocated data
 * from a string literal.
 */
#define MASTR_RCSTRING(str) mastr_rcstring_from_cstr(str)

/**
 * @brief Container Type for a String for enabling reference-counting.
 * To properly utilise this call the MASTR_RCSTRING_USE macro with the RCString
 * as its parameter at the start of the scope where it is used and call the
 * MASTR_RCSTRING_DONE macro when leaving the scope.
 */
typedef struct {
	uint32_t refcount;
	String *string;
} RCString;

/**
 * @brief Result type for functions returning an RCString. Used to avoid
 * NULL checks and having RCStrings as pointers.
 */
typedef struct {
	bool hasValue;
	RCString value;
} RCStringResult;

/**
 * @brief Create a new RCString with the given byte capacity. The
 * initial reference count will be set to 0.
 */
RCStringResult mastr_rcstring_new(uint32_t byteCapacity);

/**
 * @brief Constructs a RCString with the contents of the given String.
 */
RCStringResult mastr_rcstring_from(const String *original);

/**
 * @brief Constructs a RCString with the contents of the given C-String.
 */
RCStringResult mastr_rcstring_from_cstr(const char *original);

/**
 * @brief Appends the contents of RCString b to RCString a.
 * @note This function does not affect the reference count of its parameters.
 */
RCStringResult mastr_rcstring_append(RCString a, RCString b);

/**
 * @brief Appends the contents of C-String b to RCString a.
 * @note This function does not affect the reference count of its parameters.
 */
RCStringResult mastr_rcstring_append_cstr(RCString a, const char *b);

#endif
