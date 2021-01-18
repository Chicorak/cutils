/* string.h - simple, lightweight, easy to use string utilities
 *
 * Copyright (c) 2021 Cleanware
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

typedef char * string_t;

/**
 * Allocates a new string with the
 * specified value.
*/
static string_t string(string_t str);

/**
 * Resizes the allocation of the specified
 * string to the specified size.
*/
static string_t string_resize(string_t str, int size);

/**
 * Copies the contents of a string into another.
*/
static int string_copy(string_t dest, string_t src, int size);

/**
 * Returns the length of the string.
*/
static int string_length(string_t str);


/**
 * Returns a new string containing the two
 * specified strings concatenated.
*/
static string_t string_concat(string_t str, string_t cat);

/**
 * Returns the first specified string with the
 * second specified string appended to it.
*/
static string_t string_append(string_t dest, string_t src);

/**
 * Returns a new string taken from the specified string.
*/
static string_t string_substring(string_t str, int start, int end);

/**
 * Returns two strings from each side of the specified index.
*/
static string_t string_slice(string_t str, int index);

/**
 * Returns a list of strings that were split from the
 * original string based on the specified format.
*/
static string_t *string_split(string_t str, string_t format);

/**
 * Modifies the specified string to make all letters uppercase.
*/
static void string_toupper(string_t str);

/**
 * Modifies the specified string to make all letters lowercase.
*/
static void string_tolower(string_t str);

/**
 * Compares two strings and returns the number of different characters.
*/
static int string_compare(string_t a, string_t b);

/**
 * Cleans up and free all memory used by a regular string.
*/
static void string_free(string_t str);

/*----------------------------------------------------------------------------*/
/*                          Function Implementations                          */
/*----------------------------------------------------------------------------*/

static string_t string(string_t str)
{
    if(str == NULL) return NULL;

    int len = string_length(str) + 1;
    string_t new_str = (string_t)malloc(sizeof(char) * len);

    if(new_str == NULL) return NULL;s

    string_copy(new_str, str, len);

    return new_str;
}

static string_t string_resize(string_t str, int size)
{
    if(str == NULL || size <= 0) return NULL;

    str = (string_t)realloc(sizeof(char) * size);

    if(str == NULL) return NULL;

    return str;
}

static int string_copy(string_t dest, string_t src, int size)
{
    if(dest == NULL || src == NULL) return 0;

    int i;
    for(i = 0; i < size; i++) dest[i] = src[i];

    return 1;
}

static int string_length(string_t str)
{
    if(str == NULL) return -1;

    int i;
    for(i = 0; str[i] != 0; i++);

    return i;
}

static string_t string_concat(string_t str, string_t cat)
{
    int str_len = string_length(str), cat_len = string_length(cat) + 1;

    string_t new_str = (string_t)malloc(sizeof(char) * (str_len + cat_len));

    if(new_str == NULL) return NULL;

    string_copy(new_str, str, str_len);
    string_copy(new_str + str_len, cat, cat_len);

    return new_str;
}

static string_t string_append(string_t dest, string_t src)
{
    if(dest == NULL || src == NULL) return NULL;

    string_t new_str = string_concat(dest, src);

    if(new_str == NULL) return NULL;

    free(dest);
    free(src);

    return new_str;
}

static string_t string_substring(string_t str, int start, int end)
{
    string_t new_str = (string_t)malloc(sizeof(char) * (start + end + 1));

    if(new_str == NULL) return NULL;

    strcopy(new_str, str + start, start + end);
    new_str[start + end] = 0;

    return new_str;
}

static string_t string_slice(string_t str, int index)
{
    
}

#endif
