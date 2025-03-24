/*
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