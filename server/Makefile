CC = clang
CCFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

INC = $(wildcard $(INC_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

LIB_DIR = libmx
LIB = $(LIB_DIR)/libmx.a

LIB_JSON_DIR = cJSON
LIB_JSON = $(LIB_JSON_DIR)/libcjson.a

LIB_SQLITE_DIR = sqlite3
LIB_SQLITE = $(LIB_SQLITE_DIR)/sqlite3.a

LIB_BCRYPT_DIR = libbcrypt
LIB_BCRYPT = $(LIB_BCRYPT_DIR)/bcrypt.a

NAME = uchat-server

.PHONY: all clean uninstall reinstall

all: $(LIB) $(LIB_JSON) $(LIB_SQLITE) $(LIB_BCRYPT) $(NAME)

$(LIB):
	make -sC $(LIB_DIR)

$(LIB_JSON):
	make -sC $(LIB_JSON_DIR)
	make -sC $(LIB_JSON_DIR) install

$(LIB_SQLITE):
	make -sC $(LIB_SQLITE_DIR)

$(LIB_BCRYPT):
	make -sC $(LIB_BCRYPT_DIR)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -L $(LIB_DIR) -lmx -L $(LIB_JSON_DIR) -lcjson -L $(LIB_SQLITE_DIR) -lsqlite3 -L $(LIB_BCRYPT_DIR) -lbcrypt -lpthread -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC) $(LIB)
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) -I $(LIB_DIR)/$(INC_DIR) -I $(INC_DIR) -I $(LIB_JSON_DIR) -I $(LIB_SQLITE_DIR)/inc -I $(LIB_BCRYPT_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -sC $(LIB_DIR) clean
	make -sC $(LIB_JSON_DIR) clean
	make -sC $(LIB_SQLITE_DIR) clean
	make -sC $(LIB_BCRYPT_DIR) clean

uninstall: clean
	rm -rf $(NAME)
	make -sC $(LIB_DIR) uninstall
	make -sC $(LIB_JSON_DIR) clean
	make -sC $(LIB_SQLITE_DIR) uninstall
	make -sC $(LIB_BCRYPT_DIR) clean

reinstall: uninstall all