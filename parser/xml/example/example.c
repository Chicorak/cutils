#include "parser/xml/xml.h"
#include <stdio.h>

void print_xml(int index, struct xml_value *value)
{
    int i, j;
    for(i = 0; i < index; i++) printf("  ");

    printf("- %s : %d <>\n", value->tag, value->sub_value_count);

    for(i = 0; i < value->attr_count; i++)
    {
        for(j = 0; j < index * 2; j++) printf("  ");
        printf("- %s : %s\n", value->attributes[i]->name, value->attributes[i]->value);
    }

    for(i = 0; i < index * 2; i++) printf("  ");
    printf("Value: \"%s\"\n", value->value);

    for(i = 0; i < value->sub_value_count; i++)
    {
        print_xml(index + 1, value->sub_values[i]);
    }
}

int main(void)
{
    char *xml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?> <map version=\"1.4\" tiledversion=\"1.4.3\" orientation=\"orthogonal\" renderorder=\"right-down\" width=\"192\" height=\"192\" tilewidth=\"32\" tileheight=\"32\" infinite=\"0\" nextlayerid=\"8\" nextobjectid=\"37\"> <tileset firstgid=\"1\" name=\"Tileset\" tilewidth=\"32\" tileheight=\"32\" tilecount=\"768\" columns=\"32\"> <image source=\"../../../../../Downloads/generic_platformer_tiles.png\" width=\"1024\" height=\"768\"/> </tileset> <layer id=\"7\" name=\"Background\" width=\"192\" height=\"192\"> <data encoding=\"csv\"> ... </data> </layer> <objectgroup id=\"3\" name=\"Attributes\"> ... </objectgroup> </map>";

    struct xml_value *tree = xml_parse(xml);

    print_xml(0, tree);

    xml_delete(tree);

    return 0;
}
