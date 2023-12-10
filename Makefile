CC := g++
CCARGS := -Wall -Werror -Wpedantic -std=c++20

.PHONY: clean
all: clean compile run

compile:
	$(CC) src/*.cpp -o build/main -I./src/include $(CCARGS)

run:
	./build/main example/main.ocn

bear:
	bear -- make

clean:
	rm -rf build
	mkdir build
