CC = gcc
CFLAGS = -finput-charset=utf-8 -fexec-charset=utf-8
# CFLAGS = -finput-charset=utf-8 -fexec-charset=utf-8 -g -DDEBUG
TARGET = md_toc.exe

$(TARGET) : export_md_toc.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	del /f /q $(TARGET)

