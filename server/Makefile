CC = clang
CCFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

INC = $(wildcard $(INC_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

LIB_DIR = libmx
LIB = $(LIB_DIR)/libmx.a

NAME = uchat-server

.PHONY: all clean uninstall reinstall

all: $(LIB) $(NAME)

$(LIB):
	make -sC $(LIB_DIR)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -L $(LIB_DIR) -lmx -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC) $(LIB)
	$(CC) $(CCFLAGS) -I $(LIB_DIR)/$(INC_DIR) -I $(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -sC $(LIB_DIR) clean

uninstall: clean
	rm -rf $(NAME)
	make -sC $(LIB_DIR) uninstall

reinstall: uninstall $(NAME)