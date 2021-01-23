/* c.h - a simple, lightweight, easy to use C parser
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

#ifndef C_H
#define C_H

#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/*                              Data Structures                              */
/*---------------------------------------------------------------------------*/

struct c_value
{
    char *identifier;
    int value_type;
    void *value;
    int sub_value_count;
    struct c_value **sub_values;
};

/*---------------------------------------------------------------------------------*/
/*                              Function Declarations                              */
/*---------------------------------------------------------------------------------*/

/**
 * Takes in a C string and returns
 * a C tree.
*/
static struct c_value *c_parse(char *c);

/**
 * Cleans up and frees all memory used by
 * a C tree.
*/
static void c_delete(struct c_value *value);

/*------------------------------------------------------------------------------------*/
/*                              Function Implementations                              */
/*------------------------------------------------------------------------------------*/

enum parser_state
{
    IDLE,     /* 0 */
};

char symbols[] =
{
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '-',
    '=',
    '+',
    '[',
    '{',
    ']',
    '}',
    '|',
    '\\',
    ';',
    ':',
    '\'',
    '\"',
    ',',
    '<',
    '.',
    '>',
    '/',
    '?'
};

char *keywords[] =
{
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
    NULL
};

static struct c_value *c_parse(char *buffer)
{
    struct c_value *container = (struct c_value *)malloc(sizeof(struct c_value));

    container->identifier = NULL;
    container->value_type = -1;
    container->value = NULL;
    container->sub_value_count = 0;
    container->sub_values = NULL;

    int value_count = 1;
    struct c_value **values = (struct c_value **)malloc(sizeof(struct c_value *));

    values[0] = container;

    int storage_count = 1;
    char *storage = (char *)malloc(sizeof(char));

    storage[0] = 0;

    int state = IDLE;

    int i;
    for(i = 0; buffer[i] != 0; i++)
    {
        char c = buffer[i];

        switch(state)
        {
            case IDLE:
            {
                switch(c)
                {
                    case '(':
                    {

                        break;
                    }
                }

                storage = (char *)realloc(storage, sizeof(char) * (storage_count + 1));

                storage[storage_count - 1] = c;
                storage[storage_count] = 0;

                storage_count++;

                if(strcmp(storage, "char") == 0)
                {

                }

                break;
            }
        }
    }
}

#endif
