# Determine the platform
PLAT_NAME := $(shell uname -s)

#Compiler
ifeq ($(PLAT_NAME), Darwin)
	CC=clang++ -arch x86_64
else
	CC=g++
endif

# Project structure
CLIENT_DIR := client
SERVER_DIR := server

SRC_DIR 	:= $(SERVER_DIR)/src
BUILD_TARGET := $(SERVER_DIR)/build
EXEC_TARGET	:= $(SERVER_DIR)/bin

# Executable 
EXEC_TARGET_NAME := udp_sockets
EXEC_FULL_PATH := $(EXEC_TARGET)/$(EXEC_TARGET_NAME)

# Code properties
SRC_EXT		:= cpp
# Find all the .cpp 
SRC_FULL_PATH_LIST	:= $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
# Based on SRC_FULL_PATH_LIST create the list of *.o files 
O_FILE_FULL_PATH_LIST:= $(patsubst $(SRC_DIR)/%,$(BUILD_TARGET)/%,$(SRC_FULL_PATH_LIST:.$(SRC_EXT)=.o))
# Based on O_FILE_FULL_PATH_LIST remove the *.o file (just leave the directory structure)
BUILD_DIRECTORY_LIST:=$(foreach O_FILE,$(O_FILE_FULL_PATH_LIST),$(subst $(notdir $(O_FILE)),,$(O_FILE)))
# Remove the duplicates 
BUILD_DIRECTORY_LIST := $(sort $(BUILD_DIRECTORY_LIST))
#BUILD_TEST 		:= $(notdir $(BUILD_LIST))
#BUILD_TEST2:=$(patsubst %/$(BUILD_TEST),.a,$(BUILD_LIST))


# Find all the directories containing .hpp and .h files
HEADER_DIRECTORY_LIST	:= $(shell find $(SERVER_DIR)/include \( -name '*.hpp' -o -name '*.h' \) -exec dirname {} \; | sort | uniq)
# Prefix -I to all directories containing .hpp and .h files
INCLUDE_LIST 	:= $(patsubst $(SERVER_DIR)/include%,-I $(SERVER_DIR)/include%,$(HEADER_DIRECTORY_LIST))


CFLAGS=-c -std=c++1z -Wall -Werror -g
CLIENT_FLAGS= -std=c++1z -Wall -Werror -g

$(EXEC_TARGET): $(O_FILE_FULL_PATH_LIST)
	@mkdir -p $(EXEC_TARGET)
	@echo "Linking..."
	@echo "		$(CC) $^ -o $(EXEC_FULL_PATH)"; $(CC) $^ -o $(EXEC_FULL_PATH);
	@echo "Build complete, executing..."
	@echo ""
	./$(EXEC_FULL_PATH)

$(BUILD_TARGET)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(BUILD_DIRECTORY_LIST)
	@echo "Compiling $<..."
	@echo "$(CC) $(CFLAGS) $(INCLUDE_LIST) $< -o $@"; $(CC) $(CFLAGS) $(INCLUDE_LIST) $< -o $@

run:
	./$(EXEC_FULL_PATH)

client:
	@echo "Cleaning $(CLIENT_DIR)/client ...";
	$(RM) $(CLIENT_DIR)/client
	@echo "$(CC) $(CLIENT_FLAGS) -o $(CLIENT_DIR)/client $(CLIENT_DIR)/client.cpp";
	$(CC) $(CLIENT_FLAGS) -o $(CLIENT_DIR)/client $(CLIENT_DIR)/client.cpp 
	./$(CLIENT_DIR)/client "127.0.0.1" "8080" "Hello kitty"
	
debug:
	@echo "SRC_FULL_PATH_LIST $(SRC_FULL_PATH_LIST)"
	@echo "O_FILE_FULL_PATH_LIST $(O_FILE_FULL_PATH_LIST)"
	@echo "BUILD_DIRECTORY_LIST $(BUILD_DIRECTORY_LIST)"
	@echo ""
	@echo "HEADER_DIRECTORY_LIST $(HEADER_DIRECTORY_LIST)"
	@echo "INCLUDE_LIST $(INCLUDE_LIST)"

clean:
	@echo "Cleaning $(EXEC_FULL_PATH) ..."; $(RM) -r $(EXEC_FULL_PATH)

hardclean:
	@echo "Cleaning $(EXEC_FULL_PATH) and $(BUILD_TARGET)/* ..."; $(RM) -r $(BUILD_TARGET)/* $(EXEC_FULL_PATH)

.PHONY: clean	client
