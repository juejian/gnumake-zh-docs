# main makefile
include common-mf

TARGET = bin/main$(targetext)
objects = 

$(TARGET): $(objects)

include $(OD)

.PHONY: cleanall cleano cleand

cleanall: cleano cleand
	-rm -f $(TARGET)

cleano:
	-rm -f bin/*.o

cleand:
	-find . -name "*.d"  | xargs rm -f
	-find . -name "*.d.????"  | xargs rm -f
