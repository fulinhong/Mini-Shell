#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../inc/mini_shell.h"

#define MAX_COMMANDS 32u // 暂时先固定数值，反正是给小函数测试的
#define MAX_LINE 1024u

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

static int SearchCommand(const char *line_buffer);
static shell_result_t ShellCommand(const char *line_buffer);
static void clear_screen(void);

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

    clear_screen();
    puts("//输入help查看可用命令");

    char line_buffer[MAX_LINE];

    while (1)
    {
        fputs("mini_shell>\n$ ", stdout);
        fflush(stdout);

        if (fgets(line_buffer, sizeof(line_buffer), stdin) == NULL)
        {
            if (ferror(stdin))
            {
                puts("\n读取失败");
                fflush(stdout);
                return false;
            }
            putchar('\n');
            break;
        }

        line_buffer[strcspn(line_buffer, "\n")] = '\0'; // 以便后续命令不用加\n

        bool is_found = false;
        switch (ShellCommand(line_buffer))
        {
        case SHELL_EXIT:
            return true; // 正常退出true
        case SHELL_HANDLED:
            continue;
        case SHELL_UNKNOWN:
        {
            int cmd_index;
            if ((cmd_index = SearchCommand(line_buffer)) != -1)
            {
                is_found = true;
                commands[cmd_index].func();
            }
        }
        break;
        }

        if (!is_found && line_buffer[0] != '\0')
        {
            fputs(line_buffer, stdout);
            puts(": 命令不存在");
        }

        putchar('\n');
    }

    return true;
}

static int SearchCommand(const char *line_buffer)
{
    for (size_t i = 0; i < cmd_count; ++i)
    {
        if (!strcmp(line_buffer, commands[i].name))
        {
            return (int)i;
        }
    }

    return -1;
}

static shell_result_t ShellCommand(const char *line_buffer)
{
    if (strcmp(line_buffer, "exit") == 0)
    {
        return SHELL_EXIT;
    }
    if (strcmp(line_buffer, "help") == 0)
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
    if (strcmp(line_buffer, "clear") == 0)
    {
        clear_screen();
        return SHELL_HANDLED;
    }

    return SHELL_UNKNOWN;
}

static void clear_screen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}