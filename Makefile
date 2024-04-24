SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

LIBS_DIR = ./libs

LIBMX_DIR = $(LIBS_DIR)/libmx
LIBMX = libmx.a

LIB_JSON_DIR = $(LIBS_DIR)/cJSON
LIB_JSON = libcjson.a

LIB_SQLITE_DIR = $(LIBS_DIR)/sqlite3
LIB_SQLITE = sqlite3.a

LIB_BCRYPT_DIR = $(LIBS_DIR)/libbcrypt
LIB_BCRYPT = bcrypt.a

SERVER_DIR = server
CLIENT_DIR = client

SERVER = uchat-server
CLIENT = uchat

.PHONY: all clean uninstall reinstall

all: $(LIBMX) $(LIB_JSON) $(LIB_SQLITE) $(LIB_BCRYPT) $(SERVER) $(CLIENT)

$(LIBMX):
	make -sC $(LIBMX_DIR)

$(LIB_JSON):
	make -sC $(LIB_JSON_DIR)

$(LIB_SQLITE):
	make -sC $(LIB_SQLITE_DIR)

$(LIB_BCRYPT):
	make -sC $(LIB_BCRYPT_DIR)

$(SERVER):
	make -sC $(SERVER_DIR)

$(CLIENT):
	make -sC $(CLIENT_DIR)

clean:
	make -sC $(LIBMX_DIR) clean
	make -sC $(LIB_JSON_DIR) clean
	make -sC $(LIB_SQLITE_DIR) clean
	make -sC $(LIB_BCRYPT_DIR) clean
	make -sC $(SERVER_DIR) clean
	make -sC $(CLIENT_DIR) clean

uninstall: clean
	make -sC $(LIBMX_DIR) uninstall
	make -sC $(LIB_JSON_DIR) clean
	make -sC $(LIB_SQLITE_DIR) uninstall
	make -sC $(LIB_BCRYPT_DIR) clean
	make -sC $(SERVER_DIR) uninstall
	make -sC $(CLIENT_DIR) uninstall

reinstall: uninstall all
