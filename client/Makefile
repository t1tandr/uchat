CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic `pkg-config --cflags gtk4`
LDFLAGS = `pkg-config --libs gtk4 gmodule-export-2.0` 
INCLUDES = -I$(INC_DIR)

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:%.c=%.o))

LIB_DIR = libmx
LIB = libmx.a

TARGET = uchat

.PHONY: all clean uninstall reinstall

all: $(LIB) $(TARGET)

$(LIB):
	make -C $(LIB_DIR)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $@

$(OBJ): $(SRC)
	mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $^
	mv *.o $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIB_DIR) clean

uninstall:
	rm -f $(TARGET)
	make -C $(LIB_DIR) uninstall

reinstall: clean uninstall
	make -C $(LIB_DIR) reinstall
	make all
