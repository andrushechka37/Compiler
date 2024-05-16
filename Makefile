TARGET = compiler
CC = g++

OBJ = ./obj/
SRC = ./src/
INCLUDE = ./include/

.PHONY: compile

start: $(OBJ)tree.o $(OBJ)compiler.o $(OBJ)read_to_tree.o
	$(CC) $(OBJ)tree.o $(OBJ)compiler.o $(OBJ)read_to_tree.o -o $(TARGET) && ./$(TARGET)

$(OBJ)tree.o: $(INCLUDE)tree.h $(SRC)tree.cpp
	$(CC) -g -c $(SRC)tree.cpp -o $(OBJ)tree.o

$(OBJ)compiler.o: $(SRC)compiler.cpp $(INCLUDE)tree.h
	$(CC) -g -c $(SRC)compiler.cpp -o $(OBJ)compiler.o

$(OBJ)read_to_tree.o: $(SRC)read_to_tree.cpp $(INCLUDE)read_to_tree.h
	$(CC) -g -c $(SRC)read_to_tree.cpp -o $(OBJ)read_to_tree.o

clean:
	rm -rf $(OBJ)*.o graph.dot $(TARGET) asm.txt IR_log.txt


# // -x - синтаксис языка
# // -m - куда кодоген !!!!!!!!!!!!!!
# // -S - текст асма
# nop 