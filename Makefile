CC := g++
CC_FLAGS := -Werror -Wall -Wpedantic -Wextra -std=c++20 -g

all: compile run

compile:
	${CC} ./src/*.cpp -I./src/include/ ${CC_FLAGS} -o ./build/main

run:
	./build/main example/main.ocn

debug:
	gdb --args ./build/main example/main.ocn

compileAsm:
	nasm -g -f elf64 example/main.asm -o example/main.o
	ld -m elf_x86_64 example/main.o -o example/main
	rm example/main.o

runAsm:
	./example/main

asm: compileAsm runAsm
