/*
 * Copyright (C) 2025, Marie Eckert
 * Licensed under the BSD 3-Clause License.
 */

#include <stdlib.h>
#include <string.h>

#include "mastr.h"

#ifndef MASTR_RESIZE_ALIGNMENT
#	define MASTR_RESIZE_ALIGNMENT 64
#endif

String *
mastr_string_new(uint32_t byteCapacity)
{
	String *str;
	const size_t dataSize = byteCapacity * sizeof(*str->data);
	const size_t size = sizeof(String) + dataSize;

	str = malloc(size);
	if(str == NULL) {
		return NULL;
	}

	memset(str->data, 0, dataSize);
	str->capacity = byteCapacity;
	str->length = 0;

	return str;
}

String *
mastr_string_from(const String *original)
{
	return mastr_string_from_cstr(original->data);
}

String *
mastr_string_from_cstr(const char *original)
{
	String *str;
	const size_t size = original != NULL ? strlen(original) : 0;

	str = mastr_string_new(size + 1);

	if(str == NULL) {
		return NULL;
	}

	str->length = size;
	memcpy(str->data, original, size + 1);

	return str;
}

String *
_mastr_intern_string_append(String *a, const char *b, size_t b_len)
{
	if(a == NULL || b == NULL) {
		return a != NULL ? a : NULL;
	}

	const size_t new_length = a->length + b_len;

	if(new_length + 1 > a->capacity) {
		a = mastr_string_resize(a, new_length);

		if(a == NULL) {
			return NULL;
		}
	}

	memcpy(a->data + a->length, b, b_len);
	a->length = new_length;
	a->data[new_length] = 0;

	return a;
}

String *
mastr_string_append(String *a, const String *b)
{
	return _mastr_intern_string_append(a, b->data, b->length);
}

String *
mastr_string_append_cstr(String *a, const char *b)
{
	return _mastr_intern_string_append(a, b, strlen(b));
}

String *
mastr_string_resize(String *a, uint32_t newSize)
{
	const size_t resize_amount =
		(newSize + 1 + MASTR_RESIZE_ALIGNMENT) & ~(MASTR_RESIZE_ALIGNMENT - 1);

	const size_t new_size = resize_amount * sizeof(*a->data) + sizeof(*a) + 1;

	a = realloc(a, new_size);
	if(a == NULL) {
		return NULL;
	}

	a->capacity = sizeof(char) * resize_amount;

	return a;
}

ssize_t
_mastr_intern_strstr(const String *haystack,
					 const char *needle,
					 size_t needleLength)
{
	ssize_t pos = 0;

	bool found = true;
	for(; pos < haystack->length; pos++) {
		for(size_t ix = 0; ix < needleLength; ix++) {
			if(haystack->data[pos] != needle[needleLength]) {
				found = false;
				break;
			}
		}

		if(found) {
			break;
		}
	}

	return found ? pos : -1;
}

ssize_t
mastr_strstr(const String *haystack, const String *needle)
{
	return _mastr_intern_strstr(haystack, needle->data, needle->length);
}

ssize_t
mastr_strstr_cstr(const String *haystack, const char *needle)
{
	return _mastr_intern_strstr(haystack, needle, strlen(needle));
}

RCStringResult
mastr_rcstring_new(uint32_t byteCapacity)
{
	String *string = mastr_string_new(byteCapacity);
	return MASTR_CONSTRUCT_RCSTRING_RESULT(MASTR_CONSTRUCT_RCSTRING(string));
}

RCStringResult
mastr_rcstring_from(const String *original)
{
	String *string = mastr_string_from(original);
	return MASTR_CONSTRUCT_RCSTRING_RESULT(MASTR_CONSTRUCT_RCSTRING(string));
}

RCStringResult
mastr_rcstring_from_cstr(const char *original)
{
	String *string = mastr_string_from_cstr(original);
	return MASTR_CONSTRUCT_RCSTRING_RESULT(MASTR_CONSTRUCT_RCSTRING(string));
}

RCStringResult
mastr_rcstring_append(RCString a, RCString b)
{
	String *string = mastr_string_append(a.string, b.string);
	return MASTR_CONSTRUCT_RCSTRING_RESULT(MASTR_CONSTRUCT_RCSTRING(string));
}

RCStringResult
mastr_rcstring_append_cstr(RCString a, const char *b)
{
	String *string = mastr_string_append_cstr(a.string, b);
	return MASTR_CONSTRUCT_RCSTRING_RESULT(MASTR_CONSTRUCT_RCSTRING(string));
}

size_t
mastr_utf8_strlen(const String *string)
{
	size_t length = 0;
	for(size_t ix = 0; ix < string->length; ix++) {
		length += (size_t)(*(string->data + ix) & 0xC0) != 0x80;
	}

	return length;
}

ssize_t
mastr_utf8_strchr(const String *string, uint32_t character)
{
	char utf8_char[8];
	const size_t char_length = mastr_utf32_to_utf8_char(character, &utf8_char);
	utf8_char[char_length] = '\0';

	return mastr_strstr_cstr(string, utf8_char);
}

ssize_t
mastr_utf8_strchrnul(const String *string, uint32_t character)
{
}

size_t
mastr_utf8_rcstrlen(const String *string)
{
}

ssize_t
mastr_utf8_rcstrchr(const String *string, uint32_t character)
{
}

ssize_t
mastr_utf8_rcstrchrnul(const String *string, uint32_t character)
{
}
