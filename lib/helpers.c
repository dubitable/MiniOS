#include <stdbool.h>
#include <ctype.h>

#include "helpers.h"

bool is_int(char *line)
{
    int current = 0;

    while (line[current] != '\0')
    {
        if (!isdigit(line[current]))
        {
            return false;
        }

        current += 1;
    }

    return true;
}
