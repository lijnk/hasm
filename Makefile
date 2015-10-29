CFLAGS = -Wall -Wextra -pedantic -std=c99 -ggdb
OBJECTS = main.o Parser.o SymbolTable.o Code.o Hasm.o

hasm : $(OBJECTS)
	gcc -o hasm $(OBJECTS)

.PHONY : clean
clean :
	rm -rf $(OBJECTS) hasm
