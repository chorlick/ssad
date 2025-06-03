PROJECT_NAME := ssd
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj

# Compilation flags
CC := g++
CFLAGS := -I$(INC_DIR) -c
LDFLAGS := -L.

# Source and header files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
HDRS := $(wildcard $(INC_DIR)/*.h)

# Object files
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# Target rules
all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp $(HDRS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

# Cleaning
clean:
	rm -f $(OBJS) $(PROJECT_NAME)
	rm -rf $(OBJ_DIR)
	rm -rf /var/log/ssad_activations.log