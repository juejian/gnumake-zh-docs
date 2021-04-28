# 6. Makefile中的变量

<a id="top"></a>

[to foot](#foot)

> How to Use Variables.

在 Makefile 中，变量是一个名字（像是C语言中的宏），代表一个文本字符串（变量的值）。在 Makefile 的目标、依赖、命令中引用变量的地方，变量会被它的值所取代（与C语言中宏引用的方式相同，因此其他版本的 make 也把变量称之为“宏”）。在 Makefile 中变量有以下几个特征。

1. Makefile 中变量和函数的展开（除规则命令行中的变量和函数以外），是在 make 读取 makefile 文件时进行的，这里的变量包括了使用 “=” 定义和使用指示符 “define” 定义的。

2. 变量可以用来代表一个文件名列表、编译选项列表、程序运行的选项参数列表、搜索源文件的目录列表、编译输岀的目录列表和所有我们能够想到的事物。

3. 变量名是不包括“:”、“\#”、“=”、前置空白和尾空白的任何字符串。需要注意的是，尽管在 GNU make 中没有对变量的命名有其它的限制，但定义一个包含除字母、数字和下划线以外的变量的做法也是不可取的，因为除字母、数字和下划线以外的其它字符可能会在 make的后续版本中被赋予特殊含义，并且这样命名的变量对于一些 shell 来说是不能被作为环境变量来使用的。

4. 变量名是大小写敏感的。变量“foo”、“Foo”和“FOO”指的是三个不同的变量。Makefile 传统做法是变量名是全采用大写的方式。**推荐的做法是在对于内部定义定义的一般变量（例如目标文件列表 objects）使用小写方式，而对于一些参数列表（例如：编译选项CFLAGS）采用大写方式，但这并不是要求的**。但需要强调一点：对于一个工程，所有 Makefile 中的变量命名应保持一种风格，否则会显得你是一个蹩脚的程序员（就像代码的变量命名风格一样）。

5. 另外有一些变量名只包含了一个或者很少的几个特殊的字符（符号）。称它们为自动化变量。 像“\$<”、“\$@”、“\$?”、“\$*”等。

## 6.1. 变量的引用

> Basics of Variable References.

当我们定义了一个变量之后，就可以在 Makefile 的很多地方使用这个变量。变量的引用方 式是：“\$(VARIABLE_NAME)” 或者 “\${VARIABLE_NAME}” 来引用一个变量的定义。例如: “\$(foo)”或者“\${foo}”就是取变量“foo”的值。美元符号 “\$” 在 Makefile 中有特殊的含义，所有在命令或者文件名中使用 “\$” 时需要用两个美元符号 “\$\$” 来表示。对一个变量的引用可以在 Makefile 的任何上下文中，目标、依赖、命令、绝大多数指示符和新变量的赋值中。这里有一个例 子，其中变量保存了所有.o文件的列表：

```makefile
objects = program.o foo.o utils.o

program : $(objects)
    cc -o program $(objects)
$(objects) : defs.h
```

变量引用的展开过程是严格的文本替换过程，就是说变量值的字符串被精确的展开在变量被引用的地方。因此规则：

```makefile
foo = c

prog.o : prog.$(foo)
	$(foo)$(foo) -$(foo) prog.$(foo)
```

被展开后就是：

```makefile
prog.o : prog.c
	cc -c prog.c
```

通过这个例子会发现变量的展开过程和 c 语言中的宏展开的过程相同，是一个严格的文本替换过程。上例中变量 “foo” 被展开的过程中，变量值中的前导空格会忽略。举这个例子的目的是为了让我们更清楚地了解变量的展开过程，而不是建议大家按照这样的方式来书写Makefile。在实际书写时，最好不要这么干。否则将会给你带来很多不必要的麻烦。

注意：Makefile中在对一些简单变量的引用，我们也可以不使用 “()” 和 “{}” 来标记变量名，而直接使用 “\$x” 的格式来实现，此种用法仅限于变量名为单字符的情况。另外自动化变量也使用这种格式。对于一般多字符变量的引用必须使用括号标记，否则 make 将把变量名的首字母作为作为变量而不是整个字符串(“\$PATH” 在 Makefile 中实际上是 “\$(P)ATH” )。这一点和 shell 中变量的引用方式不同。shell 中变量的引用可以是 “\${xx}” 或者 “\$xx” 格式。但在 Makefile 中多字符变量名的引用只能是 “\$(xx)” 或者 “\${xx}” 格式。

一般在我们书写 Makefile 时，各部分变量引用的格式我们建议如下：

1. make变量(Makefile中定义的或者是make的环境变量)的引用使用“\$(VAR)”格式，无论 “VAR ”是单字符变量名还是多字符变量名。

2. 岀现在规则命令行中 shell 变量(一般为执行命令过程中的临时变量，它不属于 Makefile 变量，而是一个 shell 变量)引用使用 shell 的 “\$tmp” 格式。

3. 对岀现在命令行中的 make 变量我们同样使用 “\$(CMDVAR)” 格式来引用。

例如：

```makefile
# sample Makefile
……
SUBDIRS := src foo
.PHONY : subdir
subdir :
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir || exit 1; \
	done
……
```

## 6.2. 两种变量定义（赋值）

> The Two Flavors of Variables.

在 GNU make 中，变量的定义有两种方式(或者称为风格)。我们把使用这两种方式定义的变量可以看作变量的两种不同风格。变量的这两种不同的风格的区别在于： 
1. 定义方式；
2. 展开时机

下边我们分别对这两种不同的风格进行详细地讨论。

### 6.2.1. 递归展开式变量

第一种风格的变量是递归方式扩展的变量。这一类型变量的定义是通过 “=” 或者使用指示符 "define" 定义的。这种变量的引用，在引用的地方是严格的文本替换过程，此变量值的字符串原模原样的出现在引用它的地方。如果此变量定义中存在对其他变量的引用，这些被引用的变量会在它被展开的同时被展开。就是说在变量定义时，变量值中对其他变量的引用不会被替换展开；而是变 量在引用它的地方替换展开的同时，它所引用的其它变量才会被一同替换展开。语言的描述可能比较晦涩，让我们来看一个例子：


```makefile
foo = $(bar)
bar = $(ugh)
ugh = Huh?

all:
    echo $(foo)
```

执行 “make” 将会打印岀 “Huh?”。整个变量的替换过程时这样的：首先“\$(foo)” 被替换为 “\$(bar)”，接下来 “\$(bar)”被替换为“\$(ugh)” ，最后"\$(ugh)"被替换为“Hug?”。整个替换的过程是在执行“echo $(foo)”时完成的。

这种类型的变量是其它版本的 make 所支持的类型。我们可以把这种类型的变量称为 “递归展开” 式变量。此类型变量存有它的优点同时也存在其缺点。 **其优点是：这种类型变量在定义时，可以引用其它的之前没有定义的变量(可能在后续部分定义，或者是通过 make 的命令行选项传递的变量)**。看一个这样的例子：

```makefile
CFLAGS = $(include_dirs) -O
include_dirs = -Ifoo -Ibar
```

"CFLAGS" 会在命令中被展开为 “-Ifoo-Ibar-O”。而在 “CFLAGS” 的定义中使用了其后才定义的变量“include_dirs”。

其缺点是：

**缺点一**：使用此风格的变量定义，可能会由于岀现变量的递归定义而导致 make 陷入到无限的变量展开过程中，最终使 make 执行失败。例如，接上边的例子，我们给这个变量追加值：

```makefile
CFLAGS = $(CFLAGS) -O
```

或：

```makefile
A = $(B)
B = $(A)
```


它将会导致 make 对变量"CFLAGS"的无限展过程中去(这种定义就是变量的递归定义)。因为一旦后续同样存在对 “CLFAGS" 定义的追加，展开过程将是套嵌的、不能终止的(在发生这种情况时， make 会提示错误信息并结束)。一般书写 Makefile 时，这种追加变量值的方法很少使用 (也不是我们推荐的方式)。看另外一个例子：

```makefile
x = $(y)
y = $(x) $(z)
```

这种情况下变量在进行展开时，同样会陷入死循环。所以对于此风格的变量，当在一个变量的定义中需要引用其它的同类型风格的变量时需特别注意，防止变量展开过程的死循环。

**缺点二**：这种风格的变量定义中如果使用了函数，那么包含在变量值中的函数总会在变量被引用的地方执行(变量被展开时)。

这是因为在这种风格变量的定义中，对函数引用的替换展开发生在变量展开的过程中，而不是在定义这个变量的时候。这样所带来的问题是：使 make 的执行效率降低(每一次在变量被展开时都要展开他所引用的函数)；另外在某些时候会岀现一些变量和函数的引用岀现非预期的结果。特 别是当变量定义中引用了 “shell” 和 “wildcard” 函数的情况，可能岀现不可控制或者难以预料的错误，因为我们无法确定它在何时会被展开。

### 6.2.2. 直接展开式变量

为了避免“递归展开式”变量存在的问题和不方便。GNUmake 支持另外一种风格的变量，称为“直接展开”式。这种风格的变量使用 “:=” 定义。在使用 “:=” 定义变量时，变量值中对其他变量或者函数的引用在定义变量时被展开(对变量进行替换)。所以变量被定义后就是一个实际需要的文本串，其中不再包含任何变量的引用。因此

```makefile
x := foo
y := $(x) bar
x := later
```

其等价于：

```makefile
y := foo bar
x := later
```

和递归展开式变量不同：此风格变量在定义时就完成了对所引用变量和函数的展开，因此不能实现对其后定义变量的引用。如：

```makefile
CFLAGS := $(include_dirs) -O
include_dirs := -Ifoo -Ibar
```

由于变量 “include_dirs” 的定义岀现在“CFLAGS”定义之后。因此在“CFLAGS”的定义中, “include_dirs” 的值为空。“CFLAGS” 的值为 “-O” 而不是 “-Ifoo-Ibar-O”。这一点也是直接展开式和递归展开式变量的不同点。注意这里的两个变量都是“直接展开”式的。大家不妨试试将其中某一个变量使用递归展开式定义后看一下又会岀现什么样的结果。

下边我们来看一个复杂一点的例子。分析一下直接展开式变量定义(:=)的用法，这里也用到了 make 的 shell 函数和变量 “MAKELEVEL” (此变量在make的递归调用时代表make的调用深度)。

其中包括了对函数、条件表达式和系统变量 “MAKELEVEL” 的使用：

```makefile
ifeq (0,${MAKELEVEL})
cur-dir := $(shell pwd)
whoami := $(shell whoami)
host-type := $(shell arch)
MAKE := ${MAKE} host-type=${host-type} whoami=${whoami}
endif
```

第一行是一个条件判断，如果是顶层Makefile,就定义下列变量。否则不定义任何变量。第二、三、四、五行分别定义了一个变量，在进行变量定义时对引用到的其它变量和函数展开。最后结束定义。利用直接展开式的特点我们可以书写这样一个规则：

```makefile
${subdirs}:
    ${MAKE} cur-dir=${cur-dir}/$@ -C $@ all
```

它实现了在不同子目录下变量 “cur_dir” 使用不同的值(为当前工作目录)。

在复杂的 Makefile 中，推荐使用直接展开式变量。因为这种风格变量的使用方式和大多数编程语言中的变量使用方式基本上相同。它可以使一个比较复杂的 Makefile 在一定程度上具有可预测性。而且这种变量允许我们利用之前所定义的值来重新定义它(比如使用某一个函数来对它以前的 值进行处理并重新赋值)，此方式在 Makefile 中经常用到。尽量避免和减少递归式变量的使用。

### 6.2.3. 如何定义一个空格

**使用直接扩展式变量定义我们可以实现将一个前导空格定义在变量值中**。一般变量值中的前导空格字符在变量引用和函数调用时被丢弃。利用直接展开式变量在定义时对引用的其它变量或函数进行展开的特点，我们可以实现在一个变量中包含前导空格并在引用此变量时对空格加以保护。像这样：

```makefile
nullstring :=
space := $(nullstring) # end of the line
```

这里，变量 “space” 就表示一个空格。在 “space” 定义行中的注释使得我们的目的更清晰(明确地描述一个空格字符比较困难)，**注释和变量引用“\$(nullstring)”之间存在一个空格**。通过这种方式我们就明确的指定了一个空格。这是一个很好地实现方式。通过引用变量 “nullstring” 标明变量值的开始，采用“#”注释来结束，中间是一个空格字符。

make 对变量进行处理时变量值中尾空格是不被忽略的，因此定义一个包含一个或者多个空格的变量定义时，上边的实现就是一个简单并且非常直观的方式。但是需要注意：当定义不包含尾空格的变量时，就不能使用这种方式，将变量定义和注释书写在同一行并使用若干空格分开。否则注释之前的空格会被作为变量值的一部分。例如下边的做法就是不正确的：

```makefile
dir := /foo/bar    # directory to put the frobs in
```

变量 “dir” 的值是 “/foo/bar    ”(后面有4个空格)，这可能并不是想要实现的。如果一个文件以它作为路径来表示“\$(dir)/file”，那么大错特错了。

在书写 Makefile 时。推荐将注释书写在独立的行或者多行，防止岀现上边例子中的意外情况而且将注释书写在独立的行也使得 Makefile 清晰，便于阅读。对于特殊的定义，比如定义包含一个或者多个空格空格的变量时进行详细地说明和注释。

### 6.2.4. “?=” 操作符

GNU make 中，还有一个被称为条件赋值的赋值操作符 “?=”。被称为条件赋值是因为：只有此变量在之前没有赋值的情况下才会对这个变量进行赋值。例如：

```
FOO ?= bar
```

其等价于：

```makefile
ifeq ($(origin FOO), undefined)
FOO = bar
endif
```

含义是：如果变量 “FOO” 在之前没有定义，就给它赋值 “bar”。否则不改变它的值。


## 6.3. 变量的高级用法

> Advanced Features for Reference to Variables.

本节讨论关于变量的高级用法，这些高级的用法使我们可以更灵活的使用变量。

### 6.3.1. 变量的替换引用

> Substitution References.

对于一个已经定义的变量，可以使用“替换引用”将其值中的后缀字符(串)使用指定的字符(字符串)替换。格式为 “\$(VAR:A=B)” (或者 “\${VAR:A=B}”)，意思是，替换变量 “VAR” 中 所有“A”字符结尾的字为“B”结尾的字。“结尾”的含义是空格之前(变量值多个字之间使用空 格分开)。而对于变量其它部分的 “A” 字符不进行替换。例如：

```makefile
foo := a.o b.o c.o
bar := $(foo:.o=.c)
```

在这个定义中，变量“bar"的值就为“a.c b.c c.c”。使用变量的替换引用将变量 “foo” 以空格分开的值中的所有的字的尾字符 “o” 替换为 “c”,其他部分不变。如果在变量 “foo” 中如果存在 “o.o” 时，那么变量 “bar” 的值为 “a.c b.c c.c o.c” 而不是 “a.c b.c c.c c.c”。

**注意：替换时，foo的值并没有发生改变**。

变量的替换引用其实是函数 “patsubst” 的一个简化实现。在GNU make中同时提供了这两种方式来实现同样的目的，以兼容其它版本make。

另外一种引用替换的技术使用功能更强大的"patsubst" 函数。它的格式和上面 “\$(VAR:A=B)” 的格式相类似，不过需要在 “A” 和 "B" 中需要包含模式字符 “%”。这时它和 "`$(patsubst A,B $(VAR))`" 所实现功能相同。例如：

```makefile
foo := a.o b.o c.o
bar := $(foo:%.o=%.c)
```

这个例子同样使变量 "bar" 的值为"a.c b.c c.c”。这种格式的替换引用方式比第一种方式更通用。

### 6.3.2. 变量的嵌套引用

> Computed Variable Names.

计算的变量名是一个比较复杂的概念，仅用在那些复杂的 Makefile 中。通常我们不需要对它的计算过程进行深入地了解，只要知道当一个被引用的变量名之中含有“\$”时，可得到另外一个值。 如果您是一个比较喜欢追根问底的人，或者想弄清楚 make 计算变量的过程。那么就可以参考本节的内容。

一个变量名(文本串)之中可以包含对其它变量的引用。这种情况我们称之为 “变量的套嵌引用” 或者 “计算的变量名”。先看一个例子：

```makefile
x = y
y = z
a := $($(x))
```

这个例子中，最终定义了 “a” 的值为“z”。来看一下变量的引用过程：首先最里边的变量引用 “\$(x)” 被替换为变量名 “y”(就是 “`$($(x))`” 被替换为了 “\$(y)”)，之后 “$(y)” 被替换为 “z” (就是a:=z)。这个例子中 (`a:=$($(x))`) 所引用的变量名不是明确声明的，而是由 \$(x) 扩展得到。 这里 “\$(x)” 相对于外层的引用就是套嵌的变量引用。

上个例子我们看到是一个两层的套嵌引用的例子，具有多层的套嵌引用在 Makefile 中也是允许的。下边我们在来看一个三层套嵌引用的例子：

```makefile
x = y
y = z
z = u
a := $($($(x)))
```

这个例子最终是定义了 “a” 的值为 “u”。它的扩展过程和上边第一个例子的过程相同。首先 “\$(x)” 被替换为 “y”，则 “`$($(x))`”就是 “\$(y)”，“\$(y)” 再被替换为 “z”，所以就有 “`a:=$(z)`”； 而 “\$(z)” 最后被替换为 “u”。

以上两个套嵌引用的例子中没有用到递归展开式变量的特点。递归展开式变量的变量名的计算过程，也是按照相同的方式被扩展的。例如：

```makefile
x = $(y)
y = z
z = Hello
a := $($(x))
```

此例最终实现了 “a:=Hello”这么一个定义。这里`$($(x))`被替换成了 `$($(y))`,因为 `$(y)` 值是 “z”，所以，最终结果是：`a:=$(z)`，也就是“Hello”。

递归变量的套嵌引用过程，也可以包含变量的修改引用和函数调用。看下边的例子，其中使用了 make 的文本处理函数：

```makefile
x = variable1
variable2 := Hello
y = $(subst 1,2,$(x))
z = y
a := $($($(z)))
```

此例同样的实现“a:=Hello”。“`$($($(z)))`” 首先被替换为 “`$($(y))`”,之后再次被替换为 “`$($(su-bst 1,2,$(x)))`” (“\$(x)” 的值是 “variable1”,所以有 “`$($(subst 1,2,$(variable1)))`”)。函数处理之后为 “`$(variable2)`”。 之后对它在进行替换展开。最终，变量“a”的值就是 “Hello”。从上边的例子中我们看到，计算的变量名的引用过程存在多层套嵌，也使用了文本处理函数。这个复杂的计算变 量的过程，会使很多人感到混乱甚至迷惑。上例中所要实现的目的就没有直接使用 “a:=Hello” 来的直观。在书写Makefile时，应尽量避免使用套嵌的变量引用。在一些必需的地方，也最好不要使用高于两级的套嵌引用。使用套嵌的变量引用时，如果涉及到递归展开式变量的引用时需要特别注 意。一旦处理不当就可能导致递归展开错误，从而导致难以预料的结果。

一个计算的变量名可以不是对一个完整、单一的其他变量的引用。其中可以包含多个变量的引用，也可以包含一些文本字符串。就是说，计算变量的名字可以由一个或者多个变量引用同时加上字符串混合组成。例如：

```makefile
a_dirs := dira dirb
1_dirs := dir1 dir2

a_files := filea fileb
1_files := file1 file2

ifeq "$(use_a)" "yes"
a1 := a
else
a1 := 1
endif

ifeq "$(use_dirs)" "yes"
df := dirs
else
df := files
endif

dirs := $($(a1)_$(df))
```

这个例子对变量 “dirs” 进行定义，变量的可能取值为 “a_dirs”、“1_dirs”、“a_files” 和“ a_files ” 四个之一，具体依赖于 “use_a” 和 “use_dirs” 的定义。

计算的变量名也可以使用上一小节我们讨论过的“变量的替换引用”。例如：

```makefile
a_objects := a.o b.o c.o
1_objects := 1.o 2.o 3.o
sources := $($(a1)_objects:.o=.c)
```

这个例子实现了变量“sources”的定义，它的可能取值为 “a.c b.c c.c” 和 “1.c 2.c 3.c”, 具体依赖于“a1”的定义。大家自己分析一下计算变量名的过程。

**使用嵌套的变量引用的唯一限制是，不能通过指定部分需要调用的函数名称(调用的函数包括 了函数名本身和执行的参数)来实现对这个函数的调用。这是因为套嵌引用在展开之前已经完成了 对函数名的识别测试**。我们来看一个例子，此例子试图将函数执行的结果赋值给一个变量：

```makefile
ifdef do_sort
func := sort
else
func := strip
endif

bar := a d b g q c

foo := $($(func) $(bar))
```

此例的本意是将 “sort” 或者 “strip” (依赖于是否定义了变量“do_sort")以“a d b g q c" 的执行结果赋值变量 “foo"。 在这里使用了套嵌引用方式来实现，但是本例的结果是：变量 “foo" 的值为字符串 “sort a d b g q c” 或者 “strip adgq c”。这是目前版本的 make 在处理套嵌变量引用时的限制。


计算的变量名可以用在：

1. 一个使用赋值操作符定义变量的左值部分；
2. 使用 “define” 定义的变量名中。例如：

```makefile
dir = foo
$(dir)_sources := $(wildcard $(dir)/*.c)
define $(dir)_print
lpr $($(dir)_sources)
endef
```

在这个例子中我们定义了三个变量：“dir”，“foo_sources”和“foo_print”。

计算的变量名在进行替换时的顺序是：从最里层的变量引用开始，逐步向外进行替换。一层层展开直到最后计算岀需要应用的具体的变量，之后进行替换展开得到实际的引用值。

变量的套嵌引用(计算的变量名)在我们的 Makefile 中应该尽量避免使用。**在必需的场合使用时掌握的原则是：套嵌使用的层数越少越好，使用多个两层套嵌引用代替一个多层的套嵌引用**。如果在你的 Makefile 中存在一个层次很深的套嵌引用。会给其他人阅读造成很大的困难。而且变量的多级套嵌引用在某些时候会使简单问题复杂化。

**作为一个优秀的程序员，在面对一个复杂问题时，应该是寻求一种尽可能简单、直接并且高效的处理方式来解决，而不是将一个简单问题在实现上复杂化。如果想在简单问题上突岀自己使用某种语言的熟练程度，是一种非常愚蠢、且不成熟的行为**。

**注意：套嵌引用的变量和递归展开的变量在本质上存在区别**。套嵌的引用就是使用一个变量表示另外一个变量，或者更多的层次；而递归展开的变量表示当一个变量存在对其它变量的引用时，对这变量替换的方式。递归展开在另外一个角度描述了这个变量在定义时赋予它的一个属性或者风格。并且我们可以在定义个一个递归展开式的变量时使用套嵌引用的方式，但是建议你的实际编写 Makefile 时要尽量避免这种复杂的用法。

## 6.4. 变量取值

>  How Variables Get Their Values.

一个变量可以通过以下几种方式来获得值：

- 在运行 make 时通过命令行选项来取代一个己定义的变量值。
- 在 makefile 文件中通过赋值的方式或者使用 “define" 来为一个变量赋值。
- 将变量设置为系统环境变量。所有系统环境变量都可以被 make 使用。
- 自动化变量，在不同的规则中自动化变量会被赋予不同的值。它们每一个都有单一的习惯性用法。
- 一些变量具有固定的值。

### make 命令行定义变量 (补充)

语法：

```
make 变量名=变量值
make 变量名:=变量值
```

## 6.5. 如何设置变量

> Setting Variables.

Makefile 中变量的设置（也可以称之为定义）是通过“="（递归方式）或者 “:="（静态方式） 来实现的。“=” 和 “:=” 左边的是变量名，右边是变量的值。下边就是一个变量的定义语句： 

```
objects = main.o foo.o bar.o utils.o
```

这个语句定义了一个变量"objects",其值为一个.o文件的列表。变量名两边的空格和“=" 之后的空格在 make 处理时被忽略。

使用“=”定义的变量称之为“递归展开”式变量；使用“:=”定义的变量称为“直接展开”式变量，“直接展开”式的变量如果其值中存其他变量或者函数的引用，在定义时这些引用将会被替换展开。

定义一个变量时需要明确以下几点：

1. 变量名之中可以包含函数或者其它变量的引用，make在读入此行时根据己定义情况进行替换展开而产生实际的变量名。

2. 变量的定义值在长度上没有限制。不过在使用时还是需要根据实际情况考虑，保证你的机器 上有足够的可用的交换空间来处理一个超常的变量值。变量定义较长时，一个好的做法就是 将比较长的行分多个行来书写，除最后一行外行与行之间使用反斜杠（\\）连接，表示一个完整的行。这样的书写方式对 make 的处理不会造成任何影响，便于后期修改维护而且使得你的 Makefile 更清晰。

3. 当引用一个没有定义的变量时， make 默认它的值为空。

4. 一些特殊的变量在 make 中有内嵌固定的值，不过这些变量允许我们在 Makefile 中显式得重新给它赋值。

5. 还存在一些由两个符号组成的特殊变量，称之为自动化变量。它们的值不能在 Makefile 中进行显式的修改。这些变量使用在规则中时，不同的规则中它们会被赋予不同的值。

6. 如果你希望实现这样一个操作，仅对一个之前没有定义过的变量进行赋值。那么可以使用速记符“?=”（条件方式）来代替“=”或者“:=”来实现。

## 6.6. 追加变量值

> Appending More Text to Variables.

通常，一个通用变量在定义之后的其他一个地方，可以对其值进行追加。这是非常有用的。我们可以在定义时（也可以不定义而直接追加）给它赋一个基本值，后续根据需要可随时对它的值进 行追加（增加它的值）。在 Makefile 中使用 “+=”（追加方式）来实现对一个变量值的追加操作。像下边那样：

```
objects += another.o
```

这个操作把字符串“another.o”添加到变量“objects”原有值的末尾，使用空格和原有值分开。 因此我们可以看到：

```makefile
objects = main.o foo.o bar.o utils.o
objects += another.o
```

上边的两个操作之后变量“objects”的值就为：“main.o foo.o bar.o utils.o another.o”。使用 “+=” 操作符，相当于：

```
objects = main.o foo.o bar.o utils.o
objects := $(objects) another.o
```

但是，这两种方式可能在简单一些的 Makefile 有相同的效果，复杂的 Makefile 中它们之间的差异就会导致一些问题。为了方便我们调试，了解这两种实现的差异还是很有必要的。

1.如果被追加值的变量之前没有定义，那么，“+=” 会自动变成 “=”，此变量就被定义为一个递归展开式的变量。如果之前存在这个变量定义，那么 “+=” 就继承之前定义时的变量风格。

2.直接展开式变量的追加过程：变量使用 “:=” 定义，之后“+=”操作将会首先替换展开之前 此变量的值，尔后在末尾添加需要追加的值，并使用 “:=” 重新给此变量赋值。实际的过程像下边那样：

```makefile
variable := value
variable += more
```

等价于：

```makefile
variable := value
variable := $(variable) more
```

3.递归展开式变量的追加过程：一个变量使用“=”定义，之后“+=”操作时不对之前此变量值中的任何引用进行替换展开，而是按照文本的扩展方式（之前等号右边的文本未发生变化） 替换，尔后在末尾添加需要追加的值，并使用“=”给此变量重新赋值。实际的过程和上边的相类似：

```makefile
variable = value
variable += more
```

就是：

```makefile
temp = value
variable = $(temp) more
```

当然了，上边的过程并不会存在中间变量：“temp”，使用它的目的是方便描述。这种情况时如果 “value” 中存在某种引用，情况就有些不同了。看我们通常一个会用到的例子：

```makefile
CFLAGS = $(includes) -O
CFLAGS += -pg # enable profiling
```

第一行定义了变量“CFLAGS”，它是一个递归展开式的变量。因此 make 在处理它的定义时 不会对其值中的引用 “\$(includes)” 进行展开，它的替换展开是在变量 “CFLAGS” 被引用的规则中。因此，变量 “include” 可以在 “CFLAGS” 之前不进行定义，只要它在实际引用 “CFLAGS” 之前定义就可以了。但是如果给 “CFLAGS” 追加值使用 “:=” 操作符，我们按照下边那样实现：

```makefile
CFLAGS := $(CFLAGS) -pg # enable profiling
```

这样似乎好像很正确，但是实际上它在有些情况时却不是你所要实现的。来看看，因为 “:=” 操作符定义的是直接展开式变量，因此变量值中对其它变量或者函数的引用会在定义时进行展开。 在这种情况下，如果变量 “includes” 在之前没有进行定义的话，变量 “CFLAGS” 的值为 “-O-pg” (\$(includes)被替换展开为空字符)。而其后岀现的 “includes” 的定义对 “CFLAGS” 将不产生影响。相反的情况，如果在这里使用 “+=” 实现：

```makefile
CFLAGS += -pg # enable profiling
```

那么变量 “CFLAGS” 的值就是文本串 “\$(includes) -O -pg",因为之前 “CFLAGS” 定义为递归展开式，所以追加值时不会对其值的引用进行替换展开。因此变量 "includes" 只要岀现在规则对 “CFLAGS” 的引用之前定义，它都可以对 “CFLAGS” 的值起作用。对于递归展开式变量的追加， make 程序会同样会按照递归展开式的定义来实现对变量的重新赋值，不会发生递归展开式变量展开过程的无限循环。


## 6.7. override 指示符

> The override Directive.

**通常在执行 make 时，如果通过命令行定义了一个变量，那么它将替代在 Makefile 中岀现的同名变量的定义**。就是说，对于一个在 Makefile 中使用常规方式(使用“ = ”、“:=” 或者 “define”) 定义的变量，我们可以在执行 make 时通过命令行方式重新指定这个变量的值，命令行指定的值将替代岀现在 Makefile 中此变量的值。**如果不希望命令行指定的变量值替代在 Makefile 中的变量定义，那么我们需要在 Makefile 中使用指示符 “override” 来对这个变量进行声明**，像下边那样：

```makefile
override VARIABLE = VALUE

# 或

override VARIABLE := VALUE
```


也可以对变量使用追加方式：

```makefile
override VARIABLE += MORE TEXT
```

**对于追加方式需要说明的是：变量在定义时使用了 “override”,则后续对它值进行追加时，也需要使用带有 “override” 指示符的追加方式。否则对此变量值的追加不会生效**。

**指示符 “override” 并不是用来调整 Makefile 和执行时命令参数的冲突，其存在的目的是为了使用户可以改变或者追加那些使用 make 的命令行指定的变量的定义**。从另外一个角度来说，就是实现了在 Makefile 中增加或者修改命令行参数的一种机制。我们可能会有这样的需求；可以通过命令行来指定一些附加的编译参数，对一些通用的参数或者必需的编译参数在 Makefile 中指定，而在命令行中指定一些特殊的参数。对于这种需求，我们就需要使用指示符 “override” ”来实现。

例如：无论命令行指定那些编译参数，编译时必须打开 “-g” 选项，那么在 Makefile 中编译选项 “CFLAGS”应该这样定义：

```makefile
override CFLAGS += -g
```

这样，在执行make时无论在命令行中指定了那些编译选项(“指定CFLAGS” 的值)，编译时 “-g” 参数始终存在。

同样，使用 “define” 定义变量时同样也可以使用 “override” 进行声明。例如： 

```
override define foo
bar
endef
```

最后我们来看一个例子：


```makefile
# sample Makefile
EXEF = foo
override CFLAGS += -Wall –g
.PHONY : all debug test
all : $(EXEF)
foo : foo.c

```

执行：`make CFLAGS=-O2` 将显示编译 “foo” 的过程是 “`cc -02 -Wall -g foo.c -o foo`”。执行 “`make CFLAGS=-O2 debug`"可以查看到变量 “CFLAGS” 的值为"`-O2 -Wall -g`”。另外，这 个例子中，如果把变量"CFLAGS"之前的指示符“override”去掉，使用相同的命令将得到不同的结果:

`make CFLAGS=-O2` 显示 `cc -O2 foo.c -o foo`;
`make CFLAGS=-O2 debug` 显示 `-O2`.

## 6.8. 多行定义

> Defining Multi-Line Variables.

定义变量的另外一种方式是使用 “define” 指示符。它定义一个包含多行字符串的变量，我们就是利用它的这个特点实现了一个完整命令包的定义。使用 “define” 定义的命令包可以作为 "eval" 函数的参数来使用。

本文的前些章节己经不止一次的提到并使用了 “define”。相信大家己经有所了解。本节就 “define” 定义变量从以下几个方面来讨论：

1."define" 定义变量的语法格式：以指示符 “define”开始，"endif" 结束，之间的所有内容就是所定义变量的值。所要定义的变量名字和指示符 “define” 在同一行，使用空格分开；指示符所在行的下一行开始一直到 “endif” 所在行的上一行之间的若干行，是变量值。

```makefile
define two-lines
echo foo
echo $(bar)
endef
```

如果将变量 “two-lines” 作为命令包执行时，其相当于：

```
two-lines = echo foo; echo $(bar)
```

大家应该对这个命令的执行比较熟悉。它把变量 “two-lines” 的值作为一个完整的 shell 命令行来处理（是使用分号 “;” 分开的在同一行中的两个命令而不是作为两个命令行来处理），保证了变量完整。

2.变量的风格：使用 “define” 定义的变量和使用 “=” 定义的变量一样，属于 “递归展开” 式的变量，两者只是在语法上不同。因此“define”所定义的变量值中，对其它变量或者函数引用不会 在定义变量时进行替换展开，其展开是在 “define” 定义的变量被展开的同时完成的(即引用 define 定义的变量时才展开)。

3.可以套嵌引用。因为是递归展开式变量，所以在嵌套引用时 “\$(x)” 将是变量的值的一部分。

4.变量值中可以包含：换行符、空格等特殊符号（**注意如果定义中某一行是以 [Tab] 字符开始时，当引用此变量时这一行会被作为命令行来处理**）。

5.可以使用 “override” 在定义时声明变量：这样可以防止变量的值被命令行指定的值替代。例如：

```makefile
override define two-lines
foo
$(bar)
endef
```

## 6.9 取消变量定义 (补充)

> Undefining Variables.

如果要清除一个变量，通常将其值设置为空就足够了。展开这样的变量将产生相同的结果(空字符串)，而不管它是否被设置。但是，如果您正在使用flavor([参见flavor函数](/function.md#flavor_func)和origin([参见origin函数](/function.md#origin_func))函数，**则从未设置的变量和空值的变量之间存在差异**。在这种情况下，你可能想要使用 `undefine` 指令使一个变量看起来像从未设置过一样。例如:

```makefile
foo := foo
bar = bar

undefine foo
undefine bar

$(info $(origin foo))
$(info $(flavor bar))
```

这个例子将为两个变量输出 “undefined”。

如果你想取消命令行变量定义，你可以使用 override 指令和 undefine，类似于变量定义:

```makefile
override undefine CFLAGS
```

## 6.10. 系统环境变量

> Variables from the Environment.

make 在运行时，系统中的所有环境变量对它都是可见的。在 Makefile 中，可以引用任何已定义的系统环境变量。 （这里我们区分系统环境变量和 make 的环境变量，系统环境变量是这个系统所有用户所拥有的，而 make 的环境变量只是对于 make 的一次执行过程有效，以下正文中岀现没有限制的 “环境变量” 时默认指的是“系统环境变量”，在特殊的场合我们会区分两者）正因为如 此，我们就可以设置一个命名为 “CFLAGS” 的环境变量，用它来指定一个默认的编译选项。就可以在所有的 Makefile 中直接使用这个变量来对c源代码就行编译。通常这种方式是比较安全的，但是它的前提是大家都明白这个变量所代表的含义，没有人在 Makefile 中把它作其他的用途。当然 了，你也可以在你的 Makefile 中根据你的需要对它进行重新定义。

使用环境变量需要注意以下几点：

1. 在 Makefile 中对一个变量的定义或者以 make 命令行形式对一个变量的定义，都将覆盖同名的环境变量（**注意：它并不改变系统环境变量定义，被修改的环境变量只在 make 执行过程有效**）。而 make 使用 “-e” 参数时，Makefile 和命令行定义的变量不会覆盖同名的环境变量，make 将使用系统环境变量中这些变量的定义值。

2. make 的递归调用中，所有的系统环境变量会被传递给下一级 make。 默认情况下，只有环 境变量和通过命令行方式定义的变量才会被传递给子 make 进程。在 Makefile 中定义的普通变量需要传递给子 make 时需要使用 “export” 指示符来对它声明。

3. 一个比较特殊的是环将变量 “SHELL”。在系统中这个环境变量的用途是用来指定用户和系 统的交互接口，显然对于make是不合适的。因此 make 的执行环境变量 “SHELL” 没有使用同名的环境变量定义，而是“/bin/sh”。make默认“/bin/sh”作为它的命令行解释程序（make在执行之前将变量 “SHELL” 设置为 “/bin/sh”）。

我们不推荐使用环境变量的方式来完成普通变量的工作，特别是在 make 的递归调用中。任何一个环境变量的错误定义都对系统上的所有 make 产生影响，甚至是毁坏性的。因为环境变量具有全局的特征。所以尽量不要污染环境变量，造成环境变量名字污染。我想大多数系统管理员都明白 环境变量对系统是多么的重要。

我们来看一个例子，结束本节。假如我们的机器名为 “server-cc”；我们的Makefile内容如下：

```makefile
# test makefile
HOSTNAME = server-http
......

.PHONY : debug
debug :
	@echo "hostname is : $(HOSTNAME)"
	@echo "shell is $(SHELL)"
```

1.执行 “make debug” 将显示 

```
hostname is : server-http
shell is /bin/sh
```

2.执行 “make -e debug"；将显示：

```
hostname is : server-cc
shell is /bin/sh
```

3.执行 “make -e HOSTNAEM=server-ftp”；将显示：

```
hostname is : server-ftp
shell is /bin/sh
```

记住：除非必须，否则在你的 Makefile 中不要重置环境变量“SHELL”的值。因为一个不正确的命令行解释程序可能会导致规则定义的命令执行失败，甚至是无法执行！当需要重置它时，必须有充分的理由和配套的规则命令来适应这个新指定的命令行解释程序。

## 6.11. 目标指定变量

> Target-specific Variable Values.

在 Makefile 中定义一个变量，那么这个变量对此 Makefile 的所有规则都是有效的。它就像是一个“全局的”变量（仅限于定义它的那个 Makefile 中的所有规则，如果需要对其它的 Makefile 中的规则有效，就需要使用 “export” 对它进行声明。类似于c语言中的全局静态变量，使用static 声明的全局变量）。当然“自动化变量”除外。

另外一个特殊的变量定义就是所谓的“目标指定变量（Target-specific Variable）”。此特性允许对于相同变量根据目标指定不同的值，有点类似于自动化变量。目标指定的变量值只在指定它的目标的上下文中有效，对于其他的目标没有影响。就是说目标指定的变量具有只对此目标上下文有效的“局部性”。

设置一个目标指定变量的语法为：

```
TARGET ... : VARIABLE-ASSIGNMENT
```

或者：

```
TARGET ... : override VARIABLE-ASSIGNMENT
```

一个多目标指定的变量的作用域是所有这些目标的上下文，它包括了和这个目标相关的所有执行过程。

目标指定变量的一些特点：

1. “VARIABLE-ASSIGNMENT” 可以使用任何一个有效的赋值方式，“=”（递归）、“:=” 静态）“+=”（追加）或者 “?=”（条件）。

2. 使用目标指定变量值时，目标指定的变量值不会影响同名的那个全局变量的值。就是说目标指定一个变量值时，如果在 Makefile 中之前已经存在此变量的定义(非目标指定的)，那么对于其它目标全局变量的值没有变化。变量值的改变只对指定的这些目标可见。

3. 目标指定变量和普通变量具有相同的优先级。就是说，当我们使用 make 命令行的方式定义变量时，命令行中的定义将替代目标指定的同名变量定义(和普通的变量一样会被覆盖)。另外当使 用 make 的 “-e” 选项时，同名的环境变量也将覆盖目标指定的变量定义。因此为了防止目标指定的变量定义被覆盖，可以使用第二种格式，使用指示符 “ override” 对目标指定的变量进行声明。

4. 目标指定的变量和同名的全局变量属于两个不同的变量，它们在定义的风格(递归展开式 和直接展开式)上可以不同。

5. **目标指定的变量变量会作用到由这个目标所引发的所有的规则中去**。例如：

```
prog : CFLAGS = -g
prog : prog.o foo.o bar.o
    $(CC) $(CFLAGS) prog.o foo.o bar.o
prog.o : prog.c
    $(CC) $(CFLAGS) prog.c
foo.o : foo.c
    $(CC) $(CFLAGS) foo.c
bar.o : bar.c
    $(CC) $(CFLAGS) bar.c
```

这个例子中，无论 Makefile 中的全局变量 “CFLAGS” 的定义是什么。对于目标 "prog" 以及其所引发的所有(包含目标为“prog.o”、“foo.o”和"bar.o"的所有规则)规则，变量 "CFLAGS" 值都是“ -g”。

使用目标指定变量可以实现对于不同的目标文件使用不同的编译参数。看一个例子：


```makefile
# sample Makefile

CUR_DIR = $(shell pwd)
INCS := $(CUR_DIR)/include
CFLAGS := -Wall –I$(INCS)
EXEF := foo bar

.PHONY : all clean

all : $(EXEF)
foo : foo.c
foo : CFLAGS+=-O2
bar : bar.c
bar : CFLAGS+=-g
......
......
$(EXEF) : debug.h
	$(CC) $(CFLAGS) $(addsuffix .c,$@) –o $@
clean :
	$(RM) *.o *.d $(EXEF)
```

这个 Makefile 文件实现了在编译程序 "foo" 使用优化选项 “-02” 但不使用调试选项 "-g",而 在编译 “bar" 时采用了 “-g" 但没有 “-02”。这就是目标指定变量的灵活之处。目标指定变量的其它特性大家可以修改这个简单的 Makefile 来进行验证！

## 6.12. 模式指定变量

> Pattern-specific Variable Values.

GNU make 除了支持上一节所讨论的模式指定变量之外，还支持另外一种方式：模式指定变量 (Pattern-specific Variable),使用目标定变量定义时，此变量被定义在某个具体目标和由它所引发的规则的目标上。而模式指定变量定义是将一个变量值指定到所有符合此模式的目标上。对于同一个变量如果使用追加方式，**通常对于一个目标，它的局部变量值是：(为所有规则定义的全局值) + (引发它所在规则被执行的目标所指定的值) + (它所符合的模式指定值) + (此目标所指定的值)**。 这个大家也不需要深入了解。

设置一个模式指定变量的语法和设置目标变量的语法相似：

```
PATTERN ... : VARIABLE-ASSIGNMENT
```

或者

```
PATTERN ... : override VARIABLE-ASSIGNMENT
```

和目标指定变量语法的唯一区别就是：这里的目标是一个或者多个“模式”目标(包含模式字 符“%”)。例如我们可以为所有的 .o 文件指定变量 “CFLAGS” 的值：

```
%.o : CFLAGS += -O
```

它指定了所有.o文件的编译选项包含“-O”选项，不改变对其它类型文件的编译选项。 需要说明的是：在使用模式指定的变量定义时。目标文件一般除了模式字符(%)以外需要包含某种文件名的特征字符(例如：“a%”、“%.o”、“%.a”等)。当单独使用 “%” 作为目标时，指定的变量会对所有类型的目标文件有效。


## 6.13 抑制继承 - private 修饰符 (补充)

> Suppressing Inheritance.

在前面的章节中所描述的那样，make 变量通过依赖继承。该功能允许您根据导致重新构建依赖的目标来修改依赖的行为。

例如，你可能在一个叫 debug 的目标上设置列一个目标指定变量，然后运行 make debug。这可以导致变量被所有debug 的依赖所继承。但是运行 make all 就不会有这些赋值。

但是有时候，你也许不希望一个变量被继承。在这种场合下，make 提供了一个 private 修饰符。尽管这个 private 修饰符可以用在任何变量赋值上面，但它在目的或者模式特定的变量赋值上面更有意义。**任何标记为 private 的目的仅仅对本地的目的可见，并且不会被其依赖所继承**。

```makefile
EXEF = foo

.PHONY : all debug test
all : $(EXEF)
foo : foo.c

debug : prog

prog: private EXTRA_CFLAGS = -L/usr/local/lib
prog: a.o
	@echo in prog : $(EXTRA_CFLAGS)
a.o : a.c
	@echo in main : $(EXTRA_CFLAGS)
```

使用 `make debug -n` 调试，输出的结果为：

```
echo in main :
echo in prog : -L/usr/local/lib
```

如果取消 private，再次执行，输出结果为：

```
echo in prog : -L/usr/local/lib
echo in main : -L/usr/local/lib
```

由于 `private` 修饰符，a.o 和 b.o 不会从 prog 目标继承 `EXTRA_CFLAGS` 变量赋值。

***Note***: <font color="red">gnu make 官网手册此示例只列出部分代码，容易产生歧义，笔者修改补充为完整的代码示例。</font>

<a id="spec_var"></a>

## 6.14 其他特殊变量  (补充)

> Other Special Variables.

GNU make支持一些具有特殊属性的变量。

### MAKEFILE_LIST

[see](/summarize.md?#var_makefile_list)

### MAKEFILES

[see](/summarize.md?#var_makefiles)

### .DEFAULT_GOAL

设置在命令行上没有指定目标时使用的默认目标(请参阅[Arguments to Specify the Goals](/make-run.md#specify_goals))。`.DEFAULT_GOAL` 变量允许您发现当前的默认目标，通过清除其值重新启动默认目标选择算法，或显式地设置默认目标。下面的例子说明了这些情况:

```makefile
# Query the default goal.
ifeq ($(.DEFAULT_GOAL),)
  $(warning no default goal is set)
endif

.PHONY: foo
foo: ; @echo $@

$(warning default goal is $(.DEFAULT_GOAL))

# Reset the default goal.
.DEFAULT_GOAL :=

.PHONY: bar
bar: ; @echo $@

$(warning default goal is $(.DEFAULT_GOAL))

# Set our own.
.DEFAULT_GOAL := foo
```

`make` 执行输出:

```makefile
no default goal is set
default goal is foo
default goal is bar
foo
```

**注意**，向 `.DEFAULT_GOAL` 指定多个目标名是无效的，并将导致错误。

### MAKE_RESTARTS (具体实例待补充)

只有当make实例重新启动时才设置这个变量(请参阅 [How Makefiles Are Remade](/summarize.md#remaking-makefiles) ):它将包含该实例重新启动的次数,用以确定GNU Make是否在 makefile 中执行第一个或第二个...这样的传递。注意，这与递归(由 MAKELEVEL 变量计算)不同。您不应该设置、修改或导出此变量。

### MAKE_TERMOUT / MAKE_TERMERR

当 make 启动时，它将检查 stdout 和 stderr 是否会在终端上显示它们的输出。如果是，它将分别将 `MAKE_TERMOUT` 和`MAKE_TERMERR` 设置为终端设备的名称(如果不能确定，则为true)。如果设置了，这些变量将被标记为 `export`。这些变量不会被make 更改，如果已经设置了，也不会被修改。

可以使用这些值(特别是与输出同步结合使用时(参见并行执行时的输出)来确定make本身是否正在写入终端;例如，可以测试它们，以决定是否强制命令生成彩色输出。

如果您调用了一个子 make 并重定向了它的 stdout 或 stderr，那么如果您的 makefile 依赖于这些变量，那么重置或 unexport 这些变量也是您的责任。

### .RECIPEPREFIX 修改默认命令标识Tab字符

这个变量值的第一个字符被用作make假设引入一个命令行的字符。即这个变量值的第一个字符开始的后面的内容将解释为命令。
如果变量为空(默认情况下是空的)，该字符就是标准制表符。例如，这是一个有效的 makefile:

```makefile
.RECIPEPREFIX = >
all:
> @echo Hello, world
```

上例将默认的 [Tab] 字符后面跟命令修改为 `>` 后跟命令。

由于默认的标准Tab字符在许多常用的编辑器中会默认替换为4个或其他数量的空格，这样使 makefile 存在语法错误：多个空格开始的规则行并不会被 make 解释为命令。为了解决这个常见的错误，我们可以将默认的标志命令行的 Tab 字符修改为其他可见非字母字符，避免出错。但修改为其他可见字符时尽量不要和 makefile 内置/隐含的 特殊字符相同，容易引起歧义、混乱、甚至产生错误。

`.RECIPEPREFIX`的值可以被多次修改;一旦设置，它将对所有解析的规则保持有效，直到修改为止。

这个特性是否为 GNU make 所独有，目前在其他 make 中暂未验证。因此为了考虑兼容性，建议不要修改这个变量的值。

### .VARIABLES

展开为到目前为止定义的所有全局变量的名称列表。这包括具有空值的变量，以及内置变量(请参阅隐式规则使用的变量)，但不包括仅在特定目标上下文中定义的任何变量。注意，你给这个变量赋的任何值都会被忽略;它总是返回它的特殊值。

[details see](/summarize.md?#var_dot_variables)

### .FEATURES (待完善)

展开为这个版本的make所支持的特殊特性列表。可能的值包括但不限于:

- 'archives'  
支持ar(归档)文件使用特殊的文件名语法。请参阅[使用make更新归档文件](/update-static-lib.md#archives)。
- 'check-symlink'  
支持 `-L(--check-symlink-times)` 标志. [see Summary of Options](/make-option.md)
- 'else-if'  
支持" else if "非嵌套条件。[see Syntax of Conditionals](/condition.md#cond_syntax)
- 'jobserver'  
支持“job server”增强的并行构建。[See Parallel Execution](#parallel).
- 'oneshell'  
支持.ONESHELL特殊目标。[](#one-shell)
- 'order-only'  
Supports order-only prerequisites. [See Types of Prerequisites](#prerequisite-types).
- 'second-expansion'  
支持依赖列表的二次扩展。
- 'shortest-stem'  
使用“最短的茎”的方法选择哪个模式，多个适用的选项，将被使用。[See How Patterns Match](#pattern-match).
- 'target-specific'  
Supports target-specific and pattern-specific variable assignments. See [Target-specific Variable Values](#target_specific).
- 'undefine'  
Supports the undefine directive. [see](#var_undefine).
- 'guile'  
GNU Guile可用作嵌入式扩展语言吗? See [GNU Guile Integration](http://www.gnu.org/software/make/manual/make.html#Guile-Integration).
- 'load'  
支持用于创建自定义扩展的动态可加载对象。See [Loading Dynamic Objects](http://www.gnu.org/software/make/manual/make.html#Loading-Objects).

### .INCLUDE_DIRS

展开为搜索所包含的makefile的目录列表。see [Including Other Makefiles]()

### .EXTRA_PREREQS (完善中)

**这个变量中的每个单词都是一个新的依赖，它被添加到为其设置的目标中**。这些依赖不同于正常的依赖，因为它们不会出现在任何自动变量中(参见[自动变量]())。这允许定义不影响命令的依赖。

考虑一个连接程序的规则:

```makefile
myprog: myprog.o file1.o file2.o
       $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
```

现在假设您想要增强这个makefile，以确保对编译器的更新会导致重新链接程序。您可以添加编译器作为依赖，但必须确保它没有作为参数传递给link命令。你需要这样的东西:

```makefile
myprog: myprog.o file1.o file2.o $(CC)
       $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(filter-out $(CC),$^) $(LDLIBS)
```

然后考虑有多个额外的依赖:它们都必须被过滤掉。使用 `.EXTRA_PREREQS` 和特定于目标的变量提供了一个更简单的解决方案:

```makefile
myprog: myprog.o file1.o file2.o
       $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
myprog: .EXTRA_PREREQS = $(CC)
```

如果你想在 makefile 中添加一些依赖，但又不容易修改，那么这个特性也很有用:

```makefile
myprog: .EXTRA_PREREQS = $(CC)
```

然后调用 `make -f extra.mk -f Makefile`.

全局设置 `.EXTRA_PREREQS` 将导致这些依赖被添加到所有目标(这些目标本身不会用特定于目标的值覆盖它)。
注意 make 很聪明，没有添加 `.EXTRA_PREREQS` 中列出的依赖作为自己的依赖。

<center><b>(本章完)</b></center>

<a id="foot"></a>

[to top](#top)

---

- [上一章：5.Makefile 命令](/command.md)
- [下一章：7.Makefile 条件执行](/condition.md)
