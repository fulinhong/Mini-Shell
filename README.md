# mini_shell

一个可复用的轻量级命令 shell 库，用于快速测试自己写的小函数：
把函数注册成命令，就能在命令行里直接调用。

- 注册命令：`RegisterCommand("hello", hello)` 之后，输入 `hello` 即可执行。
- 内建命令：`exit`、`help`、`clear`。

## 演示

```c
#include <stdio.h>
#include "mini_shell.h"

void Hello(void)
{
    puts("Ciallo ~(∠ • ω<)⌒☆");
}

int main(void)
{
    if (!RegisterCommand("hello", Hello))
        puts("注册失败");

    MiniShell();
    return 0;
}
```
