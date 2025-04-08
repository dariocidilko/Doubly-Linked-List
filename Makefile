
# These are all of the commands for running the program.
CC = gcc
EXE = wordfrequency

# These are all of the directories that will be created when you make the program.
MAIN_DIR = main
OBJ_DIR = objects
INCLUDE = include

# These are the source and object files.
MAIN := $(wildcard $(MAIN_DIR)/*.c) # Main directory.
HEADERS := $(wildcard $(INCLUDE)/*.h) # Include directory.
OBJ := $(patsubst $(MAIN_DIR)/%.c,$(OBJ_DIR)/%.o,$(MAIN)) # Object directory.

# If the debug variable is equal to 0, run the release version. ('bin/release')
# However, if the debug variable is NOT equal to 0, run the debug version. ('bin/debug')
ifeq ($(DEBUG), 0)
CFLAGS += -O3 -DNDEBUG
BUILD_DIR := bin/release
TARGET := $(BUILD_DIR)/$(EXE)
else
CFLAGS += -Og -DDEBUG -D_GNU_SOURCE -g -Wall -Wextra
BUILD_DIR := bin/debug
TARGET := $(BUILD_DIR)/$(EXE)
endif

# Declare phony targets. (These are not real files to be built.)
.PHONY: all exec
.PHONY: clean distclean
.PHONY: dirs

# Everything depends on the 'dirs' and 'exec'.
# Create all the dictionaries and then run the executables.
all: dirs exec
exec: $(TARGET)

# This will link all of the object files into the final executable.
$(TARGET): $(OBJ) $(HEADERS) Makefile
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# This is a 'pattern rule' to build '.o' files from the corresponding '.c' files.
$(OBJ_DIR)/%.o: $(MAIN_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# These are the directories that are created when the program is executed.
# Either 'objects' or ('bin/debug/' or 'bin/release/').
dirs:
	mkdir -p $(OBJ_DIR) 
	mkdir -p $(BUILD_DIR)

# This is the 'make clean' command.
clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf bin/debug
	rm -rf bin/release

# This is the 'make distclean' command.
distclean: clean
	rm -rf $(OBJ_DIR)
	rm -rf .DS_Store
	rm -rf bin/