CC = clang
CCFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -L $(LIBMX_DIR) -l:$(LIBMX) -L $(LIB_JSON_DIR) -l:$(LIB_JSON) -L $(LIB_SQLITE_DIR) -lsqlite3 -L $(LIB_BCRYPT_DIR) -l:bcrypt.a -lpthread
INCLUDES = -I $(LIBMX_DIR)/$(INC_DIR) -I $(INC_DIR) -I $(LIB_JSON_DIR) -I $(LIB_SQLITE_DIR)/inc -I $(LIB_BCRYPT_DIR)

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

INC = $(wildcard $(INC_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

LIBS_DIR = ../libs

LIBMX_DIR = $(LIBS_DIR)/libmx
LIBMX = libmx.a

LIB_JSON_DIR = $(LIBS_DIR)/cJSON
LIB_JSON = libcjson.a

LIB_SQLITE_DIR = $(LIBS_DIR)/sqlite3
LIB_SQLITE = sqlite3.a

LIB_BCRYPT_DIR = $(LIBS_DIR)/libbcrypt
LIB_BCRYPT = bcrypt.a

NAME = uchat-server

.PHONY: all clean uninstall reinstall

all: $(LIBMX) $(LIB_JSON) $(LIB_SQLITE) $(LIB_BCRYPT) $(NAME)

$(LIBMX):
	make -sC $(LIBMX_DIR)

$(LIB_JSON):
	make -sC $(LIB_JSON_DIR)

$(LIB_SQLITE):
	make -sC $(LIB_SQLITE_DIR)

$(LIB_BCRYPT):
	make -sC $(LIB_BCRYPT_DIR)

$(NAME): $(OBJ_DIR) $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC) $(LIB)
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -sC $(LIBMX_DIR) clean
	make -sC $(LIB_JSON_DIR) clean
	make -sC $(LIB_SQLITE_DIR) clean
	make -sC $(LIB_BCRYPT_DIR) clean

uninstall: clean
	rm -rf $(NAME)
	make -sC $(LIBMX_DIR) uninstall
	make -sC $(LIB_JSON_DIR) clean
	make -sC $(LIB_SQLITE_DIR) uninstall
	make -sC $(LIB_BCRYPT_DIR) clean

reinstall: uninstall all