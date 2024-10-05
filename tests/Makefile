# Project-specific variables
PROJECT = test_suite
BUILD_DIR = ../build
SRC_DIR = ../src
INCLUDE_DIR = ../include
TESTS_DIR = .

# Compiler and flags
CC = gcc
CFLAGS = -Wall -I$(INCLUDE_DIR) -I/opt/homebrew/opt/cjson/include -g -O2
LDFLAGS = -L/opt/homebrew/opt/cjson/lib -lcjson -lpthread

# Source files for the agent and server
SRC_A = $(wildcard $(SRC_DIR)/agent/*.c)
SRC_S = $(wildcard $(SRC_DIR)/server/*.c)

# Test source files
TEST_A = $(TESTS_DIR)/test_agent.c
TEST_S = $(TESTS_DIR)/test_server.c

# Object files
OBJ_A = $(patsubst $(SRC_DIR)/agent/%.c,$(BUILD_DIR)/agent/%.o,$(SRC_A))
OBJ_S = $(patsubst $(SRC_DIR)/server/%.c,$(BUILD_DIR)/server/%.o,$(SRC_S))
TEST_OBJ_A = $(patsubst $(TESTS_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_A))
TEST_OBJ_S = $(patsubst $(TESTS_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_S))

# Target executable
TEST_BIN_A = $(BUILD_DIR)/$(PROJECT)_agent
TEST_BIN_S = $(BUILD_DIR)/$(PROJECT)_server

# Default target: build both test suites
all: test-agent test-server

# Compile agent test suite
test-agent: $(TEST_BIN_A)
	@echo "Running Agent Test Suite..."
	$(TEST_BIN_A)

# Compile server test suite
test-server: $(TEST_BIN_S)
	@echo "Running Server Test Suite..."
	$(TEST_BIN_S)

# Link and build the agent test suite
$(TEST_BIN_A): $(OBJ_A) $(TEST_OBJ_A)
	@mkdir -p $(BUILD_DIR)/agent
	$(CC) -o $@ $(OBJ_A) $(TEST_OBJ_A) $(LDFLAGS)
	@echo "Agent Test suite compiled!"

# Link and build the server test suite
$(TEST_BIN_S): $(OBJ_S) $(TEST_OBJ_S)
	@mkdir -p $(BUILD_DIR)/server
	$(CC) -o $@ $(OBJ_S) $(TEST_OBJ_S) $(LDFLAGS)
	@echo "Server Test suite compiled!"

# Compile object files for agent
$(BUILD_DIR)/agent/%.o: $(SRC_DIR)/agent/%.c
	@mkdir -p $(BUILD_DIR)/agent
	$(CC) -c $< -o $@ $(CFLAGS)

# Compile object files for server
$(BUILD_DIR)/server/%.o: $(SRC_DIR)/server/%.c
	@mkdir -p $(BUILD_DIR)/server
	$(CC) -c $< -o $@ $(CFLAGS)

# Compile object files for test files
$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory!"
