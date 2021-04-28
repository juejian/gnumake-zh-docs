# 14. Makefile的约定

<a id="top"></a>

[to foot](#foot)

> Makefile Conventions.

本章讨论书写 Makefile 时需要遵循的约定。工具 “Automake” 可以帮助我们创建一个遵循这些约定的 Makefile。所有 GNU 发布的软件包中的 Makefile 都是按照这些标准的约定来书写的。因此理解本章的内容，可帮助很快的熟悉那些开源代码的结构。而对于我们，在为一个工程书写 Makefile 时，也尽量遵循这些约定。虽然并没有强求你这么做，但是建议你还是按照已约定的规则来书写自己的 Makefile。更多关于 portable Makefiles 的信息, 见 POSIX and [Portable Make Programming](https://www.gnu.org/software/autoconf/manual/autoconf.html#Portable-Make) in Autoconf.

## 14.1. 基本的约定

> General Conventions for Makefiles.

本节讨论书写 Makefile 时应遵循的一些基本约定，由于不同版本 make 之间的差异。可能在 GNU make 环境中正常工作的 Makefile,换成其它版本的 make 执行时会发生错误。为了最大可能的兼容不同版本的 make，这里给岀了一些基本的约定。

本节讨论书写 Makefile 时应遵循的一些基本约定，由于不同版本 make 之间的差异。可能在 GNU make环境中正常工作的Makefile,换成其它版本的make执行时会发生错误。为了最大可能 的兼容不同版本的make，这里给岀了一些基本的约定。

1.所有的 Makefile 中应该包含这样一行：

```
SHELL = /bin/sh
```

其目的是为了避免变量 “SHELL” 在有些系统上可能继承同名的系统环境变量而导致错误。 虽然在 GNU 版本的 make 中不会岀现这种问题（GNU make中变量 “SHELL” 的默认值是 “/bin/sh”，它不同于系统环境变量 “SHELL”）。

2.小心处理后缀和隐含规则。不同 make 可识别后缀和隐含规则可能不同，它可能会导致混乱或者错误。因此在特定 Makefile 中明确限定可识别的后缀是一个不错的主意。在Makefile中应 该这样做：

```
.SUFFIXES:
.SUFFIXES: .c .o
```

第一行首先取消掉 make 默认的可识别后缀列表，第二行重新指定可识别的后缀列表。

3.小心处理规则中的路径。当需要处理指定目录的的文件时，应该明确给出路径。如 `./` 代表当前目录，`$(srcdir)` 代表源代码目录。没有指定明确路径，那么就意味着是当前目录。

知道目录 `./` (当前目录，GNU 的发布软件包中的 “build” 目录) 和 `$(srcdir)` 的区别很重要, 我们可以通过 “configure” 脚本的选项 “--srcdir” 指定源代码所在的目录(可参考 GNU 发布的软件包中的 configure 脚本)。当源代码目录和 build 目录不同时，规则：

```
foo.1 : foo.man sedscript
	sed -f sedscript foo.man > foo.1
```

将执行失败，是因为 “foo.man” 和 “sedscript” 并不在当前目录(当然，处理这种错误的手段可能有很多，诸如使用变量 “VPATH” 等)。当前目录只是 build 目录，并不是软件包目录。

4.使用 GNU make 的变量 “VPATH” 指定搜索目录。当规则只有一个依赖文件时。应该使用自动化变量 `$<` 和 `$@` 代替岀现在命令的依赖文件和目标文件(其它版本的 make,只在隐含规则中设置自动化变量 `$<`)。对于一个这样的规则：

```
foo.o : bar.c
	$(CC) -I. -I$(srcdir) $(CFLAGS) -c bar.c -o foo.o
```

我们在 Makefile 中应该以这种方式来书写：

```
foo.o : bar.c
	$(CC) -I. -I$(srcdir) $(CFLAGS) -c $< -o $@
```

另外，对于有多个依赖的规则，为了规则能被正确执行。应该在规则的命令行中明确的指定文件的完整路径名。例如第一个例子就可以这样写(需要在规则之前使用 “VPATH” 指定搜索目录)：

```
foo.1 : foo.man sedscript
	sed -e $(srcdir)/sedscript $(srcdir)/foo.man > $@
```

在 GNU 的发布软件包中，包括了很多非源代码的文件。诸如：“info”文件、“Autoconf” 的输岀文件、“Automake”、“Bison” 或者“Flex”等文件。这些文件在发布时也存在于源代码的目录中。因此 Makefile 中对它们的重建也应该是在源代码目录下，而不应该在build目录。

相反的，对于那些本来就不存在于源代码目录下的文件，也不应该将它们创建在源代码的目录下。**要记住，make 的过程不应该以任何方式修改源代码，或者改变源代码目录的结构**。

## 14.2. Makefiles 中实用工具/规则命令行的约定 (补充)

> Utilities in Makefiles

1. 编写 Makefile 命令(以及任何 shell 脚本，如configure)以在sh(传统的 Bourne shell 和 POSIX shell)下运行，而不是在 `csh` 下运行。不要使用任何 `ksh` 或 `bash` 的特殊特性，也不要使用传统的 Bourne sh 中不广泛支持的 POSIX 特性。

2. 用于创建和安装的“configure”脚本以及 Makefile 中的命令，除使用下面所列出的之外，避免使用其它命令：

	```
	awk cat cmp cp diff echo egrep expr false grep install-info ln ls
	mkdir mv printf pwd rm rmdir sed sleep sort tar test touch tr true
	```

3. 在目标 `dist` 的命令行中可以使用压缩工具 `gzip`。

4. 对于可使用的这些工具命令，尽量使用它的通用选项。不要使用那些只在特定系统上有效的选项。例如，不要使用 `mkdir -p`，尽管它可能很方便，因为一些系统根本不支持它，与其他系统，它是不安全的并行执行。有关已知不兼容性的列表，请参阅 Autoconf 中的 [可移植 Shell编程](https://www.gnu.org/software/autoconf/manual/autoconf.html#Portable-Shell)。

5. 尽量不要在规则的命令行中创建符号连接文件(使用“ln"命令)。因为有些系统不支持符号 连接文件(对于类 Unix 的系统我们基本上没有问题，可能这里所说的是 MS-DOS 系统的系统。我想大家也没有兴趣或者说没有必要在 MS-DOS 下写 Makefile,所以这个限制基本可以不考虑)。

6. 重建或者安装目标(一般是伪目标)的命令行可使用编译器或者相关工具程序，这些命令使用 一个变量来表示。这样做的好处是：当修改一个命令时，只需要更改代表命令的变量的值就可以了。对于以下的这些命令程序：

	```
	ar bison cc flex install ld ldconfig lex
	make makeinfo ranlib texi2dvi yacc
	```

	使用下面的 `make` 变量来运行这些程序:

	```
	$(AR) $(BISON) $(CC) $(FLEX) $(INSTALL) $(LD) $(LDCONFIG) $(LEX)
	$(MAKE) $(MAKEINFO) $(RANLIB) $(TEXI2DVI) $(YACC)
	```

当您使用 `ranlib` 或 `ldconfig` 时，您应该确保如果系统中没有涉及到的程序，那么不会发生任何糟糕的事情。安排忽略该命令中的错误，并在命令之前打印一条消息，告诉用户该命令的失败并不意味着有问题。(Autoconf `AC_PROG_RANLIB` 宏可以帮助解决这个问题。)

如果使用符号链接，则应该为没有符号链接的系统实现回退。

可以通过 Make 变量使用的其他实用程序有:

```
chgrp chmod chown mknod
```

在 Makefile 部分(或脚本)中使用其他实用程序是可以的，这些实用程序只适用于特定的系统，并且您知道这些实用程序存在。

我们可以在 Makefile 中为这些命令组件定义一个代表它的变量(如：CHRP、CHMOD等，在命令行中就可以使用 `$(CHMOD)` 来引用)。

书写多系统兼容 Makefile 时需要遵循以上的约定。如果只考虑一种系统时，以上的这些约定中 可以灵活处理，比如：在命令组件的使用上，我们就可以使用这个系统独具的那些命令组件；系统 支持符号链接时，我们就可以在命令行重创建一个符号链接文件。对于上边的第 6 个约定，强烈建议大家都遵循。

## 14.3. 规则命令行的变量

> Variables for Specifying Commands.

在我们书写的 Makefile 中应该将所有的命令、选项作为变量定义，方便后期对命令的修改和对选项的修改。就是说用户可以通过修改一个变量值来重新指定所要执行的命令，或者来控制命令执行的选项、参数等。

当使用变量来表示命令时，如果规则中需要使用此命令时，可通过引用代表此命令的变量来实 现。例如：定义变量 `CC = gcc`，规则中就可使用 `$(CC)` 来引用"gcc"。对于一些件管理器工具如 `ln`，`rm` `mv`等，可以不为它们定义变量，而直接使用。

所有命令执行的参数选项也应该定义一个变量(可称为命令的选项变量)。在命令变量(代表一个命令的变量)后添加 “FLAGS” 来命名这个选项变量。例如：变量 "CFLAGS" 是c编译器(命令变量为“CC”)的命令行选项变量；变量 "YFLAGS" 时命令"yacc"(命令变量为“YACC”)选项变量；变量 “LDFLAGS” 是命令 “ld" (命令变量为“LD”)的选项变量等。在所有需要执行预处 理的命令行应该使用变量 “CPPFLAGS” 作为 gcc 的执行参数；同样在任何进行链接的编译命令中以及在任何直接使用 “ld” 的情况下使用 “LDFLAGS” 作为命令的执行参数。

c编译器的编译选项变量 “CFLAGS” 在 Makefile 中通常是为编译所有的源文件提供选项变量。为编译一个特定文件增加的选项，不应包含在变量 “CFLAGS” 中。编译特定文件(或者一类 特定文件)时，如果需要使用特定的选项参数，可以将这些选项写在编译它所执行规则的命令行中也可以使用目标指定变量或者模式指定变量)。例如：

```makefile
CFLAGS = -g
ALL_CFLAGS = -I. $(CFLAGS)
.c.o:
	$(CC) -c $(CPPFLAGS) $(ALL_CFLAGS) $<
```

例子中，变量 “CFLAGS” 指定编译选项为 “-g”，在本例中它作为缺省的编译选项。对于所有源文件的编译都使用 “-g” 选项。双后缀规则的命令行中为编译生成 “.o” 文件指定了另外的选项 “-I. -g”。

在所有编译命令行中，变量 “CFLAGS” 应该放在编译选项列表的最后。这样可以保证当命令行参数岀现重复时，“CFLAGS” 始终效的。另外，在任何调用c编译器的命令行中都应该使用选项变量 “CFLAGS”，无论是进行编译还是连接。

如果需要在 Makefile 中实现文件安装的规则，那么就需要在 Makefile 中定义变量 "INSTALL"。此变量代表安装命令(install)。同时在Makefile中也需要定义变量 “INSTALL_PROGRAM” 和 “INSTALL_DATA” (“INSTALL_PROGRAM” 的缺省值都是 `$(INSTALL)`； `INSTALL_DATA` 的缺省值是 `${INSTALL} -m 644`)。可以使用这些变量，来安装可执行程序或者非可执行程序到指定位置。例如：

```makefile
$(INSTALL_PROGRAM) foo $(bindir)/foo
$(INSTALL_DATA) libfoo.a $(libdir)/libfoo.a
```

另外，也可以使用变量 “DESTDIR” 来指定目标需要安装的目录。通常也可以不在 Makefile 定义变量 “DESTDIR",可通过 make 命令行参数的形式来指定。例如：`make DESTDIR=exec/ install`。因此上边的命令就可以这样实现：

```makefile
$(INSTALL_PROGRAM) foo $(DESTDIR)$(bindir)/foo $(INSTALL_DATA) libfoo.a
$(DESTDIR)$(libdir)/libfoo.a
```

安装命令中使用文件名而不是目录名作为第二个参数。每一个需要安装的文件使用单独的命令包括安装一个目录)。

在一个命令中安装多个文件是可以接受的，但不是必须的，最后一个参数是一个目录，如:

```makefile
$(INSTALL_PROGRAM) foo bar baz $(bindir)
```

## 14.4. DESTDIR: 支持分段安装 (补充)

> DESTDIR: Support for Staged Installs。

DESTDIR是每个安装的目标文件前的一个变量，像这样:

```makefile
$(INSTALL_PROGRAM) foo $(DESTDIR)$(bindir)/foo
$(INSTALL_DATA) libfoo.a $(DESTDIR)$(libdir)/libfoo.a
```

DESTDIR 变量由用户在 make 命令行上指定为绝对路径。例如:

```
make DESTDIR=/tmp/stage install
```

DESTDIR 应该只在 `install*` 和 `uninstall*` 目标中支持，因为只有在这些目标中它才有用。

如果你的安装步骤通常会安装 `/usr/local/bin/foo` 和 `/usr/local/lib/libfoo.a`，然后在上面的例子中调用安装将安装 `/tmp/stage/usr/local/bin/foo` 和 `/tmp/stage/usr/local/lib/libfoo`。

通过这种方式将变量 `DESTDIR` 预先添加到每个目标，可以实现分段安装，在这种情况下，安装的文件不会直接放置到预期的位置，而是复制到一个临时位置(DESTDIR)。然而，安装的文件保持它们的相对目录结构，任何嵌入的文件名都不会被修改。

你不应该在你的 Makefile 中设置 DESTDIR 的值;然后在默认情况下将文件安装到它们的预期位置。此外，指定 DESTDIR 不应该以任何方式改变软件的操作，因此它的值不应该包含在任何文件内容中。

`DESTDIR` 支持通常用于包创建。对于希望了解给定包将安装在何处的用户，以及允许通常没有权限安装到受保护区域的用户在获得这些权限之前构建和安装，它也很有帮助。最后，对于 stow 这样的工具来说，它可能很有用，在 `stow` 中，代码安装在一个地方，但使用符号链接或特殊的安装操作使其看起来像是安装在其他地方。因此，我们强烈建议 GNU 包支持 `DESTDIR`，尽管它不是绝对必需的。

## 14.5. 安装目录变量

> Variables for Installation Directories.

在 Makefile 中，安装目录同样需要使用变量来指定，这样就可以很方便的修改文件的安装路径。安装目录的标准命名下边将一一介绍。这些变量基于标准的文件系统结构，这些变量的变种在 SVR4、4.4BSD、Linux、Ultrix v4 以及其它现代操作系统中都有使用。

**以下所罗列的两个变量是指定安装文件的根目录**。所有其它安装目录都是它们的子目录。注意：文件不能直接安装在这两个目录下。

- **prefix**

	这个变量(通常作为实际文件安装目录的父目录，可以理解为其它实际文件安装目录的前缀) 用于构造下列(除这两个安装根目录以外的其它目录变量)变量的缺省值。变量 “prefix” 缺省值是 "/usr/local"。创建完整的 GNU 系统时，变量 prefix 的缺省值是空值，“/usr” 是 "/" 的符号连接符文件。（如果使用“Autoconf”工具，它应该写成 “@prefix@”）。注意：当更改了变量 “prefix” 以后重新执行 “make install",不会导致可执行程序（终极目标）的重建。

- **exec_prefix**

	这个前缀用于构造下列变量的缺省值。变量 `exec_prefix` 缺省值是 `$(prefix)`（如果使用 “Autoconf”工具，它应该写为“@exec_prefix@"）。通常，`$(exec_prefix)` 目录中的子目录下存放和机器相关文件（例如可执行文件和例程库）。`$(prefix)` 目录的子目录存放通用的一般文件。同样：改变 `exec_prefix` 的值之后执行“make install”，不会重建可执行程序（终 极目标）。

**可执行程序安装在以下目录之一**。

- **bindir**

	用于安装一般用户可运行的可执行程序。通常它的值为：“/usr/local/bin”，使用时应写为: `$(exec_prefix)/bin`。（使用“ Autoconf" 工具时，应该为“ @bindir@"）。

- **sbindir**

	安装可在 shell 中直接调用执行的程序。这些命令仅对系统管理员有用（系统管理工具）。通常它的值为：“/usr/local/sbin”,要求在使用时应写为：`$(exec_prefix)/sbin`。（使用"Autoconf" 工具时，应该为 “@sbindir@”）。

- **libexecdir**

	用于安装那些通常不是由用户直接使用，而是由其它程序调用的可执行程序。通常它的值为 “/usr/local/libexec”，要求在使用时应写为：`$(exec_prefix)/libexec`。（使用 “Autoconf” 工具时，应该为 “@libexecdir@"）。

	`libexecdir` 的定义对所有包都是一样的，所以你应该把你的数据安装在它的子目录中。大多数包安装它们的数据在 `$(libexecdir)/package-name/` 下，可能在其他子目录下，如 `$(libexecdir)/package-name/machine/version`。

程序执行时使用的数据文件可从以下两个方面来分类：

1. 是否可由程序更改。分为两类：程序可修改和不可修改的文件（虽然用户可编辑其中某些文件）。
2. 是否和体系结构相关。分为两类：体系结构无关文件，可被所有类型的机器共享；体系结构相关文件，仅可被相同类型机器、操作系统共享；其它的就是那些不能被任何两个机器共享的文件。

这样就存在六种不同的可能。除编译生成的目标文件（.o文件）和库文件以外，不推荐使用那些和特定机器体系结构相关的文件，使用和体系无关的数据文件更加简洁，而且，它的实现也并不非常困难。

在 Makefile 中应该使用以下变量为不同类型的文件指定对应的安装目录：

- **datarootdir**

	用于安装和机器体系结构无关的只读数据文件的根目录。通常它的值为：“/usr/local/share”, 使用时应写为 `$(prefix)/share`。（使用 “Autoconf” 工具时，应该为 “@datarootdir@"）。`datadir` 的默认值基于此变量;还有 `infodir`、`mandir` 等。

- **datadir**

	用于安装和机器体系结构无关的只读数据文件。通常它的值为：“/usr/local/share”，使用时应写为：`$(datarootdir)`。（使用 “Autoconf” 工具时，应该为 “@datadir@"）。

	`datadir` 的定义对所有包都是相同的，所以你应该把数据安装在它的子目录中。大多数包在 `$(datadir)/package-name/` 下安装数据。

- **sysconfdir**

	用于安装从属于特定机器的只读数据文件，包括：主机配置文件、邮件服务、网络配置文件、 “/etc/passwd”文件等。所有该目录下的文件都应该是普通文本文件（可识别的“ASCII”码文本文件）。通常它的值为：“/usr/local/etc”，在使用时应写为：`$(prefix)/etc`。（使用“Autoconf" 工具时，应该为 "@sysconfdir@"）。

	不要将可执行文件安装在这个目录下(可执行文件的安装目录应该是 `$(libexecdir)` 或者 `$(sbindir)`。也不要在这个目录下安装那些需要更改的文件(系统的配置文件等)。这些文件应该安装在目录 `$(localstatedir)` 下。

- **sharedstatedir**

	用于安装那些可由程序运行时修改的文件，这些文件与体系结构无关。通常它的值为：“/usr/local/com”, 要求在使用时应写为：`$(prefix)/com`。(使用 “Autoconf” 工具时，应该为 “@sharedstatedir@”。

- **localstatedir**

	用于安装那些可由程序运行时修改的文件，但这些文件和体系结构相关。用户没有必要通过直接修改这些文件来配置软件包，对于不同的配置文件，将它们放在 `$(datadir)` 或者 `$(sysconfdir)` 目录中。`$(localstatedir)` 值通常为:“/usr/local/var”,在使用时应写为：`$(prefix)/var`。 (使用 “Autoconf" 工具时，应该为 “@localstatedir@”)。

- **runstatedir**

	用于安装程序在运行时修改的数据文件的目录，这些文件属于特定的机器，并且不需要比程序的执行持续更长的时间，而程序的执行通常是长期存在的，例如，直到下一次重新引导。系统守护进程的PID文件是一个典型的用途。此外，除了在重新引导时，这个目录不应该被清理，而通用的 `/tmp(TMPDIR)` 可以被任意地清理。它的值通常应该是 `/var/run`，写为 `$(localstatedir)/run`。例如，将它作为一个单独的变量允许在需要时使用`/run`。(如果您使用Autoconf 2.70或更高版本，请将其写成@runstatedir@。)

下面这些变量指定用于安装特定类型文件的目录(如果程序中有它们的话)。每个 GNU 包都应该有 info 文件，所以每个程序都需要 `infodir`，但不是所有程序都需要 `libdir` 或 `lispdir`。

- **includedir**

	用于安装用户程序源代码使用 “#include” 包含的头文件。它的缺省值为：“/usr/local/include", 使用时应写为：`$(prefix)/include`。(使用 “Autoconf” 工具时，应该为 “@includedir@")。 除 gcc 外的大多数编译器不会在目录 "/usr/local/include" 中搜寻头文件，因此这种方式只适用 gcc 编译器。这一点应该不是一个问题，因为很多情况下一些库需要 gcc 才能工作。对那些依靠其它编译器的库文件，需要将头文件安装在两个地方，一个由变量 `includedir` 指定， 另一个由变量`oldincludedir` 指定。

- **oldincludedir**

	它所指定的目录也同样用于安装头文件，这些头文件用于非gcc的编译器。它的缺省值为： "/usr/include"o。(使用“Autoconf" 工具时，应该为“@oldincludedir@")。

	Makefile 在安装头文件时，需要判断变量 "oldincludedir" 的值是否为空。如果为空，就不使用它进行头文件的安装(一般是安装完成"/usr/local/include" 下的头文件之后才安装此目录下的头文件)。

	一个软件包的安装不能替换该目录下已经存在的头文件，除非是同一个软件包(重新使用相同的软件包在此目录下安装头文件）。例如，软件包 “Foo” 需要在 “oldincludedir” 指定的目录下安装一个头文件 “foo.h” 时，可安装的条件为：1.目录 `$(oldincludedir)` 目录下不存在头文件 "foo.h"； 2.己经存在头文件 "foo.h",存在的头文件 "foo.h"是之前软件包 “Foo” 安装的。

	检查头文件 “foo.h” 是否来自于软件包Foo，需要在头文件的注释中包含一个 “magic” 字符串，使用命令 “grep” 来在该文件中查找这个magic。

- **docdir**

	用于安装此包的文档文件(info文件除外)的目录。默认情况下，它应该是 `/usr/local/share/doc/yourpkg`，但它应该写成 `$(datarootdir)/doc/yourpkg`。(如果你正在使用Autoconf，写为@docdir@。) `yourpkg` 子目录可能包含一个版本号，它可以防止具有通用名称(如 `README` )的文件之间发生冲突。

- **infodir**

	用于安装软件包的 Info 文件。它的缺省值为:"/usr/local/share/info",使用时应写为:`$(datarootdir)/info`。(使用“Autoconf" 工具时，应该为“@infodir@”)。

- **htmldir** <br/> **dvidir** <br/> **pdfdir** <br/> **psdir**

	用于以特定格式安装文档文件的目录。默认情况下，它们都应该设置为 `$(docdir)`。
	(如果你使用Autoconf，把它们写成@htmldir@，@dvidir@，等等)提供文档的几种翻译的包应该安装在 `$(htmldir)/ ll`，`$(pdfdir)/ ll`等，其中 `ll` 是地区缩写，如 `en` 或 `pt_BR`。

- **libdir**

	用于存放编译后的目标文件和库文件(文档文件或者执行的共享库文件)。不要在此目录下安装可执行文件(可执行文件应该安装在目录`$(libexecdir)` 下)。变量 `libdir` 值通常 为：“/usr/local/lib”，使用时应写为：`$(exec_prefix)/lib`。(使用 “Autoconf” 工具时，应 该为“@libdir@”。

- **lispdir**

	用于安装软件包的Emacs Lisp文件的目录。它的缺省值为：“/usr/local/share/emacs/site-lisp",使用时应写为：`$(datarootdir)/emacs/site-lisp`。当使用 Autoconf 工具时，应将写为 “@lispdir@”。为了保证“@lispdir@”能够正常工作，需要在 “configure.ac” 文件中包含如下部分：

	```
	lispdir='${datarootdir}/emacs/site-lisp'
	AC_SUBST(lispdir)
	```
- **localedir**

	用于为这个包安装特定于语言环境的信息目录（locale-specific message catalogs）的目录。默认情况下，它应该是 `/usr/local/share/locale`，但它应该写成 `$(datarootdir)/locale`。(如果你使用Autoconf，写为@localedir@。)该目录通常每个区域设置有一个子目录。

Unix风格的帮助文件需要安装在以下目录中：

- **mandir**

	安装该软件包的帮助文档（如果有）的顶层目录。它的缺省值为: `/usr/local/share/man`,要求在使用时应写为：`$(datarootdir)/man`。（使用“Autoconf”工具时，应该为“@@mandir@@”）。

- **mandir**

	安装该软件包的帮助文档（如果有）的顶层目录。它的缺省值为: `/usr/local/share/man`,要求在使用时应写为：`$(datarootdir)/man`。（使用“Autoconf”工具时，应该为 “@mandir@”）。

- **man1dir**

	用于安装帮助文档的第一节（man 1）。它的缺省值为：`$(mandir)/man1`。

- **man2dir**

	用于安装帮助文档的第二节（man 2）。它的缺省值为：`$(mandir)/man2`。

- **`…`**

	不要让任何 GNU 软件的主要文档成为一个手册页(man page)。用 Texinfo 编写手册。手册页只是为那些在 Unix 上运行 GNU 软件的人准备的，它只是一个次要的应用程序。

- **manext**

	文件名扩展字，它是对安装手册的扩展。以点号（.）开始的十进制数。缺省值为：“.1”。

- **man1ext**

	帮助文档的第一节（man1）的文件名扩展字。

- **man2ext**

	帮助文档的第一节（man 2）的文件名扩展字。

- **`…`**

	当一个软件包的帮助手册有多个章节时，使用这些变量代替 “manext”。（第一节“man1ext”, 第二节“ man2ext”，第三节“man3ext”......）。

最后下列这些变量也应该在 Makefile 中定义：

- **srcdir**

此变量指定的目录是需要编译的源文件所在的目录。该变量的值在使用 "configure" 脚本对软件包进行配置时产生的。（使用“Autoconf”工具，应该书写为 `srcdir = @srcdir@`）。

例如：

```
# 安装的公共目录路径前缀。
# 注意：该目录在开始安装前必须存在
prefix = /usr/local
datarootdir = $(prefix)/share
datadir = $(datarootdir)
exec_prefix = $(prefix)
# 放置“gcc”命令使用的可执行程序
bindir = $(exec_prefix)/bin
# 编译器需要的目录
libexecdir = $(exec_prefix)/libexec
# 软件包的 Info 文件所在目录
infodir = $(datarootdir)/info
```

在用户标准指定的目录下安装大量文件时，可以将这些文件分类安装在指定目录的多个子目录下。可以在 Makefile 中实现一个“install” 伪目标来描述安装这些文件的命令(包括创建子目录, 安装文件到对应的子目录中)。

在发布的软件包中，不能强制要求用户必须指定这些安装目录的变量。使用一套标准的安装目 录变量来指定安装目录，当用户需要指定安装目录时，通过修改变量定义来指定具体的目录，在用户没有指定的情况下，使用默认的目录。

有时，并不是所有这些变量都可以在当前版本的 Autoconf 和/或 Automake 中实现;但对于 Autoconf 2.60，我们相信它们都是。当其中任何一个缺失时，这里的描述将作为 Autoconf 将实现什么的规范。作为一名程序员，你可以使用 Autoconf 的开发版本，也可以避免使用这些变量，直到一个支持它们的稳定版本发布。

## 14.6. Makefile 的标准目标名

> Standard Targets for Users。

所有 GNU 发布的软件包的 Makefile 中，必须包含以下这些目标：

- **all**

	此目标的动作是编译整个软件包。“all” 应该为 Makefile 的终极目标。该目标的动作不重建任何文档(只编译所有的源代码，生成可执行程序)；Info文件应该作为发布文件的一部分，DVI 文件只在明确指定的时候才应该被重建。  

	缺省情况下，对所有源程序的编译和连接应该使用选项“-g”，是最终的可执行程序中包含调 试信息。当最终的可执行程序不需要包含调试信息时，可使用“strip”去掉可执行程序中的调 试符号以减小最终的程序大小。

- **install**

	此目标的动作是完成程序的编译并将最终的可执行程序、库文件等拷贝到安装的目录。如果只是验证这些程序是否可被正确安装，它的动作应该是一个测试安装动作。  

	安装时一般不要对可执行程序进行strip (去掉可执行程序内部的调试信息)。存在另外一个目标 “install-strip”，它实现安装的同时完成对可执行程序 strip。

	保证目标 “install” 的动作不更改程序创建目录(builid目录)下的任何文件，对这个目录下文件的修改(重建或者更新)是目标 “all” 所要定义的动作。

	"install"目标定义的动作在安装目录不存在时，能够创建这些不存在的安装目录。这些目录包括：变量 “prefix” 和 “exec__prefix” 指定的目录和所有必要的子目录。完成此任务的方式可以使用下边介绍的 “installdirs” 目标。

	在安装 man 文档的命令前使用 “-” 忽略这安装命令的错误，这样可以避免在没有 Unix man 文档的系统上执行安装时岀现错误。

	安装 Info 文档的方法是使用变量 `INSTALL__DATA` 将 Info文档拷贝到 `$(infodir)` 目录下，如果存在 `install-info` 命令则执行它。“install-info” 是一个编辑 Info “dir”文件的程序，更新或者修改 “info” 文档的入口和目录；它是 Texinfo 软件包的一部分。这里有一个安装 Info 文档的例子：

	```makefile
	do-install-info: foo.info installdirs
		$(NORMAL_INSTALL)
	# 可能在“.”（当前目录）存在一个新的文档，而不是“srcdir”
		if test -f foo.info; then d=.; \
			else d="$(srcdir)"; fi; \
		$(INSTALL_DATA) $$d/foo.info \
			"$(DESTDIR)$(infodir)/foo.info"
	# 如果 install-info 命令存在则运行它
	# 使用“if”代替在命令行前的“-”
	# 这样，就可以看到运行 install-info 产生的真正错误
	# 我们使用“$(SHELL) -c”是因为在一些 shell 中
	# 遇到未知的命令不会运行失败
		$(POST_INSTALL)
		if $(SHELL) -c 'install-info --version' \
			>/dev/null 2>&1; then \
			install-info --dir-file="$(DESTDIR)$(infodir)/dir" \
						"$(DESTDIR)$(infodir)/foo.info"; \
		else true; fi
	```

	目标 install 的命令需要分为三类：正常命令、预安装命令和安装后命令。

- **install-html** <br/> **install-dvi** <br/> **install-pdf** <br/> **install-ps**

	这些目标以非Info格式安装文档;如果需要这种格式，它们将由安装包的人显式调用。GNU更喜欢 info 文件，所以这些必须由安装目标安装。

	当您需要安装许多文档文件时，我们建议您将这些目标安装在适当安装目录的子目录中，例如 htmldir，从而避免冲突和混乱。举个例子,如果你的包中有多个手册,与许多文件和您希望安装的 HTML 文档(如分割模式输出 `makeinfo --html`),你肯定会想使用子目录,或者具有相同名称的两个节点在不同的手册会互相覆盖。

	Please make these install-format targets invoke the commands for the format target, for example, by making format a dependency.
	请让这些安装格式目标调用格式目标的命令，例如，让格式成为依赖项。

- **uninstall**

	删除所有已安装文件——由 install 创建的文件拷贝。规则所定义的命令不能修改编译目录下的文件，仅仅是删除安装目录下的文件。像 install 目标的命令一样， uninstall 目标的命令也分为三类。参考 14.7 安装命令分类一节。

- **install-strip**

	和目标 install 的动作类似，但是install-strip指定的命令在安装时对可执行文件进行strip (去掉程序内部的调试信息)。它的定义如下：

	```makefile
	install-strip:
		$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' \
			install
	```

	如果软件包的存在安装脚本时，目标 install-strip 所定义的命令就不能是对目标 “install” 的引用，它仅仅完成对可执行文件的 strip。

	"install-strip" 不应该直接在 build 目录下对可执行文件进行 strip,应该是对安装目录下的可执行文件进行 strip。就是说“install-strip” 所定义的命令不能对 build 目录下的文件产生影响。

	一般不建议安装时对可执行文件进行strip,因为去掉可执行文件的调试信息后，如果在程序中存在bug，就不能通过 gdb 对程序进行调试。

- **clean**

	清除当前目录下编译生成的所有文件，这些文件在 make 过程中产生。注意，clean 动作不能删除软件包的配置文件，同时也不能删除build 时创建的那些文件(诸如：目录、build生成的信息记录文件等)。因为这些文件都是发布版本的一部分。

	对于.dvi文件，当它不作为发布版本的一部分时，可以删除。

- **distclean**

	删除当前目录(或由这个 makefile 创建)中通过配置或构建程序创建的所有文件。如果您已经解包了源代码，并且构建了程序而没有创建任何其他文件，那么 `make distclean` 应该只保留发布版中的文件。但是，不需要删除使用 `mkdir -p` 创建的父目录，因为它们可能已经存在。

- **mostlyclean**

	类似于目标 “clean”，但是可保留一些编译生成的文件，避免在下次编译时对这些文件重建。 例如，对于 gcc 来说，此目标指定的命令不删除文件 “libgcc.a”，因为在绝大多数情况下它都不需要重新编译。

- **maintainer-clean**

	此目标所定义的命令几乎会删除所有当前目录下能够由 Makefile 重建的文件。典型的，包括目标 "distclean" 删除的文件、由Bison生成的.c源文件、tags记录文件、Ifon文件等。但是有一个例外，就是执行 `make maintainer-clean` 不能删除"configure" 这个配置脚本文件, 即使 “configure” 可以由 Makefile 生成。因为 "configure" 是软件包的配置脚本。

	目标 “maintainer-clean” 应该只能由维护软件包的用户使用，而不能被普通用户使用。因为它会删除一些软件包的发布文件，而重建这些文件可能需要专门的工具。因此我们在使用此目标时需要小心。为了让用户能够在执行前得到提示，通常目标“maintainer-clean” 的命令以下两行为开始：

	```makefile
	@echo 'This command is intended for maintainers to use; it'
	@echo 'deletes files that may need special tools to rebuild.'
	```

- **TAGS**

	此目标所定义的命令完成对该程序的tags记录文件的更新。tags文件通常可被编辑器作为符号记录文件，例如 vim， Emacs 等。

- **info**

	产生必要的 Info 文档。此目标应该按照如下书写：

	```makefile
	info: foo.info

	foo.info: foo.texi chap1.texi chap2.texi
		$(MAKEINFO) $(srcdir)/foo.texi
	```

	必须在 Makefile 中定义变量 “MAKEINFO”，代表命令工具 makeinfo,该工具是发布软件 Texinfo 的一部分。

	通常，GNU 的发布程序会和 Info 文档会被一同创建，这意味着 Info 文档是在源文件的目录下。用户在创建发布软件时，一般情况下， make 不更新 Info 文档，因为它们已经更新到最新了。

- **dvi** <br/> **html** <br/> **pdf** <br/> **ps**

	生成给定格式的文档文件。这些目标应该始终存在，但是如果不能生成给定的输出格式，任何一个或所有目标都可以是空操作。这些目标不应该是所有目标的依赖性;用户必须手动调用它们。

	为所有的 Texinfo 文件创建对应的 DVI 文件。例如：

	```makefile
	dvi: foo.dvi

	foo.dvi: foo.texi chap1.texi chap2.texi
		$(TEXI2DVI) $(srcdir)/foo.texi
	```

	必须在 Makefile 中定义变量 “TEXI2DVI”。它代表命令工具 texi2dvi，该工具是发布软件 Texinfo 一部分。

	规则中也可以没有命令行，这样 make 程序会自动为它推导对应的命令。

- **dist**

	此目标指定的命令创建发布程序的 tar 文件。创建的 tar 文件应该是这个软件包的目录，文件名中也可以包含版本号(就是说创建的 tar 文件在解包之后应该是一个目录)。例如，发布的 gcc 1.40 版的 tar 文件解包的目录为 “gcc-1.40”。

	通常的做法是是创建一个空目录，如使用 ln 或 cp 将所需要的文件加入到这个目录中，之后对这个目录使用 tar 进行打包。打包之后的 tar 文件使用 gzip 压缩。例如，实际的 gcc 1.40

	版的发布文件叫 “gcc-1.40.tar.gz”。

	目标“dist"的依赖文件为软件包中所有的非源代码的文件，因此在使用目标进行发布软件打包压缩之前必须保证这些文件是最新的。

- **check**

	此目标指定的命令完成所有的自检功能。在执行检查之前，应确保所有程序已经被创建，可以不安装。为了对它们进行测试，需要实现在程序没有安装的情况下被执行的规则命令。  

- **installcheck**

	执行安装检查。在执行安装检查之前，确保所有程序已经被创建并且被安装。需要注意的是安装目录 `$(bindir)` 是否在搜索路径中。  

- **installdirs**

	使用目标 “installdirs” 创建安装目录以及它的子目录在很多场合是非常有用的。脚本 `mkin-stalldirs` 就是为了实现这个目的而编写的；发布的 Texinfo 软件包中就包含了这个脚本文件。 Makefile 中的规则可以这样书写：

	```makefile
	# 确保所有安装目录(例如$(bindir))存在，如有必要则创建这些目录 
	installdirs: mkinstalldirs
		$(srcdir)/mkinstalldirs $(bindir) $(datadir) \
								$(libdir) $(infodir) \
								$(mandir)
	```

	或者可以使用变量 `DESTDIR`：

	```makefile
	# Make sure all installation directories (e.g. $(bindir))
	# actually exist by making them if necessary.
	installdirs: mkinstalldirs
		$(srcdir)/mkinstalldirs \
			$(DESTDIR)$(bindir) $(DESTDIR)$(datadir) \
			$(DESTDIR)$(libdir) $(DESTDIR)$(infodir) \
			$(DESTDIR)$(mandir)
	```

	该规则不能更改软件的编译目录，仅仅是创建程序的安装目录。

## 14.7. 安装命令分类

> Install Command Categories.

> When writing the install target, you must classify all the commands into three categories: normal ones, pre-installation commands and post-installation commands.

在为 Makefile 书写 `install` 目标时，需要将其命令分为三类：正常命令、安装前命令和安装后命令。

正常命令是把文件移动到合适的地方，并设置它们的模式。这个过程不修改任何文件，仅仅是把需要安装的文件从软件包中拷贝到安装目录。

安装前命令和安装后命令可能修改某些文件；通常，修改一些配置文件和系统的数据库文件。典型地，安装前命令在正常命令之前执行，安装后命令在正常命令执行后执行。

大多数情况是，安装后命令是运行 `install-info` 程序。它所完成的工作不能由正常命令完成,因为它更新了一个文件(Info的目录)，该文件不能单独的或者完整的从软件包来进行安装，因为它只能在正常安装命令完成安装软件包的 info 文档之后才可正确执行。

大多数程序不需要安装前命令，但应该在 Makefile 中提供。

将 `install` 规则的命令分为这三类时，应该在命令之间插入分类行(category lines)。分类行说明了后续需要执行的命令的类别。

分类行是由一个［Tab］字符开始的对 make 特殊变量的引用，行尾是可选的注释内容。可以使用三个特殊的变量，每一个代表一种类别。分类行执行一个空动作，因为这三个特殊的 make 变量没有定义(我们书写的 Makefile 中也不能定义它们)。

以下是三种可能的分类行，以及它们的解释：

```makefile
$(PRE_INSTALL) # 以下是安装前命令
$(POST_INSTALL) # 以下是安装后命令
$(NORMAL_INSTALL) # 以下是正常命令
```

如果安装规则(`install`所在的规则)的命令行没有使用分类行，那么在第一个岀现的分类行之前的所有的命令行都被认为是正常命令。如果命令行中没有分类行，那么规则的所有命令行都都被认为是正常命令行。

对应的，以下这三个是 `uninstall` 命令的分类行：

```makefile
$(PRE_UNINSTALL) # 以下是卸载前命令
$(POST_UNINSTALL) # 以下是卸载后命令
$(NORMAL_UNINSTALL) # 以下是正常命令
```

卸载前命令应该是取消软件包 Info 文档的入口。

如果目标 install 或 uninstall 存在依赖，其作为安装的子例程，那么就应该在每一个依赖目标的命令行开始使用分类行，同时目标 insall 和 uninstall 的命令行开始也需要使用分类行。这样就可以确保任何调用方式时每一条命令都被正确的分类。

除下列命令外，安装前命令和安装后命令不应该使用其它命令：

```
[ basename bash cat chgrp chmod chown cmp cp dd diff echo
egrep expand expr false fgrep find getopt grep gunzip gzip
hostname install install-info kill ldconfig ln ls md5sum
mkdir mkfifo mknod mv printenv pwd rm rmdir sed sort tee
test touch true uname xargs yes
```

按照这种方式分类命令的原因是为了创建二进制的软件包。典型的二进制软件包包括可执行文件、必须安装的其它文件以及它自己的安装文件，因此二进制软件包就不需要任何正常命令、只需要安装前命令和安装后命令。

构建二进制包的程序通过提取安装前和安装后命令来工作。下面是一种**抽取安装前命令的方法**(需要使用-s选项来屏蔽关于进入子目录的消息):

```
make -s -n install -o all \
    PRE_INSTALL=pre-install \
    POST_INSTALL=post-install \
    NORMAL_INSTALL=normal-install \
    | gawk -f pre-install.awk
```

文件 `pre-install.awk` 可能包括以下内容：

```
$0 ~ /^(normal-install|post-install)[ \t]*$/ {on = 0}
on {print $0}
$0 ~ /^pre-install[ \t]*$/ {on = 1}
```

安装前命令的执行结果和软件包的安装 shell 脚本的结果一样。

<a id="foot"></a>

[to top](#top)

<center><b>(本章完)</b></center>

---

- [上一章：13.和其它版本的兼容](/compatibility.md)
- [下一章：15.make 的常见错误信息](/errmsg.md)

