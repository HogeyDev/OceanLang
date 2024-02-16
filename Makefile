CC := g++
LD := g++
CC_FLAGS := -Werror -Wpedantic -Wextra -std=c++20 -g
LD_FLAGS := 
SRCDIR := src
OBJDIR := build/objects
BUILDDIR := build

CPPSRCS := $(shell find $(SRCDIR)/ -name "*.cpp" -printf "%f\n")
# HEADSRCS := $(shell find ./src/ -name "*.h")
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(CPPSRCS))

all: compile run asm

compile: $(OBJS) link
	@# $(CC) ./src/*.cpp -I./src/include/ $(CC_FLAGS) -o ./build/main

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@printf "CC \t%s > %s\n" $^ $@
	@$(CC) -c $^ -o $@ -I $(SRCDIR)/include/ $(CC_FLAGS)

link: $(OBJS)
	@printf "LD \t%s\n" $(BUILDDIR)/main
	@g++ $(OBJDIR)/*.o -o $(BUILDDIR)/main

run:
	./build/main example/main.ocn

debugGF:
	./gf/gf2 --args ./build/main example/main.ocn

debug:
	gdb --args ./build/main example/main.ocn

compileAsm:
	nasm -g -f elf64 example/main.asm -o example/main.o
	ld -m elf_x86_64 example/main.o -o example/main
	rm example/main.o

runAsm:
	./example/main

asm: compileAsm runAsm

debugAsm:
	gdb --args ./example/main

clean:
	rm $(BUILDDIR)/main
	rm $(OBJDIR)/*.o
