PROJ_NAME = gaisan

SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
DOCS_DIR = docs
INC_DIR = include
LIB_DIR = lib

CC = gcc
REL_CFLAGS = -Wall -Wextra -Wshadow -pedantic -std=c11 -Ofast -lm -march=native
DBG_CFLAGS = -Wall -Wextra -Wshadow -pedantic -std=c11 -g3 -lm -march=native

$(BUILD_DIR)/$(PROJ_NAME).a:
	$(CC) -c $(SRC_DIR)/*.c $(REL_CFLAGS)
	mv *.o $(BUILD_DIR)
	ar -cvq $(BUILD_DIR)/lib$(PROJ_NAME).a $(BUILD_DIR)/*.o
	ar -t $(BUILD_DIR)/lib$(PROJ_NAME).a
	rm $(BUILD_DIR)/*.o

.PHONY: debug
debug:
	$(CC) -c $(SRC_DIR)/*.c $(DBG_CFLAGS)
	mv *.o $(BUILD_DIR)
	ar -cvq $(BUILD_DIR)/lib$(PROJ_NAME).a $(BUILD_DIR)/*.o
	ar -t $(BUILD_DIR)/lib$(PROJ_NAME).a
	rm $(BUILD_DIR)/*.o

.PHONY: tests
tests:
	cp $(BUILD_DIR)/lib$(PROJ_NAME).a $(LIB_DIR)/lib$(PROJ_NAME).a; cd $(TEST_DIR); make

.PHONY: examples
examples:
	cd examples; make

.PHONY: docs
docs:
	cd $(SRC_DIR);\
	doxygen;\
	cd ..;

.PHONY: clean
clean:
	rm $(BUILD_DIR)/* -rf
	rm $(DOCS_DIR)/* -rf
