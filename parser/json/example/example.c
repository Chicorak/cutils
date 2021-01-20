#include "parser/json/json.h"
#include <stdio.h>

void print_json(int level, struct json_value *tree)
{
    int i, j;
    for(i = 0; i < tree->value_count; i++)
    {
        struct json_value *value = tree->values[i];

        switch(value->type)
        {
            case JSON_TYPE_NOVALUE:
            {
                for(j = 0; j < level; j++)
                    printf("  ");

                printf("- %s\n", value->key);
                break;
            }
            case JSON_TYPE_STRING:
            {
                for(j = 0; j < level; j++)
                    printf("  ");

                printf("- %s : %s\n", value->key, value->string_value);
                break;
            }
            case JSON_TYPE_OBJECT:
            case JSON_TYPE_ARRAY:
            {
                for(j = 0; j < level; j++)
                    printf("  ");

                printf("- %s : %d []\n", value->key, value->value_count);

                print_json(level + 1, value);

                break;
            }
        }
    }
}

int main(void)
{
    char *json = "{ \"cc\": \"gcc\", \"out\": \"easymake\", \"src\": \"src/easymake.c\", \"include_dirs\": \"-Iinclude\", \"#default\": \"debug\", \"debug\": [ \"echo Building debug on platform #(os)...\", \"#(cc) -o #(out) #(src) #(include_dirs) -g -Og\", \"echo Done!\"], \"release\": [ \"echo Building release on platform #(os)...\", \"#(cc) -o #(out) #(src) #(include_dirs) -s -Os\", \"echo Done!\" ] }";

    struct json_value *tree = json_parse(json);

    print_json(0, tree);

    json_delete(tree);

    return 0;
}
