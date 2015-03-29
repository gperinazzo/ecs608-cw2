CC=g++
FLAGS=-Wall -std=c++11
OBJECT= obj/main.o
SRC=src/
PROG_NAME=ecs608-cw2
EXEC=cw2
INCLUDE=include/


all: $(OBJECT)
	$(CC) -o $(EXEC) $(FLAGS)  $(OBJECT) 

obj/%.o: src/%.cpp
	$(CC) -c $(FLAGS) -I $(INCLUDE) $< -o $@

clean:
	rm $(EXEC).exe
	rm $(OBJECT)
