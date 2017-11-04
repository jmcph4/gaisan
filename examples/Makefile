SRC_DIR = .
BUILD_DIR = ../build

INC_DIR = ../src
LIB_DIR = ../build

CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -pedantic -std=c11 -g -O3 -I$(INC_DIR)\
	-L$(LIB_DIR) -lgaisan -lm -march=native

.PHONY: all
all: $(BUILD_DIR)/ex_horner $(BUILD_DIR)/ex_bisect $(BUILD_DIR)/ex_euler $(BUILD_DIR)/ex_monte_carlo $(BUILD_DIR)/ex_gauss

$(BUILD_DIR)/ex_horner: $(SRC_DIR)/ex_horner.c
	$(CC) $^ $(CFLAGS) -o $@

$(BUILD_DIR)/ex_bisect: $(SRC_DIR)/ex_bisect.c
	$(CC) $^ $(CFLAGS) -o $@

$(BUILD_DIR)/ex_euler: $(SRC_DIR)/ex_euler.c
	$(CC) $^ $(CFLAGS) -o $@

$(BUILD_DIR)/ex_monte_carlo: $(SRC_DIR)/ex_monte_carlo.c
	$(CC) $^ $(CFLAGS) -o $@

$(BUILD_DIR)/ex_gauss: $(SRC_DIR)/ex_gauss.c
	$(CC) $^ $(CFLAGS) -o $@
