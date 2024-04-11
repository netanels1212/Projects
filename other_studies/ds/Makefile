# Test file as input from terminal
ifdef T
    TEST_ARG = $(MAIN_DIR)/$(T)
else
    TEST_ARG = $(MAIN_DIR)/priority_test.c
endif

# Compiler and flags
CC = gcc
CFLAGS = -ansi -fPIC -pedantic-errors -Wall -Wextra -I$(HEADER_DIR) -g
IMLICIT_FLAG = -shared -o 

# Source files and the output executable
SRC_DIR = src
OBJ_DIR = obj
MAIN_DIR = test
HEADER_DIR = include	
BIN_DIR = bin

# Default target (build the final executable and the library)
all: sharedlibrary exe

# Get a list of all C source files in the current directory
SOURCES := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(HEADER_DIR)/*.h)
MAINS := $(TEST_ARG)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
TEST_OBJECTS = $(patsubst $(MAIN_DIR)/%.c,$(OBJ_DIR)/%.o,$(MAINS))
SHARED_LIBRARY = sharedlibrary.so
EXE = $(BIN_DIR)/$(basename $(T))


# Rule to create shared library and executable file
$(SHARED_LIBRARY): $(OBJECTS) 
	$(CC) $(IMLICIT_FLAG) $@ $^	

sharedlibrary: $(SHARED_LIBRARY)

exe: $(EXE) 

$(EXE): $(TEST_OBJECTS) $(SHARED_LIBRARY)
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $^ -o $@
	@mkdir -p $(BIN_DIR)
	@mv $(SHARED_LIBRARY) $(BIN_DIR)/

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile test source files into object files
$(OBJ_DIR)/%.o: $(MAIN_DIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the object files and executable
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(SHARED_LIBRARY)

