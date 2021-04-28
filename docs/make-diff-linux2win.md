
make 原本是 Linux 平台工具，但目前已随着 gcc 等 GNU GCC 编译器一起移植到 windows 平台，如 Mingw / Mingw64 中都将 Linux make 工具移植到 windows, 在 windows 上的名称为：`mingw32-make.exe` , 为了在 windows 使用 make 命令，在将 `mingw32-make.exe` 所在的目录添加到 PATH 环境变量中后，在同一个目录下将`mingw32-make.exe` 复制并重命名为 `make.exe`。

移植后的 `mingw32-make.exe` 和 Linux make 行为基本一致，但由于 2 个操作系统的差异，还是有部分行为存在差异。以下列出部分自己遇到的问题。

1.所有依赖文件均未更新，但在 win 中生成最终可执行目标的命令总会被执行，然而在 Linux 中不会被执行。

**现象分析**

例如：main.c

```c
#include <stdio.h>

int main(void)
{
	printf("%d\n",33);
	return 0;
}
```

Makefile 

```makefile
objects = main.c
CC = gcc
TARGET = main
$(TARGET): $(objects)
	$(CC) $(objects) -o $(TARGET)
```

在 windows 连续执行2次 make，输出

```
$ make
gcc main.c -o main
$ make
gcc main.c -o main
```

而在 linux 连续执行2次：

```
$ make
gcc main.c -o main
$ make
make: 'main.exe' is up to date.
```

显然，win 行为有问题，因第一次执行 make 后，main.c 并未修改过，按 make 工作原理来说，构建最终目标程序的代码不应该执行，但经过测试，这个代码无论如何都会执行。最后，考虑到这儿的目标程序的名字为 `main`,而 win 中可执行程序的后缀名必须为 `.exe`, 因此移植到 windows 上的 make 会在生成最终可执行的目标程序时自动添加 `.exe` 后缀。因而最终你看到的文件的全名为 `main.exe` 而非 `main`.

分析到这儿，原因很清楚，因为 make 执行时，Makefile 中目标名为 main 而非 main.exe，因此在当前位置找不到文件 `main`,所以每次执行 make 时，都会执行这个代码。

***引申***

Linux 中可执行文件与它的后缀无关，执行时通过 `/文件名` 或 `sh 文件名` 执行即可。即使后缀为 .txt 等，只要它的内容是可执行的二进制即可。但是 windows 中要同时保证内容为可执行的二进制程序，而且后缀为 `.exe` 否则将无法运行。

**分析验证** 

将 main.exe 重命名为 main,执行 make，输出 `make: 'main.exe' is up to date.` 符合预期行为。

**解决方案**

已知原因，只需要将可执行目标的文件名添加后缀 `.exe` 即可。本文中将 `TARGET = main` 改为 `TARGET = main.exe` 问题解决。

但是如果你的程序需要跨平台编译，那么在 Linux 上就需要删除 Makefile 中添加 “.exe” 后缀的代码，那有何办法可使在 Linux 和 win 中不修改 Makefile 的内容使其正常工作吗？

可以，那就是使用 make 命令行定义变量的方式。现在将 Makefile 修改为：

```makefile
objects = main.c
CC = gcc
TARGET = main$(targetext)

$(TARGET): $(objects)
	$(CC) $(objects) -o $(TARGET)
```

变量 targetext 表示可执行文件的后缀名，Linux 执行时，直接输入 make 即可；win 中 输入 `make targetext=.exe`。这样便可完美的解决以上问题。

