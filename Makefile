# Makefile variables
LIB_DIR = lib
INCLUDE_DIR = include
COMPILER = g++
OUTPUT_DIR = bin
OBJ_DIR = bin/obj
SRC_DIR = src
OUTPUT_NAME = mcsm

# List of source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
EXECUTABLE = $(OUTPUT_DIR)/$(OUTPUT_NAME).exe

# Compiler flags and include directories
CXXFLAGS = -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR)

# Default target
all: $(EXECUTABLE)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(COMPILER) $(CXXFLAGS) -c $< --std=c++17 -O3 -o $@

# Link object files to create the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(COMPILER) $(LDFLAGS) $^ -o $@ -lcurl

# Clean the compiled files
clean:
	-@del /q $(subst /,\,$(OBJ_FILES)) 2> nul

.PHONY: all clean
