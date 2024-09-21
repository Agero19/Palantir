# Project Name
PROJECT = resource_monitor

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
LOG_DIR = logs
TEST_DIR = tests

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -I$(INCLUDE_DIR) -g -O2    # Use optimization flag `-O2` and debug flag `-g`
LDFLAGS = -lpthread -lsqlite3             # Link to libraries like pthread and SQLite3

# Source Files (Agent, Server, etc.)
SRC = $(wildcard $(SRC_DIR)/agent/*.c $(SRC_DIR)/server/*.c $(SRC_DIR)/network/*.c $(SRC_DIR)/logger/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Test Source Files
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_BIN = $(BUILD_DIR)/test_suite

# Targets
TARGET = $(BUILD_DIR)/$(PROJECT)

# Default Target: Build all
all: $(TARGET)

# Build the project
$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)
	@echo "Build completed!"

# Compile all source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program (can specify agent or server mode)
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Test: Run unit tests
test: $(TEST_BIN)
	./$(TEST_BIN)

# Build the test suite
$(TEST_BIN): $(OBJ) $(TEST_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $(TEST_BIN) $(OBJ) $(TEST_OBJ) $(LDFLAGS)
	@echo "Test suite compiled!"

# Clean: Remove build artifacts
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory!"

# Clean logs
clean-logs:
	rm -rf $(LOG_DIR)/*.log
	@echo "Cleaned log files!"

# Dependencies
$(OBJ): $(INCLUDE_DIR)/*.h

.PHONY: all clean clean-logs run test
