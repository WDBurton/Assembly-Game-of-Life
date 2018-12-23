CC=gcc
CFLAGS += -g -Wall -Wextra -pthread
CTESTFLAGS += -isystem -src

TESTPACKAGE = `pkg-config --cflags --libs gtk+-3.0`
TESTLIBS = `pkg-config --libs gtk+-3.0`

MKDIR_P = mkdir -p
OBJ_DIR = obj

all: main

${OBJ_DIR}:
	${MKDIR_P} ${OBJ_DIR}

obj/%.o: src/%.c ${OBJ_DIR}
	$(CC) -c -o $@ $< $(CFLAGS) $(TESTPACKAGE)


main: obj/main.o
	$(CC) -o $@ $^ $(CFLAGS) $(TESTPACKAGE)

clean:
	rm -f main
