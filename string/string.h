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

#ifdef SHORTER_NAMES
    #define sizstr resize_string
    #define cpystr copy_string
    #define cntstr count_string
    #define catstr concat_strings
    #define appstr append_string
    #define slistr slice_string
    #define splstr split_string
    #define cmpstr compare_strings
#endif

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
static string_t resize_string(string_t str, int size);

/**
 * Copies the contents of a string into another.
*/
static int copy_string(string_t dest, string_t src, int size);

/**
 * Returns the count of characters in a string.
*/
static int count_string(string_t str);

/**
 * Returns a new string containing the two
 * specified strings concatenated.
*/
static string_t concat_strings(string_t str, string_t cat);

/**
 * Returns the first specified string with the
 * second specified string appended to it.
*/
static string_t append_string(string_t dest, string_t src);

/**
 * Returns a new string taken from the specified string.
*/
static string_t substring(string_t str, int start, int end);

/**
 * Returns two strings from each side of the specified index.
*/
static string_t *slice_string(string_t str, int index);

/**
 * Returns a list of strings that were split from the
 * original string based on the specified format.
*/
static string_t *split_string(string_t str, string_t format);

/**
 * Modifies the specified string to make all letters uppercase.
*/
static void toupper(string_t str);

/**
 * Modifies the specified string to make all letters lowercase.
*/
static void tolower(string_t str);

/**
 * Compares two strings and returns the number of different characters.
*/
static int compare_strings(string_t a, string_t b);

/*----------------------------------------------------------------------------*/
/*                          Function Implementations                          */
/*----------------------------------------------------------------------------*/

static string_t string(string_t str)
{
    if(str == NULL) return NULL;

    int len = string_length(str) + 1;
    string_t new_str = (string_t)malloc(sizeof(char) * len);

    if(new_str == NULL) return NULL;

    string_copy(new_str, str, len);

    return new_str;
}

static string_t resize_string(string_t str, int size)
{
    if(str == NULL || size <= 0) return NULL;

    str = (string_t)realloc(str, sizeof(char) * size);

    if(str == NULL) return NULL;

    return str;
}

static int copy_string(string_t dest, string_t src, int size)
{
    if(dest == NULL || src == NULL) return 0;

    int i;
    for(i = 0; i < size; i++) dest[i] = src[i];

    return 1;
}

static int count_string(string_t str)
{
    if(str == NULL) return -1;

    int i;
    for(i = 0; str[i] != 0; i++);

    return i;
}

static string_t concat_strings(string_t str, string_t cat)
{
    int str_len = string_length(str), cat_len = string_length(cat) + 1;

    string_t new_str = (string_t)malloc(sizeof(char) * (str_len + cat_len));

    if(new_str == NULL) return NULL;

    string_copy(new_str, str, str_len);
    string_copy(new_str + str_len, cat, cat_len);

    return new_str;
}

static string_t append_string(string_t dest, string_t src)
{
    if(dest == NULL || src == NULL) return NULL;

    string_t new_str = string_concat(dest, src);

    if(new_str == NULL) return NULL;

    free(dest);
    free(src);

    return new_str;
}

static string_t substring(string_t str, int start, int end)
{
    string_t new_str = (string_t)malloc(sizeof(char) * (start + end + 1));

    if(new_str == NULL) return NULL;

    string_copy(new_str, str + start, start + end);
    new_str[start + end] = 0;

    return new_str;
}

static string_t *slice_string(string_t str, int index)
{
    int length = string_length(str);
    string_t *sliced = (string_t *)malloc(sizeof(string_t) * 2);

    if(sliced == NULL) return NULL;

    sliced[0] = (string_t)malloc(sizeof(char) * (index + 1));
    sliced[1] = (string_t)malloc(sizeof(char) * (length - index + 1));

    string_copy(sliced[0], str, index);
    string_copy(sliced[1], str + index + 1, length - index);

    sliced[0][index] = 0;
    sliced[0][length - index] = 0;

    return sliced;
}

static string_t *split_string(string_t str, string_t format)
{
    int split_count = 0;
    string_t *split = (string_t *)malloc(sizeof(string_t));


}

static void toupper(string_t str)
{
    int i;
    for(i = 0; str[i] != 0; i++)
    {
        if(str[i] >= 97 && str[i] <= 122)
            str[i] -= 32;
    }
}

static void tolower(string_t str)
{
    int i;
    for(i = 0; str[i] != 0; i++)
    {
        if(str[i] >= 65 && str[i] <= 90)
            str[i] += 32;
    }
}

static int compare_strings(string_t a, string_t b)
{
    int i, diff = string_length(a) - string_length(b);

    if(diff < 0) diff *= -1;

    for(i = 0; a[i] != 0; i++)
    {
        if(b[i] == 0) break;
        if(a[i] != b[i]) diff++;
    }

    return diff;
}

#endif
