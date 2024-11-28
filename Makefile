# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

# OpenCV settings using pkg-config
PKG_CONFIG = pkg-config
OPENCV_FLAGS = $(shell pkg-config --cflags --libs opencv4)

# Source files and executable
SRC = main.cpp  # Add other .cpp files here if you have them
OBJ = $(SRC:.cpp=.o)
EXEC = main

# Default target
all: $(EXEC)

# Rule to link object files and create the executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(OPENCV_FLAGS)

# Rule to compile .cpp files to .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< $(OPENCV_FLAGS)

# Clean up the object files and executable
clean:
	rm -f $(OBJ) $(EXEC)

# Rebuild everything from scratch
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild
