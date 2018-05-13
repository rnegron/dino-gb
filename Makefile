ROM_NAME = dinoboy.gb

TOOLS_DIR = /Users/rauln/code/gb_dev

# Directories
GBDK_DIR = $(TOOLS_DIR)/gbdk
SRC_DIR = ./src
DIST_DIR = ./bin
OBJ_DIR = ./build

# Binaries
CC	= $(GBDK_DIR)/bin/lcc
EMULATOR = $(TOOLS_DIR)/bgb/bgb.exe

# Files
ROM_FILE = $(DIST_DIR)/$(ROM_NAME)
OBJ_FILES = $(OBJ_DIR)/main.o

all: $(ROM_FILE)

$(ROM_FILE): $(OBJ_FILES)
	@mkdir -p $(DIST_DIR)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $<

.PHONY : run
run: $(ROM_FILE)
	wine $(EMULATOR) $<

.PHONY : clean
clean:
	@rm -rf $(DIST_DIR)
	@rm -rf $(OBJ_DIR)
