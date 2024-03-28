# make build script.
#
# usage:
#    make: clean, compile and run
#    make clean: clean compiled file
#    make cleanAndCompile: clean compiled file and compile the project
#    make compile: compile the project
#    make compileAndRun: compile the project and run the compiled file
#    make run: run the compiled file
#
# author: Prof. Dr. David Buzatto

currentFolderName := $(lastword $(notdir $(shell pwd)))
compiledFile := $(currentFolderName)
CFLAGS := -O1 -Wall -Wextra -Wno-unused-parameter -pedantic-errors -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
CFLAGS_LINUX := -O1  -Wall -Wextra -Wno-unused-parameter -pedantic-errors -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp
.PHONY: clean

all: clean compile run

clean:
	rm -f $(compiledFile)

compile:
	if [ "$(target)" = "windows" ]; then \
		gcc *.c -o $(compiledFile) $(CFLAGS);\
	else \
		gcc *.c -g -o $(compiledFile) $(CFLAGS_LINUX);\
	fi
run:
	if [ "$(target)" = "windows" ]; then \
		./$(compiledFile).exe;\
	else \
		./$(compiledFile);\
	fi
		
	rm -f $(compiledFile)

cleanAndCompile: clean compile
compileAndRun: compile run
