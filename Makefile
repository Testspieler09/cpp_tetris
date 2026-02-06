# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude -Iexternal/raylib/src
DEPFLAGS := -MMD -MP
LDFLAGS := -Lexternal/raylib/src -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

# Directories
SRC_DIR := src
BUILD_DIR := build
RAYLIB_DIR := external/raylib/src

# Output binary
TARGET := $(BUILD_DIR)/tetris

# Find all source files recursively
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Raylib library
RAYLIB := $(RAYLIB_DIR)/libraylib.a

# Rule to build the final output
all: $(RAYLIB) $(TARGET)

# Build raylib
$(RAYLIB):
	@echo "Building raylib..."
	@cd $(RAYLIB_DIR) && $(MAKE) PLATFORM=PLATFORM_DESKTOP

# Linking the final executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile each source file to an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# Include dependency files, if they exist
-include $(DEPS)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up the build directory and the output binary
clean:
	rm -rf $(BUILD_DIR)

# Clean everything including raylib
cleanall: clean
	cd $(RAYLIB_DIR) && $(MAKE) clean

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean cleanall run
