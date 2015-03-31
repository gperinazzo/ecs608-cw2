CXX=g++
FLAGS=-Wall -O3
OBJECT= obj/main.o
SRC=src/
PROG_NAME=ecs608-cw2
EXEC=cw2
INCLUDE=include/


all: $(OBJECT)
	$(CXX) -o $(EXEC) $(FLAGS)  $(OBJECT)

obj/%.o: src/%.cpp
	$(CXX) -c $(FLAGS) -I $(INCLUDE) $< -o $@

clean:
	rm $(EXEC).exe
	rm $(OBJECT)
