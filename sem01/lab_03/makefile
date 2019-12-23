FLAGS = -Wall -Werror -std=c99 -g -MD
FILES = $(wildcard functions/*.c *.c)
HDRS_DIR = headers/
CC = gcc

.PHONY: clean all run

all: app.exe

app.exe: $(subst .c,.o,$(FILES))
	$(CC) -o $@ -I $(HDRS_DIR) $(FLAGS) $^

run: app.exe
	./app.exe

functions/%.o: functions/%.c
	$(CC) -c -o $@ -I $(HDRS_DIR) $(FLAGS) $<

%.o: %.c
	$(CC) -c -o $@ -I headers/ $(FLAGS) $<

clean:
	rm -f *.exe *.d *.o functions/*.o functions/*.d

include $(wildcard functions/*.d *.d)
