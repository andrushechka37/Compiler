
.PHONY: compile

start: tree.o compiler.o read_to_tree.o
	g++ tree.o compiler.o read_to_tree.o -o compiler && ./compiler

tree.o: tree.h tree.cpp
	g++ -g -c tree.cpp

compiler.o: compiler.cpp tree.h
	g++ -g -c compiler.cpp

read_to_tree.o: read_to_tree.cpp read_to_tree.h
	g++ -g -c read_to_tree.cpp

clean:
	rm -rf *.o graph.dot compiler asm.txt


# // -x - синтаксис языка
# // -m - куда кодоген
# // -S - текст асма