#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../inc/mini_shell.h"
#include "../inc/get_line.h"
#include "../inc/parser_line.h"

#define MAX_COMMANDS 32u // 暂时先固定数值，反正是给小函数测试的

typedef enum
{
    SHELL_EXIT = 0,    // exit退出
    SHELL_UNKNOWN = 1, // 不是SHELL命令，交给注册表
    SHELL_HANDLED = 2, // SHELL命令已处理，继续循环
} shell_result_t;

static struct
{
    const char *name;
    cmd_func_t func;
} commands[MAX_COMMANDS];

static size_t cmd_count = 0; // 已注册命令数量

static int SearchCommand(char **argv);
static shell_result_t ShellCommand(char **argv);

bool RegisterCommand(const char *name, cmd_func_t func)
{
    if (name == NULL || func == NULL || name[0] == '\0')
    {
        fputs("参数无效\n", stderr);
        return false;
    }

    for (size_t i = 0; i < cmd_count; ++i)
    {
        if (!strcmp(name, commands[i].name))
        {
            fputs(name, stderr);
            fputs(": 命令已注册\n", stderr);
            return false;
        }
    }

    if (cmd_count >= MAX_COMMANDS)
    {
        fputs("注册表已满\n", stderr);
        return false;
    }

    commands[cmd_count].name = name;
    commands[cmd_count].func = func;
    cmd_count++;

    return true;
}

bool MiniShell(void)
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
    system("clear");

    puts("//输入help查看可用命令");

    while (1)
    {
        fputs("mini_shell>\n$ ", stdout);
        fflush(stdout);

        char *line = GetLine(stdin);
        if (line == nullptr)
        {
            if (ferror(stdin))
            {
                fputs("\n读取失败", stderr);
                fflush(stderr);
                return false;
            }

            putchar('\n');
            return true;
        }

        char *argv[PARSER_LINE_MAX_TOKENS];
        int argc = ParseLine(line, argv);
        if (argc == 0)
        {
            free(line);
            putchar('\n');
            continue;
        }
        if (argc == -1)
        {
            free(line);
            fputs("参数无效\n\n", stderr);
            continue;
        }

        bool is_found = false;
        switch (ShellCommand(argv))
        {
        case SHELL_EXIT:
            free(line);
            return true;
        case SHELL_HANDLED:
            free(line);
            continue;
        case SHELL_UNKNOWN:
        {
            int cmd_index;
            if ((cmd_index = SearchCommand(argv)) != -1)
            {
                is_found = true;
                commands[cmd_index].func();
            }
        }
        break;
        }

        if (!is_found)
        {
            fputs(argv[0], stderr);
            fputs(": 命令不存在\n", stderr);
            fflush(stderr);
        }

        free(line);
        putchar('\n');
    }

    return true;
}

static int SearchCommand(char **argv)
{
    for (size_t i = 0; i < cmd_count; ++i)
    {
        if (!strcmp(argv[0], commands[i].name))
        {
            return (int)i;
        }
    }

    return -1;
}

static shell_result_t ShellCommand(char **argv)
{
    if (strcmp(argv[0], "clear") == 0)
    {
        if (!isatty(fileno(stdout)))
        {
            return SHELL_HANDLED;
        }
        fputs("\033[2J\033[H", stdout);
        fflush(stdout);
        return SHELL_HANDLED;
    }

    if (strcmp(argv[0], "help") == 0)
    {
        puts("##可用命令##");
        puts("exit: 退出shell");
        puts("help: 显示可用命令");
        puts("clear: 清屏");

        for (size_t i = 0; i < cmd_count; ++i)
        {
            printf("%s\n", commands[i].name);
        }

        putchar('\n');
        fflush(stdout);
        return SHELL_HANDLED;
    }

    if (strcmp(argv[0], "exit") == 0)
    {
        return SHELL_EXIT;
    }

    return SHELL_UNKNOWN;
}