.PHONY: compile

start: tree.o compiler.o
	clang++ tree.o compiler.o -o comp && ./comp

hashtree_table.o: tree.h tree.cpp
	clang++ -c tree.cpp

hash_func.o: compiler.cpp tree.h
	clang++ -c compiler.cpp


clean:
	rm -rf *.o graph.dot comp