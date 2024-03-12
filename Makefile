CC = gcc
CFLAGS = -I include
SRC_DIR = source
OBJ_DIR = source/bin

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# The main target (executable)
TARGET = $(OBJ_DIR)/http-server

all: $(TARGET)

# Create the bin directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $^ -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)