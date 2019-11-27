CFLAGS = -Wall -Werror -std=c99 -g
HDRS_DIR = headers/
OBJ = $(patsubst %.c, %.o, $(wildcard functions/*.c *.c))
CC = gcc

.PHONY: all clean run

all: app.exe

app.exe: $(OBJ)
	$(CC) -o $@ -I $(HDRS_DIR) $(CFLAGS) $^

%.o: %.c
	$(CC) -c -o $@ -I $(HDRS_DIR) $(CFLAGS) $< -MD

functions/%.o: functions/%.c
	$(CC) -c -o $@ -I $(HDRS_DIR) $(CFLAGS) $< -MD

clean:
	rm -f *.exe functions/*.o *.o functions/*.d *.d

include $(wildcard functions/*.d *.d)
