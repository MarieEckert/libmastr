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
 * Copyright (C) 2024  Marie Eckert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

typedef struct {
	uint32_t capacity;
	uint32_t length;
	char data[];
} String;

String *mastr_string_new(uint32_t byteCapacity);

String *mastr_string_from(const String *original);

String *mastr_string_from_cstr(const char *original);

String *mastr_string_append(String *a, const String *b);

String *mastr_string_append_cstr(String *a, const char *b);

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

RCStringResult mastr_rcstring_from(const String *original);

RCStringResult mastr_rcstring_from_cstr(const char *original);

RCStringResult mastr_rcstring_append(RCString a, RCString b);

RCStringResult mastr_rcstring_append_cstr(RCString a, const char *b);

#endif
