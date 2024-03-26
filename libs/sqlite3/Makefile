CC = clang
# CCFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic
CCFLAGS =

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

INC = $(wildcard $(INC_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

NAME = libsqlite3.a

.PHONY: all clean uninstall reinstall

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	ar -rcs $(NAME) $(OBJ)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)
	$(CC) $(CCFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

uninstall: clean
	rm -rf $(NAME)

reinstall: uninstall all