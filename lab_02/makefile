headers = headers/print_funcs.h headers/read.h headers/struct_utils.h headers/student_struct.headers
params = -Wall -Werror -pedantic -std=c99
objects = read.o struct_utils.o print_funcs.o


.PHONY: all clean 

all: app.exe

clean:
	rm *.o *.exe

app.exe: $(objects) main.o
	gcc -o app.exe $^ $(params) -g

print_funcs.o: functions/print_funcs.c headers/print_funcs.h headers/student_struct.h 
	gcc -c $^ $(params) -g 

struct_utils.o: functions/struct_utils.c headers/struct_utils.h headers/student_struct.h \
headers/print_funcs.h
	gcc -c $^ $(params) -g 

read.o: functions/read.c headers/read.h headers/print_funcs.h headers/student_struct.h \
headers/struct_utils.h
	gcc -c $^ $(params) -g

main.o: main.c headers/read.h headers/print_funcs.h headers/student_struct.h \
headers/struct_utils.h
	gcc -c $^ $(params) -g