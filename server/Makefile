CXX = g++
CC  = gcc

SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = build
TARGET  = server

INC_FLAGS = -I$(SRC_DIR) -I$(LIB_DIR)

CXXFLAGS = -Wall -Wextra -O2 -std=c++11 $(INC_FLAGS)

CFLAGS   = -O2 $(INC_FLAGS)

LDFLAGS  = -lpthread -ldl

SRCS_CPP = $(wildcard $(SRC_DIR)/*.cpp)

SRCS_C   = $(wildcard $(LIB_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_CPP)) \
       $(patsubst $(LIB_DIR)/%.c,   $(OBJ_DIR)/%.o, $(SRCS_C))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all run clean