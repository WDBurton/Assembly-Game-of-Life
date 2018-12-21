CXX=gcc
CXXFLAGS += -g -Wall -Wextra -pthread

MKDIR_P = mkdir -p
OBJ_DIR = obj

all: main

${OBJ_DIR}:
	${MKDIR_P} ${OBJ_DIR}

obj/%.o: src/%.c ${OBJ_DIR}
	$(CXX) $(CXXFLAGS) -c -o $@ $<


main: obj/main.o
	$(CXX) -o $@ $^

clean:
	rm -f main
