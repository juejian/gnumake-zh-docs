a = f1.c
b := f2
CC = gcc

# .SECONDARY: main.o

main : main.o
	$(LINK.o) $(LDFLAGS) main.o $(LOADLIBES) $(LDLIBS)

.PHONY : all cleanall

all:
	@echo $(flavor a),$(flavor b),$(flavor c)

cleanall:
	del *.exe *.o