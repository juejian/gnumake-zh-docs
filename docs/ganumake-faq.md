# GNU make FAQ

**1.为何要在依赖关系中显式列出源文件中包含的头文件？**

Ans: 既然 gcc 编译程序时，会在指定的查找目录或系统头文件包含目录中自动查找引入的头文件，那在编写 Makefile 时就可以省略啊，为何 Makefile 手册建议我们最好显式指定依赖的头文件？原因是显式指定后，如果指定的头文件发生变化，那么下次执行 make 时，依赖它的所有目标会重新生成。否则只更新头文件，make 不会重建依赖它的目标。

**2.Makefile 中命令前面的TAB字符可以用4个或更多空格代替吗?**

Ans: 绝对不可以。这样做 make 执行时，出现错误：`*** missing separator.  Stop.`。这一点尤其要重视，因为很多编辑器中全局设置都会将 tab 字符用 4 个空格中替换，这种设置会使得编写的 Makefile 出现错误。因此编写 Makefile 前，必须要设置编辑器保留 TAB 字符。

像 VScode 这种智能编辑器默认会根据识别文件的类型来决定是保留 TAB 字符还是用4个空格替换，这样可避免手动切换设置，不易出错，对应的设置选项为：`"editor.detectIndentation": true`。如果要手动设置保留 TAB 字符，需要在工作区 `.vscode/settings.json` 文件中添加：

```
"editor.insertSpaces": false,
"editor.detectIndentation": false,
```

**3.CPPFLAGS是表示C++编译选项的 make 默认变量？**

Ans: 错误。CPPFLAGS 是表示C预处理器选项的make 默认变量，而 CXXFLAGS 才是表示C++编译选项的 make 默认变量。  
由于 c++ 默认的源文件后缀名为 `.cpp`，很多人经常误认为 CPPFLAGS 是表示C++编译选项的 make 默认变量。

**4.为何** `make —C subdir` **不能切换到 subdir 目录，但** `make --directory=subdir`  **却可以？**

Ans: 实际上是 `—C` 前面的短横线符号非法引起的，修改为 `-C` 即可。因此要特别注意命令行的选项符号前的短横线是英文半角符号 `-`,而非中文全角符号 `—`。这种问题常见于复制粘贴网页上的代码示例。

**5.make 有哪些特殊的命令行选项默认不能赋值给变量** `MAKEFLAGS`,**从而传递给子 make 进程/ 子 Makefile 文件？**

Ans: “-C”、“-f”、“-o” 和 “-W”。

**6.“MAKEFLAGS” 与 “MFLAGS” 的区别？**  

Ans: [see](/command.md#makeflags_diff_mflags).

**7.makefile 中为何要定义空命令？**

Ans: [see](/command.md#empty_command)


**8.GNU make 中特殊变量都有哪些？用途是？**

Ans: [see](/variable.md#spec_var)

**9.GNU make 中自动化变量都有哪些？用途是？**

Ans: [see](/implicit-rule.md#auto_var)

**10.GNU make 中特殊内置目标都有哪些？用途是？**

Ans: [see](/rule.md#special_target)

**11.make 标准的伪目标和空目标有哪些？**

Ans: [see](/make-run.md#phony_target)

**12.多目标模式规则和多目标的普通规则的区别？**

Ans: [see](/implicit-rule.md#multiobj_diff_pattern_vs_usual_rule)

<center><b>(本章更新中)</b></center>

