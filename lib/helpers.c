#include <ctype.h>

#include "helpers.h"

int is_int(char *line)
{
    int current = 0;

    while (line[current] != '\0')
    {
        if (!isdigit(line[current]))
        {
            return 0;
        }

        current += 1;
    }

    return 1;
}
