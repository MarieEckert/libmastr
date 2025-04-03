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

#include <sys/types.h>

#define MASTR_VERSION "1.0.0"

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

ssize_t mastr_strstr(const String *haystack, const String *needle);

ssize_t mastr_strstr_cstr(const String *haystack, const char *needle);

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

/* unicode support */

/**
 * @brief Convert the given UTF-32 encoded Code-Point to its UTF-8
 * representation.
 * @param out_utf8_char The char array to write the UTF-8 bytes and a NULL
 * Terminator to. Needs to be atleast 5 bytes large.
 * @return The amount of bytes written (not including the NULL Terminator).
 */
size_t mastr_utf32_to_utf8_char(uint32_t utf32, char out_utf8_char[static 5]);

/**
 * @brief Get the length/number of codepoints of a UTF-8 encoded string.
 * @note Does not resolve Grapheme Clusters, this literally just counts the
 * codepoints.
 */
size_t mastr_utf8_strlen(const String *string);

/**
 * @brief Get the first appearance of a Character/Codepoint in the given String.
 * @param character The Character/Codepoint to search for (UTF-32).
 * @return The byte-offset from string->data for the first byte of the character
 * in the string. -1 if not found
 */
ssize_t mastr_utf8_strchr(const String *string, uint32_t character);

/**
 * @brief Get the first appearance of a Character/Codepoint in the given String.
 * @param character The Character/Codepoint to search for (UTF-32).
 * @return The byte-offset from string->data for the first byte of the character
 * in the string or the NULL Terminator.
 */
ssize_t mastr_utf8_strchrnul(const String *string, uint32_t character);

/**
 * @brief mastr_utf8_strlen for RCStrings.
 * @see mastr_utf8_strlen
 */
size_t mastr_utf8_rcstrlen(RCString string);

/**
 * @brief mastr_utf8_strchr for RCStrings.
 * @see mastr_utf8_strchr
 */
ssize_t mastr_utf8_rcstrchr(RCString string, uint32_t character);

/**
 * @brief mastr_utf8_strchrnul for RCStrings.
 * @see mastr_utf8_strchrnul
 */
ssize_t mastr_utf8_rcstrchrnul(RCString string, uint32_t character);

#endif
