CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic
LDFLAGS = `pkg-config --libs gtk4 gmodule-export-2.0` -L $(LIBMX_DIR) -l:$(LIBMX) -L $(LIB_JSON_DIR) -l:$(LIB_JSON)
INCLUDES = `pkg-config --cflags gtk4` -I $(LIBMX_DIR)/$(INC_DIR) -I $(INC_DIR) -I $(LIB_JSON_DIR)

INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj

INC = $(wildcard $(INC_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c $(SRC_DIR)/**/**/*.c)
OBJ = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:%.c=%.o))

LIBS_DIR = ../libs

LIBMX_DIR = $(LIBS_DIR)/libmx
LIBMX = libmx.a

LIB_JSON_DIR = $(LIBS_DIR)/cJSON
LIB_JSON = libcjson.a

TARGET = uchat

.PHONY: all clean uninstall reinstall

all: $(TARGET)

$(TARGET): $(OBJ_DIR) $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC) $(LIB)
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

uninstall: clean
	rm -rf $(TARGET)

reinstall: uninstall all
