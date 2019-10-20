FLAGS = -Wall -Werror -std=c99 -pedantic -g
FILES = $(wildcard *.c functions/*.c)
HDRS_DIR = ../headers/
CC = gcc

.PHONY: clean all run

all: app.exe

app.exe: $(subst .c,.o,$(FILES))
	$(CC) -o $@ -I $(HDRS_DIR) $(FLAGS) $^

run: app.exe
	./app.exe

%.o: %.c
	$(CC) -o $@ -I $(HDRS_DIR) $(FLAGS) $@

functions/%.o: functions/%.c
	$(CC) -o $@ -I $(HDRS_DIR) $(FLAGS) $@

clean:
	rm -rf *.exe *.d *.o

include $(wildcard functions/*.d *.d)