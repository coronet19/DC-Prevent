# Compiler
CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Iinclude

# Source files
SRCS = src/main.cpp src/Logger.cpp src/HookManager.cpp src/TrayManager.cpp src/GUI.cpp

# Object files go to build/
OBJS = $(patsubst src/%.cpp,build/%.o,$(SRCS))

# Target executable
TARGET = DCPrevent.exe

# Linker flags: user32, shell32, ole32, Unicode Windows GUI
LDFLAGS = -municode -mwindows -static -static-libgcc -static-libstdc++ -luser32 -lshell32 -lole32 -pthread

# Default target
all: build $(TARGET)

# Create build folder
build:
	if not exist build mkdir build

# Build executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source to object files in build/
build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build
clean:
	if exist build rd /s /q build
	if exist $(TARGET) del $(TARGET)

