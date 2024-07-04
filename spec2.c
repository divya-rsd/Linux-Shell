#include "headers.h"
void removeLeadingSpaces(char *str)
{
    size_t len = strlen(str);
    size_t offset = strspn(str, " \t"); 

    if (offset > 0)
    {
        memmove(str, str + offset, len - offset + 1);
    }
}
