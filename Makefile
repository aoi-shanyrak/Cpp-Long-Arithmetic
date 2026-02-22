# Makefile for SuperLong Arbitrary Precision Arithmetic Library

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
DEBUG_FLAGS = -g -DDEBUG

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Source files
SRC_FILES = src/superlong-construct.cpp src/superlong-additive.cpp src/superlong-multiplicative.cpp src/superlong-compare.cpp
OBJ_FILES = build/superlong-construct.o build/superlong-additive.o build/superlong-multiplicative.o build/superlong-compare.o

# Test
TEST_SOURCE = $(TEST_DIR)/test_superlong.cpp
TEST_OBJ = $(BUILD_DIR)/test_superlong.o
TEST_EXECUTABLE = $(BUILD_DIR)/test_superlong

# Default target
.PHONY: all test clean help debug

all: test

test: $(TEST_EXECUTABLE)
	@echo "Running tests..."
	@./$(TEST_EXECUTABLE)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TEST_EXECUTABLE): $(OBJ_FILES) $(TEST_OBJ) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(TEST_OBJ) -o $@

$(BUILD_DIR)/superlong-construct.o: $(SRC_DIR)/superlong-construct.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/superlong-additive.o: $(SRC_DIR)/superlong-additive.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/superlong-multiplicative.o: $(SRC_DIR)/superlong-multiplicative.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/superlong-compare.o: $(SRC_DIR)/superlong-compare.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/test_superlong.o: $(TEST_SOURCE) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean test

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

