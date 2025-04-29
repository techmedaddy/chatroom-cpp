# Makefile for chatroom-cpp project

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pthread -std=c++11

# Directories
SRC_DIR_SERVER = server
SRC_DIR_CLIENT = client
BUILD_DIR = build

# Source files
SERVER_SRC = $(SRC_DIR_SERVER)/server.cpp
CLIENT_SRC = $(SRC_DIR_CLIENT)/client.cpp

# Target binaries
SERVER_BIN = $(BUILD_DIR)/server
CLIENT_BIN = $(BUILD_DIR)/client

# Default target
all: $(SERVER_BIN) $(CLIENT_BIN)

# Build server binary
$(SERVER_BIN): $(SERVER_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Build client binary
$(CLIENT_BIN): $(CLIENT_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean up binaries
clean:
	rm -rf $(BUILD_DIR)

# Run server
run-server: $(SERVER_BIN)
	./$(SERVER_BIN)

# Run client
run-client: $(CLIENT_BIN)
	./$(CLIENT_BIN)

# Phony targets (targets that are not files)
.PHONY: all clean run-server run-client
