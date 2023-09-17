# Writen by: Oscar Bergström
# https://github.com/OSCARJFB
#	
#	MIT License
#	Copyright (c) 2023 Oscar Bergström

cc = gcc
cflags := -g -Wall -Wextra -pedantic
crflags := -O3 -march=native -mtune=native -flto -fomit-frame-pointer

main: 
	$(cc) main.c old_eEngine.c $(cflags) -o main.o
release: 
	$(cc) main.c old_engine.c $(crflags) -o chess

clean:
	rm -rf *.o
