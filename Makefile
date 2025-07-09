# Define the compiler as gcc; Using $(CC) later is the same as using gcc.
# The final executable file will be named main and will be output to build/main.
CC = gcc
TARGET = main

# File directory
BUILD_DIR = build
SRC_DIR = sources common serialPort bt/src vendor services/src
INC_DIR = includes common serialPort bt/inc vendor services/inc

include config.mk

# Search for all .c files in the sources directory (non-recursively), and keep only the filenames (remove the paths).
# Similar to the approach above, but used to list .h files. Although it's listed here, the variable isn't actually used later.
SOURCES = $(foreach dir, $(SRC_DIR), $(notdir $(wildcard $(dir)/*.c)))
INCLUDES = $(foreach dir, $(INC_DIR), $(notdir $(wildcard $(dir)/*.h)))


# Replace the *.c filenames in SOURCES with the corresponding build/*.o object filenames. For example: main.c → build/main.o.
# Add a -I option for each include directory to tell gcc where to find header files. For example: includes → -Iincludes.
# Set the search path for source files to sources. This way, even if main.c is not found in the current directory, --
# -- it will be searched for in the sources/ directory.
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SOURCES))
CFLGS = $(patsubst %, -I%, $(INC_DIR))
VPATH = $(SRC_DIR)

#   INC_DIR = includes other misc
#   CFLGS = $(patsubst %, -I%, $(INC_DIR))
#   CFLGS = -Iincludes -Iother -Imisc

$(info MAKE START)

# The target is build/main, which depends on build/*.o.
# $@ represents the target file, which is build/main.
# $^ represents all the dependency files, such as build/main.o, etc.
# Use gcc to link all .o files into an executable, and add -lpthread to link the pthread threading library.
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@$(CC) -o $@ $^ -lpthread -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# This is a pattern rule: compile .c files to generate .o object files.
# $< represents the first prerequisite, which is the .c file.
# $@ represents the target, which is the .o file.
# Uses the previously defined CFLGS to specify the header file directories.
# | create_build is an order-only prerequisite, meaning that create_build will run if the build directory does not exist, --
# -- but changes to it will not trigger recompilation.
$(BUILD_DIR)/%.o: %.c | create_build
	@echo "Compiling $< ..."
	@$(CC) -c $< -o $@ $(CFLGS) $(CFLAGS)

# Define these two targets as “phony targets” (not real files) to avoid conflicts between file names and commands.
.PHONY: clean create_build size memcheck

# Clean all build files by deleting the build directory.
clean:
	rm -r build

# Create the build directory. The @ symbol means the command itself will not be printed.
create_build:
	@mkdir -p $(BUILD_DIR)

size: $(BUILD_DIR)/$(TARGET)
	size $(BUILD_DIR)/$(TARGET)

memcheck: $(BUILD_DIR)/$(TARGET)
	/usr/bin/time -v ./$(BUILD_DIR)/$(TARGET)
