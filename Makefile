CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic `pkg-config --cflags gtk4`
LDFLAGS = `pkg-config --libs gtk4`
INCLUDE = -I inc

INC_DIR = inc
SRC_DIR = src
BIN_DIR = obj

TARGET = uchat

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC_DIR)/*.c -o $@ $(LDFLAGS) $(INCLUDE)