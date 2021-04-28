# 3. Makefile 总述

<a id="top"></a>

[to foot](#foot)

## 3.1. Makefile 的内容

在一个完整的 Makefile 中，包含了 5 个东西：显式规则、隐含规则、变量定义、指示符和注释。关于“规则”、“变量” 和 “Makefile 指示符” 将在后续的章节进行详细的讨论。本章讨论的是一些基本概念。

- 显式规则：它描述了在何种情况下如何更新一个或者多个被称为目标的文件（Makefile 的目标文件）。书写 Makefile 时需要明确地给出目标文件、目标的依赖文件列表以及更新目标文件所需要的命令（有些规则没有命令，这样的规则只是纯粹的描述了文件之间的依赖关系）。
- 隐含规则：它是 make 根据一类目标文件（典型的是根据文件名的后缀）而自动推导出来的规则。make 根据目标文件的名，自动产生目标的依赖文件并使用默认的命令来对目标进行更新（建立一个规则）。
- 变量定义：使用一个字符或字符串代表一段文本串，当定义了一个变量以后，Makefile 后续在需要使用此文本串的地方，通过引用这个变量来实现对文本串的使用。第一章的例子中，我们就定义了一个变量“objects”来表示一个.o 文件列表。
- Makefile 指示符：指示符指明在 make 程序读取 makefile 文件过程中所要执行的一个动作。其中包括：
  - 读取一个文件，读取给定文件名的文件，将其内容作为 makefile 文件的一部分。
  - 决定（通常是根据一个变量的得值）处理或者忽略 Makefile 中的某一特定部分。
  - 定义一个多行变量。
- 注释：Makefile 中“`#`”字符后的内容被作为是注释内容（和 shell 脚本一样）处理。如果此行的第一个非空字符为“`#`”，那么此行为注释行。注释行的结尾如果存在反斜线（\\），那么下一行也被作为注释行。一般在书写 Makefile 时推荐将注释作为一个独立的行，而不要和 Makefile 的有效行放在一行中书写。当在 Makefile 中需要使用字符“`#`”时，可以使用反斜线加“`#`”来实现（对特殊字符“#”的转义），其表示将“#”作为一字符而不是注释的开始标志。

**需要注意的地方**：

Makefile 中第一个规则之后的所有以 [Tab] 字符开始的的行，make 程序都会将其交给系统 shell 程序去解释执行。因此，以 [Tab] 字符开始的注释行也会被交给 shell 来处理，此命令行是否需要被执行（shell 执行或者忽略）是由系统 shell 程序来判决的。

另外，在使用指示符 “define” 定义一个多行的变量或者命令包时，其定义体（“define” 和 “endef” 之间的内容）会被完整的展开到 Makefile 中引用此变量的地方（包含定义体中的注释行）；make 在引用此变量的地方对所有的定义体进行处理，决定是注释还是有效内容。Makefile 中变量的引用和C 语言中的宏类似（但是其实质并不相同，后续将会详细讨论）。对一个变量引用的地方 make 所做的就是将这个变量根据定义进行基于文本的展开，展开变量的过程不涉及到任何变量的具体含义和功能分析。

## 3.2. makefile 文件的命名

默认的情况下，make 会在工作目录（执行 make 的目录）下按照文件名顺序寻找 makefile 文件读取并执行，查找的文件名顺序为：“GNUmakefile”、“makefile”、“Makefile”。

通常应该使用 “makefile” 或者 “Makefile” 作为一个 makefile 的文件名（我们推荐使用“Makefile”，首字母大写而比较显著，一般在一个目录中和当前目录的一些重要文件（README,Chagelist等）靠近，在寻找时会比较容易的发现它）。而“GNUmakefile”是我们不推荐使用的文件名，因为以此命名的文件只有“GNU make”才可以识别，而其他版本的 make 程序只会在工作目录下查找 “makefile” 和 “Makefile” 这两个文件。

如果 make 程序在工作目录下无法找到以上三个文件中的任何一个，它将不读取任何其他文件作为解析对象。但是根据 make 隐含规则的特性，我们可以通过命令行指定一个目标，如果当前目录下存在符合此目标的依赖文件，那么这个命令行所指定的目标将会被创建或者更新，参见[注释](#comment)。

当 makefile 文件的命名不是这三个任何一个时，需要通过 make 的“-f”或者“–file”选项来指定 make 读取的 makefile 文件。给 make 指定 makefile 文件的格式为：“-f NAME” 或者 “--file=NAME” 或 “--makefile=FILE”，它指定文件 “NAME” 作为执行 make 时读取的 makefile 文件。也可以通过多个“-f”或者“--file”选项来指定多个需要读取的 makefile 文件，多个 makefile 文件将会被按照指定的顺序进行链接并被 make 解析执行。当通过“-f”或者“--file”指定 make 读取 makefile 的文件时，make就不再自动查找这三个标准命名的 makefile 文件。

<a id="comment"></a>
**注释**：通过命令指定目标使用 make 的隐含规则：

当前目录下不存在以"GNUmakefile"、"makefile"、"Makefile"命名的任何文件，

1. 当前目录下存在一个源文件 foo.c 的，我们可以使用"make foo.o" 来使用 make 的隐含规则自动生成 foo.o，当执行 "make foo.o" 时。我们可以看到其执行的命令为：`cc -c -o foo.o foo.c` 之后，foo.o 将会被创建或者更新。
2. 如果当前目录下没有 foo.c 文件时，就是 make 对.o 文件目标的隐含规则中依赖文件不存在。如果使用命令 "make foo.o" 时，将会得到如下提示：`make：*** No rule to make target "foo.o" Stop.`
3. 如果直接使用命令 "make" 时，得到的提示信息如下：`make：*** No targets specified and no makefile found.Stop.`

## 3.3. 包含其它 makefile 文件

本节我们讨论如何在一个 Makefile 中包含其它的 makefile 文件。Makefile 中包含其它文件所需要使用的关键字是 “include”，和 c 语言对头文件的包含方式一致。“include”指示符告诉 make暂停读取当前的 Makefile，而转去读取“include”指定的一个或者多个文件，完成以后再继续当前 Makefile 的读取。Makefile 中指示符 “include” 书写在独立的一行，其形式如下：

```
include FILENAMES...
```

FILENAMES 是 shell 所支持的文件名（可以使用通配符）。指示符 “include” 所在的行可以一个或者多个空格（make 程序在处理时将忽略这些空格）开始，切忌不能以 [Tab] 字符开始（如果一行以 [Tab] 字符开始 make 程序将此行作为一个命令行来处理）。指示符 “include” 和文件名之间、多个文件之间使用空格或者 [Tab] 键隔开。行尾的空白字符在处理时被忽略。使用指示符包含进来的 Makefile 中，如果存在变量或者函数的引用。它们将会在包含它们的 Makefile 中被展开（详细可参考第六章 Makefile 中的变量）。来看一个例子，存在三个.mk 文件 a.mk、b.mk、c.mk，“\$(bar)” 被扩展为 “bish bash”。则

```makefile
include foo *.mk $(bar)
```

等价于

```makefile
include foo.make a.mk b.mk c.mk e.mk f.mk
```

之前已经提到过 make 程序在处理指示符 include 时，将暂停对当前使用指示符 “include” 的 makefile 文件的读取，而转去依此读取由 “include” 指示符指定的文件列表。直到完成所有这些文件以后再回过头继续读取指示符 “include” 所在的 makefile 文件。

通常指示符 “include” 用在以下场合：

1. 有多个不同的程序，由不同目录下的几个独立的 Makefile 来描述其重建规则。它们需要使用一组通用的变量定义或者模式规则。通用的做法是将这些共同使用的变量或者模式规则定义在一个文件中（没有具体的文件命名限制），在需要使用的 Makefile 中使用指示符 “include” 来包含此文件。
2. 当根据源文件自动产生依赖文件时；我们可以将自动产生的依赖关系保存在另外一个文件中，主 Makefile 使用指示符 “include” 包含这些文件。这样的做法比直接在主 Makefile 中追加依赖文件的方法要明智的多。其它版本的 make 已经使用这种方式来处理。

如果指示符 “include” 指定的文件不是以斜线开始（绝对路径，如/usr/src/Makefile...），而且当前目录下也不存在此文件；make 将根据文件名试图在以下几个目录下查找：首先，查找使用命令行选项 “-I” 或者 “--include-dir” 指定的目录，如果找到指定的文件，则使用这个文件；否则继续依此搜索以下几个目录（如果其存在）：“/usr/gnu/include”、“/usr/local/include” 和 “/usr/include”。

当在这些目录下都没有找到 “include” 指定的文件时，make 将会提示一个包含文件未找到的告警提示，但是不会立刻退出。而是继续处理 Makefile 的后续内容。当完成读取整个 Makefile 后，make 将试图使用规则来创建通过指示符 “include” 指定的但未找到的文件，当不能创建它时（没有创建这个文件的规则），make 将提示致命错误并退出。会输出类似如下错误提示：

```
Makefile: 错误的行数：未找到文件名：提示信息（No such file or directory）
Make： *** No rule to make target ‘<filename>’. Stop
```

通常我们在 Makefile 中可使用 “-include” 来代替 “include”，来忽略由于包含文件不存在或者无法创建时的错误提示（“-”的意思是告诉 make，忽略此操作的错误。make 继续执行）。像下边那样：

```
-include FILENAMES...
```

使用这种方式时，当所要包含的文件不存在时不会有错误提示、make 也不会退出；除此之外，和第一种方式效果相同。以下是这两种方式的比较：

使用 “include FILENAMES...”，make 程序处理时，如果 “FILENAMES” 列表中的任何一个文件不能正常读取而且不存在一个创建此文件的规则时 make 程序将会提示错误并退出。

使用 “-include FILENAMES...” 的情况是，当所包含的文件不存在或者不存在一个规则去创建它，make 程序会继续执行，只有真正由于不能正确完成终极目标的重建时（某些必需的目标无法在当前已读取的 makefile 文件内容中找到正确的重建规则），才会提示致命错误并退出。为了和其它的 make 程序进行兼容。也可以使用 “sinclude” 来代替 “-include”（GNU 所支持的方式）。

<a id="var_makefiles"></a>

## 3.4. 变量 MAKEFILES

如果在当前环境定义了一个 “MAKEFILES” 环境变量，**make 执行时首先将此变量的值作为需要读入的 Makefile 文件**，多个文件之间使用空格分开。类似使用指示符 “include” 包含其它 Makefile 文件一样，如果文件名非绝对路径而且当前目录也不存在此文件，make 会在一些默认的目录去寻找。它和使用“include”的区别：

1. 环境变量指定的 makefile 文件中的 “目标” 不会被作为 make 执行的“终极目标”。就是说，这些文件中所定义规则的目标，make 不会将其作为“终极目标”来看待。如果在 make 的工作目录下没有一个名为 “Makefile”、“makefile” 或者 “GNUmakefile” 的文件，make 同样会
提示 `“make: *** No targets specified and no makefile found. Stop.”`；而在 make 的工作目录下存在这样一个文件（ “Makefile”、“makefile” 或者 “GNUmakefile”），那么 make 执行时的“终极目标”就是当前目录下这个文件中所定义的“终极目标”。
2. 环境变量所定义的文件列表，在执行 make 时，如果不能找到其中某一个文件（不存在或者无法创建）。make 不会提示错误，也不退出。就是说环境变量 “MAKEFILES” 定义的包含文件是否存在不会导致 make 错误（这是比较隐蔽的地方）。
3. make 在执行时，首先读取的是环境变量 “MAKEFILES” 所指定的文件列表，之后才是工作目录下的 makefile 文件，“include”所指定的文件是在 make 发现此关键字时、暂停正在读取的文件而转去读取 “include” 所指定的文件。

变量 “MAKEFILES” 主要用在 “make” 的递归调用过程中的的通信。实际应用中很少设置此变量。因为一旦设置了此变量，在多级 make 调用时；由于每一级 make 都会读取 “MAKEFILES” 变量所指定的文件，将导致执行出现混乱（这可能不是你想看到的执行结果）。不过，我们可以使用此环境变量来指定一个定义了通用“隐含规则”和变量的文件，比如设置默认搜索路径；通过这种方式设置的 “隐含规则” 和定义的变量可以被任何 make 进程使用（有点象 C 语言中的全局变量）。

也有人想让 make 程序自动的在自己的工作环境中设置此环境变量，编写的 Makefile 建立在此环境变量的基础上。此想法可以肯定地说不是一个好主意。规劝大家千万不要这么干，否则你所编写的 Makefile 在其它人的工作环境中肯定不能正常工作。因为别人的工作环境中可能没有设置相同的环境变量“MAKEFILES”。推荐的做法：在需要包含其它 makefile 文件时使用指示符 “include” 来实现。

<a id="var_makefile_list"></a>

## 3.5. 变量 MAKEFILE_LIST

make 程序在读取多个 makefile 文件时，包括由环境变量 “MAKEFILES” 指定、命令行指定、当前工作下的默认的以及使用指示符 “include” 指定包含的，在对这些文件进行解析执行之前 make 读取的文件名将会被自动依次追加到变量 “MAKEFILE_LIST” 的定义域中，彼此之间用空格分割。

这样我们就可以通过测试此变量的最后一个字来获取当前 make 程序正在处理的 makefile 文件名。具体地说就是在一个 makefile 文件中如果使用指示符 “include” 包含另外一个文件之后，变量 “MAKEFILE_LIST” 的最后一个字只可能是指示符 “include” 指定所要包含的那个文件的名字。一个 makefile 的内容如下：

```makefile
name1 := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
-include inc.mk
name2 := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
all:
	@echo name0 = $(MAKEFILE_LIST)
	@echo name1 = $(name1)
	@echo name2 = $(name2)
```

执行 make，则看到的将是如下的结果：

```
name0 =  Makefile inc.mk
name1 = Makefile
name2 = inc.mk
```

此例子中涉及到了 make 的函数的和变量定义的方式，这些将在后续的章节中有详细的讲述。


## 3.6. 其他特殊变量

<a id="var_dot_variables"></a>

GNU make 支持一个特殊的变量“.VARIABLES”，此变量不能通过任何途经给它赋值。即您分配给该变量的任何值都将被忽略；它将始终返回其特殊值。
它被展开为一个特定的值，被展开以后是此引用点之前、makefile 文件中所定义的所有全局变量列表。包括：空变量（未赋值的变量）和 make 的内嵌变量，但不包含目标指定的变量，目标指定变量值在特定目标的上下文有效。

特殊变量 “.VARIABLES” 使用示例：(ubuntu20.04 + make v4.2)

```makefile
.PHONY: second

second:
	-@echo "$(.VARIABLES)"
```

执行 `make second` 可查看 `.VARIABLES` 的值。

可在 shell 终端输入 `make -p > a.txt` 在 `a.txt` 查看在当前环境中的 make 内部的数据库。

see [make inner data base samples](/gnumf-innerDB.md)。

<a id="remaking-makefiles"></a>

## 3.7. makefile 文件的重建

> How Makefiles Are Remade.

Makefile 可由其它文件生成，例如 RCS 或 SCCS 文件。如果 Makefile 由其它文件重建，那么在 make 在开始解析这个 Makefile 时需要重新读取更新后的 Makefile、而不是之前的 Makefile。make 的处理过程是这样的：

make 在读入所有 makefile 文件之后，首先将所读取的每个 makefile 作为一个目标，寻找更新它们的规则。如果存在一个更新某一个 makefile 文件明确规则或者隐含规则，就去更新对应的 makefile 文件。完成对所有的 makefile 文件的更新之后，如果之前所读取任何一个 makefile 文件被更新，那么 make 就清除本次执行的状态重新读取一遍所有的 makefile 文件（此过程中，同样在读取完成以后也会去试图更新所有的已经读取的 makefile 文件，但是一般这些文件不会再次被重建，因为它们在时间戳上已经是最新的）。读取完成以后再开始解析已经读取的 makefile 文件并开始执行必要的动作。

实际应用中，我们会明确给出 makefile 文件，而并不需要来由 make 自动重建它们。但是 make在每一次执行时总会自动地试图重建那些已经存在的 makefile 文件，如果需要处于效率考虑，可以采用一些办法来避免 make 在执行过程时查找重建 makefile 的隐含规则。例如我们可以书写一个明确的规则，以 makefile 文件作为目标，规则的命令定义为空。

Makefile 规则中，如果使用一个没有依赖只有命令行的双冒号规则去更新一个文件，那么每次执行 make 时，此规则的目标文件将会被无条件的更新（此规则定义的命令会被无条件执行）。如果这样一个规则的目标是 makefile 文件，那么执行 make 时，这个 makefile 文件（双冒号规则的目标）就会被无条件更新，而使得 make 的执行陷入到一个死循环（此 makefile 文件被不断的更新、重新读取、更新再重新读取的过程）。为了防止这种情况的发生，make 在遇到一个目标是 makefile 文件的双冒号规则时，将忽略对这个规则的执行（其中包括了使用“MAKEFILES”指定、命令行选项指定、指示符“include”指定的需要 make 读取的所有 makefile 文件中定义的这一类双冒号规则）。

执行 make 时，如果没有使用“-f（--file）”选项指定一个文件，make 程序将读取缺省的文件。和使用“-f（--file）”选项不同，make 无法确定工作目录下是否存在缺省名称的 makefile 文件。如果缺省 makefile 文件不存在，但可以通过一个规则来创建它（此规则是隐含规则），则会自动创建缺省 makefile 文件，之后重新读取它并开始执行。

因此，如果在当前目录下不存在一个缺省的 makefile 文件，make 将会按照搜索 makefile 文件的名称顺序去试图创建它，直到创建成功或者超越其缺省的命名顺序。需要明确的一点是：执行make 时，如果不能成功地创建缺省的 makefile 文件，并不一定会导致错误。一个存在（缺省命名的或者可被创建的）的 makefile 文件并不是 make 正确运行的前提（关于这一点大家会在后续的阅读过程中体会到）。

当使用“-t（--touch）”选项来更新 Makefile 的目标文件（更新规则目标文件的时间戳）时，对于哪些是 makefile 文件的目标是无效的，这些目标文件（makefile 文件）的时间戳并不会被更新。就是说即使在执行 make 时使用了选项“-t”，那些目标是 makefile 文件的规则同样也会被执行（重建这些 makefile 文件，而其它的规则不会被执行，make 只是简单的更新规则目标文件的时间戳）；类似还有选项“-q（--question）”和“-n（--just-print）”，这主要是因为一个过时的 makefile 文件对其它目标的重建规则在当前看来可能是错误的。

正因为如此，执行命令“make –f mfile –n foo”首先会试图重建“mfile 文件”、并重新读取它，之后会打印出更新目标“foo”所要执行的命令（但不会真正的执行这些命令）。在这种情况时，如果不希望重建 makefile 文件。我们需要在执行 make 时，在命令行中将这个 makefile 文件作为一个最终目标，这样选项“–t”和其它的选项就对这个 makefile 文件目标有效，防止执行这个 makefile作为目标的规则（如果是“-t”参数，则是简单的更新这个 makefile 文件的时间戳）。同样，命令“make –f mfile –n mfile foo”会读取文件“mfile”，打印出重建文件“mfile”的命令、重建“foo”的命令而实际不执行任何命令。并且所打印的用于更新“foo”目标的命令是选项“-f”指定的、没有被重建的“mfile”文件中所定义的命令。

## 3.8. 重载另外一个 makefile

有些情况下，存在两个比较类似的 makefile 文件。其中一个（makefile-A）需要使用另外一个（makefile-B）中所定义的变量和规则。通常我们会想到在“makefile-A”中使用指示符“include”包含“mkaefile-B”来达到目的。但使用这种方式，如果在两个 makefile 文件中存在相同目标，而在不同的文件中其描述规则使用不同的命令。这样，相同的目标文件就同时存在两个不同的规则命令，这是 makefile 所不允许的。遇到这种情况，使用指示符 “include” 显然是行不通的。GNU make 提供另外一种途径来实现此目的。具体的做法如下：

在需要包含的 makefile 文件（makefile-A）中，定义一个称之为 “所有匹配模式” 的规则，它用来述那些在“makefile-A”中没有给出明确创建规则的目标的重建规则。就是说，如果在当前 makefile文件中不能找到重建一个目标的规则时，就使用“所有匹配模式”所在的规则来重建这个目标。

看一个例子，如果存在一个命名为 “Makefile” 的 makefile 文件，其中描述目标 “foo” 的规则和其他的一些规则，我们也可以书写一个内容如下命名为 “GNUmakefile” 的文件。

```makefile
#sample GNUmakefile
foo:
	frobnicate > foo
%: force
	@$(MAKE) -f Makefile $@
force: ;
```

执行命令 “make foo”, make 将使用工作目录下命名为 “GNUmakefile” 的文件并执行目标 “foo” 所在的规则，创建目标 “foo” 的命令是：“`frobnicate > foo`” 如果执行另外一个命令 “make bar”, 因为在 “GUNmakefile” 中没有此目标的更新规则。make 将使用“所有匹配模式”规则，执行命令 “`@$(MAKE) -f Makefile bar`”。如果文件 “Makefile” 中存在此目标更新规则的定义，那么这个规则会被执行。此过程同样适用于其它 “GNUmakefile” 中没有给岀的目标更新规则。此方式的灵活之处在于：如果在 “Makefile” 文件中存在同样一个目标 “foo” 的重建规则，由于 make 执行时首先读取文件 “GUNmakefile” 并在其中能够找到目标 “foo” 的重建规则，所以 make 就不会去执行这个“所有模式匹配规则”（上例中目标“`%`”所在的规则）。这样就避免了使用指示符 "include” 包含一个 makefile 文件时所带来的目标规则的重复定义问题。

此种方式，模式规则的模式只使用了单独的 “%”（我们称它为“所有模式匹配规则”），它可以匹配任何一个目标；它的依赖是“force”，保证了即使目标文件己经存在也会执行这个规则（文件己存在时，需要根据它的依赖文件的修改情况决定是否需要重建这个目标文件）；“force” 规则中使用空命令是为了防止 make 程序试图寻找一个规则去创建目标 “force” 时，又使用了模式规则 “%:force” 而陷入无限循环。

## 3.9. make如何解析makefile文件

GUN make 的执行过程分为两个阶段。

- **第一阶段**：读取所有的 makefile 文件（包括 “MAKEFILES” 变量指定的、指示符 “include” 指定的、以及命令行选项“-f（--file）”指定的makefile文件），内建所有的变量、明确规则和隐含规则，并建立所有目标和依赖之间的依赖关系结构链表。
- **第二阶段**：根据第一阶段己经建立的依赖关系结构链表决定哪些目标需要更新，并使用对应的规则来重建这些目标。

理解 make 执行过程的两个阶段是很重要的。它能帮助我们更深入的了解执行过程中变量以及函数是如何被展开的。变量和函数的展开问题是书写 Makefile 时容易犯错和引起大家迷惑的地方之一。本节将对这些不同的结构的展开阶段进行简单的总结（明确变量和函数的展开阶段，对正确的使用变量非常有帮助）。首先，明确以下基本的概念；在make执行的第一阶段中如果变量和函数被展开，那么称此展开是 “立即” 的，此时所有的变量和函数被展开在需要构建的结构链表的对应规则中（此规则在建立链表是需要使用）。其他的展开称之为“延后”的。这些变量和函数不会被 “立即”展开，而是直到后续某些规则须要使用时或者在make处理的第二阶段它们才会被展开。

可能现在讲述的这些还不能完全理解。不过没有关系，通过后续章节内容的学习，我们会一步 一步的熟悉make的执行过程。学习过程中可以回过头来参考本节的内容。相信在看完本书之后, 会对 make 的整个过程有全面深入的理解。

### 3.9.1. 变量取值

变量定义解析的规则如下：

```makefile
IMMEDIATE = DEFERRED
IMMEDIATE ?= DEFERRED
IMMEDIATE := IMMEDIATE
IMMEDIATE += DEFERRED or IMMEDIATE
define IMMEDIATE
DEFERRED
endef
```

当变量使用追加符（`+=`）时，如果此前这个变量是一个简单变量（使用`:=`定义的）则认为它是立即展开的，其它情况时都被认为是“延后”展开的变量。

### 3.9.2. 条件语句

所有使用到条件语句在产生分支的地方， make 程序会根据预设条件将正确地分支展开。就是说条件分支的展开是“立即”的。其中包括：“ifdef”、“ifeq”、“ifndef”和“ifneq”所确定的所有分支命令。


### 3.9.3. 规则的定义

所有的规则在 make 执行时，都按照如下的模式展开：

```makefile
IMMEDIATE : IMMEDIATE ; DEFERRED DEFERRED
```

其中，规则中目标和依赖如果引用其他的变量，则被立即展开。而规则的命令行中的变量引用会被延后展开。此模板适合所有的规则，包括明确规则、模式规则、后缀规则、静态模式规则。

## 3.10. 总结

make 的执行过程如下：

1. 依次读取变量 “MAKEFILES” 定义的 makefile 文件列表。
2. 读取工作目录下的 makefile 文件（根据命名的查找顺序“GNUmakefile”，“makefile”，“Makefile”， 首先找到那个就读取那个）。
3. 依次读取工作目录 makefile 文件中使用指示符 “include” 包含的文件。
4. 查找重建所有已读取的 makefile 文件的规则（如果存在一个目标是当前读取的某一个 makefile 文件，则执行此规则重建此 makefile 文件，完成以后从第一步开始重新执行）。
5. 初始化变量值并展开那些需要立即展开的变量和函数并根据预设条件确定执行分支。
6. 根据 “终极目标” 以及其他目标的依赖关系建立依赖关系链表。
7. 执行除 “终极目标” 以外的所有的目标的规则（规则中如果依赖文件中任一个文件的时间戳比目标文件新，则使用规则所定义的命令重建目标文件）。
8. 执行 “终极目标” 所在的规则。

执行一个规则的过程是这样的：对于一个存在的规则（明确规则和隐含规则）首先，make程序 将比较目标文件和所有的依赖文件的时间戳。如果目标的时间戳比所有依赖文件的时间戳更新（依赖文件在上一次执行 make 之后没有被修改），那么什么也不做。否则（依赖文件中的某一个或者全 部在上一次执行 make 后已经被修改过），规则所定义的重建目标的命令将会被执行。这就是 make 工作的基础，也是其执行规制所定义命令的依据。（后续讨论规则时将会对此详细地说明）。

## 补充

1. [makefile专题：预定义变量的使用](https://www.jianshu.com/p/96ec2a7c7755)

<center><b>(本章完)</b></center>

<a id="foot"></a>

[to top](#top)

---

- [上一章：2.GNU make 介绍](/introduce.md)
- [下一章：4.Makefile 规则](/rule.md)
