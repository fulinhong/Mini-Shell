#include "../inc/get_line.h"
#include <stdlib.h>
// 从流里读，返回字符串，失败返回nullptr
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
        // 不知道为什么测不出CRLF, 所以先不管了
        // 暂无
        // ###############################
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
