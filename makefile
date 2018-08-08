# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#	-g    adds debugging information to the executable file
#	-lcurl= Curl lib
#	-lssl = For https
#	-ljson = For the json lib
#	-lpthread = For multithreading
#	-Wall = turn on warnings in gcc
CFLAGS  = -g -lcurl -lssl -ljson -lpthread -Wall -lm

# set of .h files on which the .c files depend
DEPS = aws_lambda.h chipui.h console_colors.h hx711.h initialize.h json_stuff.h lights.h pins.h tests.h the_structs.h logfile.h

# the build target executable:
TARGET = main

FILES = chipui.c aws_lambda.c hx711.c initialize.c json_stuff.c lights.c pins.c tests.c logfile.c

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c $(FILES) $(CFLAGS)

clean:
	$(RM) $(TARGET)