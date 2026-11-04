#include "../inc/get_line.h"
#include <stdlib.h>

char *GetLine(FILE *fp)
{
    size_t capacity = 128;
    size_t char_index = 0;

    char *line_buffer = (char *)malloc(capacity);
    if (line_buffer == nullptr)
        return nullptr;

    int cur_char;
    while ((cur_char = fgetc(fp)) != EOF)
    {
        if (cur_char == '\n')
            break;

        if (cur_char == '\r')
        {
            int next = fgetc(fp);
            if (next != EOF && next != '\n')
                ungetc(next, fp);
        }

        if (char_index + 1 >= capacity)
        {
            size_t new_capacity = 2 * capacity;
            char *new_line_buffer = (char *)realloc(line_buffer, new_capacity);

            if (new_line_buffer == nullptr)
            {
                free(line_buffer);
                return nullptr;
            }

            line_buffer = new_line_buffer;
            capacity = new_capacity;
        }
        // line_buffer里没有'\n'和'\0', cur_char是'\n'
        line_buffer[char_index++] = (char)cur_char;
    }

    if (char_index == 0 && cur_char == EOF)
    {
        free(line_buffer);
        return nullptr;
    }

    line_buffer[char_index] = '\0';
    return line_buffer;
}