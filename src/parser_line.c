// 测试用头文件
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../inc/parser_line.h"
// line已经在mini_shell.c中确保非空
static int TokenizeInLine(char *line);
static void DispatchArgs(char *line, int argc, char **argv);

// 测试用char *argv[128];
// const char *argv[128];

// 把命令行解析为参数数组，返回参数数量
int ParseLine(char *line, char **argv)
{
    int argc = TokenizeInLine(line);
    DispatchArgs(line, argc, argv);
    return argc;
}
// 把原字符串line中的空格替换为'\0'，返回token数量
static int TokenizeInLine(char *line)
{
    bool is_in_token = false;
    int token_count = 0;

    char *read_ptr = line;
    char *write_ptr = line;
    while (*read_ptr != '\0')
    {
        if (*read_ptr == ' ' || *read_ptr == '\t')
        {
            while (*read_ptr == ' ' || *read_ptr == '\t')
                read_ptr++;

            if (is_in_token)
            {
                *write_ptr = '\0';

                ++write_ptr;
                ++token_count;
            }

            is_in_token = false;
        }
        else
        {
            *write_ptr++ = *read_ptr++;
            is_in_token = true;
        }
    }

    if (is_in_token)
        ++token_count;

    *write_ptr = '\0';
    if (token_count > PARSER_LINE_MAX_TOKENS)
        return -1;
    return token_count;
}

static void DispatchArgs(char *line, int argc, char **argv)
{
    char *read_ptr = line;
    for (int i = 0; i < argc; ++i)
    {
        argv[i] = read_ptr;

        while (*read_ptr != '\0')
            read_ptr++;

        if (i < argc - 1)
            read_ptr++;
    }
}

// 测试Main
// int main(void)
// {
//     system("chcp 65001 > nul");
//     char *line = (char *)malloc(128);
//     strcpy(line, "  Ciallo (●'◡'●) \" \" ");
//     int token_count = ParseLine(line, argv);
//     printf("token_count = %d\n", token_count);

//     for (int i = 0; i < token_count; ++i)
//     {
//         printf("token[%d] = #%s;\n", i, argv[i]);
//     }

//     free(line);
//     return 0;
// }
