#ifndef MINI_SHELL_H
#define MINI_SHELL_H

#ifdef __cplusplus
extern "C"
{
#endif

    // 命令的函数指针类型
    typedef void (*cmd_func_t)(void);
    // 返回true表示成功, false表示失败
    bool RegisterCommand(const char *name, cmd_func_t func);
    // 返回true表示成功, false表示失败
    bool MiniShell(void);

#ifdef __cplusplus
}
#endif

#endif