
LIB_PATH = lib
LIB = libcr.a

SRC_PATH = src
SRC = $(shell find src/ -type f -name "*.c")

OBJ_PATH = obj
OBJ = $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

CSFLAGS = -Wall -Wextra -Werror

all : $(LIB)

$(OBJ_PATH) :
	mkdir $(OBJ_PATH)

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	cc $(CSFLAGS) -o  $@ -c $< -Iinc/

$(LIB) : $(OBJ_PATH) $(OBJ)
	ar rc  $(LIB)  $(OBJ)
	ranlib $(LIB)

clean :
	rm -f $(OBJ)
	rmdir $(OBJ_PATH)


fclean : clean
	rm -f $(LIB)

re : fclean all
	