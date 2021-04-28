CC = gcc
CFLAGS = -finput-charset=utf-8 -fexec-charset=utf-8
# CFLAGS = -finput-charset=utf-8 -fexec-charset=utf-8 -g -DDEBUG
TARGET = gnp.exe

$(TARGET) : g_n_p.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	del /f /q $(TARGET)
