# Writen by: Oscar Bergström
# https://github.com/OSCARJFB

# Compile code.
main: chess.c  
	cc chess.c -o chess

# Remove all object files.
clean:
	rm -rf chess