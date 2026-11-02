# mini_shell

一个可复用的轻量级命令 shell 库，用于快速测试自己写的小函数：
把函数注册成命令，就能在命令行里直接调用。

- 注册命令：`RegisterCommand("hello", hello)` 之后，输入 `hello` 即可执行。
- 内建命令：`exit`、`help`、`clear`。

## 快速开始

```c
#include <stdio.h>
#include "mini_shell.h"

static void hello(void)
{
    puts("hello from mini_shell");
}

int main(void)
{
    if (RegisterCommand("hello", hello) != 0) {
        fputs("register failed\n", stderr);
        return 1;
    }
    return MiniShell();
}
```
