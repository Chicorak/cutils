/* xml.h - a simple, lightweight, easy to use xml parser
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

#ifndef XML_H
#define XML_H

#include <stdlib.h>
#include <string.h>

/**
 * Represents the test="test" in <tag test="test"> ... </test>
*/
struct xml_tag
{
    char *tag, *value;
};

/**
 * Represents a tag (and its subtags and sub values) in
 * <tag test="test"> Hello! <tag2> Goodbye! </tag2> </tag>
*/
struct xml_value
{
    char *tag, *value;
    int tag_count;
    struct xml_tag **tags;
    int sub_value_count;
    struct xml_value **sub_values;
};

/**
 * Takes in a xml string, and returns
 * a xml tree.
*/
static struct xml_value *xml_parse(char *xml);

/**
 * A self-branching function to generate a xml
 * string from a xml tree.
*/
static void xml_generate(char **xml, int index, struct xml_value *value);

/**
 * A function that simplifies the use of xml_generate.
*/
static char *xml_build(struct xml_value *value);

/**
 * A function to cleanup and free all memory used by
 * a xml tree.
*/
static void xml_delete(struct xml_value *value);

/*----------------------------------------------------------------------------*/
/*                          Function Implementations                          */
/*----------------------------------------------------------------------------*/

enum parser_state
{
    IDLE,           /* 0 */
    PARSING_TAG,    /* 1 */
    EXITING_TAG,    /* 2 */
    PARSING_KEY,    /* 3 */
    PARSING_VALUE   /* 4 */
};

static struct xml_value *xml_parse(char *xml)
{
    struct xml_value *container = (struct xml_value *)malloc(sizeof(struct xml_value));

    container->tag = NULL;
    container->value = NULL;
    container->tag_count = 0;
    container->tags = NULL;
    container->sub_value_count = 0;
    container->sub_values = NULL;

    int value_count = 1;
    struct xml_value **values = (struct xml_value **)malloc(sizeof(struct xml_value *));

    values[0] = container;

    int storage_count = 1;
    char *storage = (char *)malloc(sizeof(char));

    storage[0] = 0;

    /* parser state */
    int state = IDLE;

