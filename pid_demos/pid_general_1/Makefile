# src
SRC 			= ./*.c
# Program name
NAME 			= pid_demo
# C flags
CFLAGS			= -std=c99 -Wall

# Build executable
$(NAME): $(SRC)
	gcc $(CFLAGS) -o $(NAME) $(SRC)

# Build executable with debug information
debug: $(SRC)
	gcc $(CFLAGS) -o $(NAME)d $(SRC) -g

# Run valgrind over debug executable
vgtest: $(NAME)d
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=20 --track-fds=yes ./$(NAME)d

# Delete executables
clean:
	rm $(NAME) $(NAME)d