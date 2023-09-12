# Writen by: Oscar Bergström
# https://github.com/OSCARJFB
#	
#	MIT License
#	Copyright (c) 2023 Oscar Bergström

# Compile code.

cc = gcc
raylib := -lraylib -lm
cflags := -g -Wall -Wextra -pedantic
crflags := -O3 -march=native -mtune=native -flto -fomit-frame-pointer

main: 
	$(cc) main.c chess_engine.c chess_graphics.c $(raylib) $(cflags) -o main.o
release: 
	$(cc) main.c chess_engine.c chess_graphics.c $(raylib) $(crflags) -o chess

# Remove all object files.
clean:
	rm -rf *.o
