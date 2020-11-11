CFLAGS = -Wall -std=c99 -g
HDRS_DIR = include/
OBJ = $(patsubst %.c, %.o, $(wildcard src/*.c *.c))
CC = gcc

.PHONY: all clean run

all: app.exe

app.exe: $(OBJ)
	$(CC) -o $@ -I $(HDRS_DIR) $(CFLAGS) $^

%.o: %.c
	$(CC) -c -o $@ -I $(HDRS_DIR) $(CFLAGS) $< -MD

src/%.o: src/%.c
	$(CC) -c -o $@ -I $(HDRS_DIR) $(CFLAGS) $< -MD

clean:
	rm -f *.exe src/*.o *.o src/*.d *.d

include $(wildcard src/*.d *.d)
