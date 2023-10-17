
LIB = libcr.a
SRC = t_window.c
OBJ_DIR = obj
OBJ = $(SRC:.c=.o)
CSFLAGS = -Wall -Wextra -Werror

all : $(LIB)

$(OBJ_DIR) :
	mkdir $(OBJ_DIR)

%.o : %.c $(OBJ_DIR)
	cc $(CSFLAGS) -o $(OBJ_DIR)/$@ -c $<

$(LIB) : $(OBJ)
	ar rc $(LIB) $(OBJ_DIR)/$(OBJ)
	ranlib $(LIB)

clean :
	rm -f $(OBJ_DIR)/*
	rmdir $(OBJ_DIR)


fclean : clean
	rm -f $(LIB)
	rm main.o
	rm maketest

re : fclean all

test : all
	cc main.c -c
	cc main.o -lm -L. -lcr -o maketest
	./maketest 32 16
	rm maketest
	