    int i, j = 0;
    for(i = 0; xml[i] != 0; i++)
    {
        char c = xml[i];

        switch(state)
        {
            case IDLE:
            {
                switch(c)
                {
                    case '<':
                    {
                        if(xml[i + 1] != 0)
                        {
                            if(xml[i + 1] == '/')
                            {
                                if(value_count > 1)
                                {
                                    values = (struct xml_value **)realloc(values, sizeof(struct xml_value *) * (value_count - 1));
                                    value_count--;
                                }

                                i++;

                                state = EXITING_TAG;
                                break;
                            }
                        }

                        struct xml_value *value = (struct xml_value *)malloc(sizeof(struct xml_value));

                        value->tag = NULL;
                        value->value = NULL;

                        value->tag_count = 0;
                        value->tags = (struct xml_tag **)malloc(sizeof(struct xml_tag *));

                        value->sub_value_count = 0;
                        value->sub_values = (struct xml_value **)malloc(sizeof(struct xml_value));

                        values[value_count - 1]->sub_values = (struct xml_value **)realloc(values[value_count - 1]->sub_values, sizeof(struct xml_value *) * (values[value_count - 1]->sub_value_count + 1));
                        values[value_count - 1]->sub_value_count++;

                        values[value_count - 1]->sub_values[values[value_count - 1]->sub_value_count - 1] = value;

                        values = (struct xml_value **)realloc(values, sizeof(struct xml_value *) * (value_count + 1));
                        value_count++;

                        values[value_count - 1] = value;

                        state = PARSING_TAG;
                        break;
                    }
                    default:
                    {
                        if(values[value_count - 1]->value == NULL)
                        {
                            values[value_count - 1]->value = (char *)malloc(sizeof(char));
                            values[value_count - 1]->value[0] = 0;
                        }

                        int length = strlen(values[value_count - 1]->value);

                        values[value_count - 1]->value = realloc(values[value_count - 1]->value, sizeof(char) * (length + 2));

                        values[value_count - 1]->value[length] = c;
                        values[value_count - 1]->value[length + 1] = 0;

                        break;
                    }
                }

                break;
            }
            case PARSING_TAG:
            {
                switch(c)
                {
                    case ' ':
                    {
                        if(storage_count > 1)
                        {
                            values[value_count - 1]->tag = strdup(storage);

                            storage = (char *)realloc(storage, sizeof(char));
                            storage_count = 1;

                            storage[0] = 0;
                        }

                        state = PARSING_KEY;
                        break;
                    }
                    case '>':
                    {
                        if(storage_count > 1)
                        {
                            values[value_count - 1]->tag = strdup(storage);

                            storage = (char *)realloc(storage, sizeof(char));
                            storage_count = 1;

                            storage[0] = 0;
                        }

                        state = IDLE;
                        break;
                    }
                    default:
                    {
                        if(c == '_' || (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
                        {
                            storage = (char *)realloc(storage, sizeof(char) * (storage_count + 1));

                            storage[storage_count - 1] = c;
                            storage[storage_count] = 0;

                            storage_count++;
                        }

                        break;
                    }
                }

                break;
            }
            case EXITING_TAG:
            {
                switch(c)
                {
                    case '>':
                    {
                        state = IDLE;
                        break;
                    }
                }

                break;
            }
            case PARSING_KEY:
            {
                switch(c)
                {
                    case '>':
                    {
                        storage = (char *)realloc(storage, sizeof(char));
                        storage_count = 1;

                        storage[0] = 0;

                        if(xml[i - 1] >= 0)
                        {
                            if(xml[i - 1] == '/')
                            {
                                values = (struct xml_value **)realloc(values, sizeof(struct xml_value *) * (value_count - 1));
                                value_count--;
                            }
                        }

                        state = IDLE;
                        break;
                    }
                    case '=':
                    {
                        if(storage_count > 1)
                        {
                            struct xml_tag *tag = (struct xml_tag *)malloc(sizeof(struct xml_tag));

                            tag->tag = strdup(storage);

                            values[value_count - 1]->tags = (struct xml_tag **)realloc(values[value_count - 1]->tags, sizeof(struct xml_tag *) * (values[value_count - 1]->tag_count + 1));
                            values[value_count - 1]->tag_count++;

                            values[value_count - 1]->tags[values[value_count - 1]->tag_count - 1] = tag;

                            state = PARSING_VALUE;
                        }

                        storage = (char *)realloc(storage, sizeof(char));
                        storage_count = 1;

                        storage[0] = 0;

                        break;
                    }
                    default:
                    {
                        if(c == '_' || (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
                        {
                            storage = (char *)realloc(storage, sizeof(char) * (storage_count + 1));

                            storage[storage_count - 1] = c;
                            storage[storage_count] = 0;

                            storage_count++;
                        }

                        break;
                    }
                }

                break;
            }
            case PARSING_VALUE:
            {
                switch(c)
                {
                    case '\'':
                    case '\"':
                    {
                        if(j == 0)
                        {
                            j = 1;
                        }
                        else
                        {
                            j = 0;

                            if(storage_count > 1)
                            {
                                values[value_count - 1]->tags[values[value_count - 1]->tag_count - 1]->value = strdup(storage);

                                storage = (char *)realloc(storage, sizeof(char));
                                storage_count = 1;

                                storage[0] = 0;
                            }

                            state = PARSING_KEY;
                        }

                        break;
                    }
                    default:
                    {
                        storage = (char *)realloc(storage, sizeof(char) * (storage_count + 1));

                        storage[storage_count - 1] = c;
                        storage[storage_count] = 0;

                        storage_count++;

                        break;
                    }
                }

                break;
            }
        }
    }

    free(values);
    free(storage);

    return container;
}

static void xml_generate(char **xml, int index, struct xml_value *value)
{

}

static void xml_delete(struct xml_value *value)
{
    if(value == NULL) return;

    if(value->tag != NULL) free(value->tag);
    if(value->value != NULL) free(value->value);

    int i;
    for(i = 0; i < value->tag_count; i++)
    {
        struct xml_tag *tag = value->tags[i];

        if(tag != NULL)
        {
            if(tag->tag != NULL) free(tag->tag);
            if(tag->value != NULL) free(tag->value);

            free(tag);
        }
    }

    free(value->tags);

    for(i = 0; i < value->sub_value_count; i++)
    {
        struct xml_value *sub_value = value->sub_values[i];

        xml_delete(sub_value);
    }

    free(value->sub_values);

    free(value);
}

#endif
