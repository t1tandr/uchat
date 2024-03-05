CC = clang
CCFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -L $(LIBMX_DIR) -l:$(LIBMX) -L $(LIB_JSON_DIR) -l:$(LIB_JSON)
INCLUDES = -I $(LIBMX_DIR)/$(INC_DIR) -I $(INC_DIR) -I $(LIB_JSON_DIR)

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

INC = $(wildcard $(INC_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

LIBS_DIR = ../libs

LIBMX_DIR = $(LIBS_DIR)/libmx
LIBMX = libmx.a

LIB_JSON_DIR = $(LIBS_DIR)/cJSON
LIB_JSON = libcjson.a

NAME = uchat-server

.PHONY: all clean uninstall reinstall

all: $(LIBMX) $(LIB_JSON) $(NAME)

$(LIBMX):
	make -sC $(LIBMX_DIR)

$(LIB_JSON):
	make -sC $(LIB_JSON_DIR)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC) $(LIB)
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -sC $(LIBMX_DIR) clean
	make -sC $(LIB_JSON_DIR) clean

uninstall: clean
	rm -rf $(NAME)
	make -sC $(LIBMX_DIR) uninstall
	make -sC $(LIB_JSON_DIR) clean

reinstall: uninstall all