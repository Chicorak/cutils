#define SHORTER_NAMES
#include "string/string.h"
#include <stdio.h>

int main(void)
{
    string_t s1 = string("Hello");
    string_t s2 = string("H3ll0");

    int diff = cmpstr(s1, s2);

    printf("%d\n", diff);

    free(s1);
    free(s2);

    return 0;
}
