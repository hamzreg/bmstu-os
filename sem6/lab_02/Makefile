CC := gcc
CFLAGS := -std=c99 -Wall -pedantic

TARGET := app.exe

SRC_DIR := ./src
CFILES := $(wildcard $(SRC_DIR)/*.c)

HDRS_DIR := ./inc
HDRS := $(wildcard $(HDRS_DIR)/*.h)

OBJ_DIR := ./out
OBJS := $(CFILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: clean


$(TARGET): $(OBJS)
	$(CC) $^ -o $@

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(UFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(OBJ_DIR) *.exe