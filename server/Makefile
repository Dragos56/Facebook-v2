CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -Ilib -std=c++11
TARGET = server
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

OBJ_DIR = build
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: $(TARGET)

# leagă obiectele în executabil
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

# compilează fiecare .cpp în .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all run clean