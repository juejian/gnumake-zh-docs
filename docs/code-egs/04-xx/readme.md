构建项目的方法：

1. `make gdf -f gd` 生成 .d 文件
2. Linux `make` 执行构建; Win 中执行 `make targetext=.exe` 构建

提供 3 个清除方法：

1. `make cleand` 删除所有 `.d` 文件
2. `make cleano` 删除所有 `.o` 文件
3. `make cleanall`删除 1 和 2 中的文件，并且删除最终生成的可执行文件。

