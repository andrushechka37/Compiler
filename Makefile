

CCX = clang++
TARGET = compile

SRC_FOLDER = ./src/
OBJ_FOLDER = ./obj/

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))



$(TARGET): $(OBJ)
	$(CCX) $(OBJ) -v -o $(TARGET) && ./$(TARGET)

%.o: %.c %.h
	$(CCX) -v -c $< -o $@
clean:
	rm -rf *.o graph.dot $(TARGET) asm.txt