
#Parameters
OUT_PATH = out
OUT = libcr.a

INC_PATH = inc 

SRC_PATH = src
SRC = $(shell find $(SRC_PATH)/ -type f -name "*.c")

OBJ_PATH = obj
OBJ = $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

CSFLAGS = -Wall -Wextra -Werror

default : all

#Folder Cration
$(OBJ_PATH) :
	mkdir $(OBJ_PATH)
$(OUT_PATH) :
	mkdir $(OUT_PATH)

#Actual compiling and linking
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	cc $(CSFLAGS) -o  $@ -c $< $(addprefix -I, $(addsuffix /, $(INC_PATH)))
$(OUT_PATH)/$(OUT) : $(OUT_PATH) $(OBJ_PATH) $(OBJ)
	ar rc  $(OUT_PATH)/$(OUT)  $(OBJ)
	ranlib $(OUT_PATH)/$(OUT)

#Useful targets
all : $(OUT_PATH)/$(OUT)

clean :
	rm -f $(OBJ)
	rmdir $(OBJ_PATH)

fclean : clean
	rm -f $(OUT_PATH)/$(OUT)
	rmdir $(OUT_PATH)

re : fclean all

#Testing purpose
main : all
	cc main.c -L./out/ -lcr -lm -Iinc

debug :
	cc main.c src/*.c -g3 -L./out/ -lcr -lm -Iinc