

CCX = clang++
TARGET = compile

SRC_FOLDER = ./src/
OBJ_FOLDER = ./obj/

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))



$(TARGET): $(OBJ)
	$(CCX) $(OBJ) -o $(TARGET) && ./$(TARGET)

%.o: %.c 
	$(CCX) -c $< -o $@
clean:
	rm -rf *.o graph.dot $(TARGET)