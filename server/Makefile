CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = server

SRC_DIR = src
BUILD_DIR = build

# toate fișierele .cpp
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# compilează fiecare .cpp în .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Cleaned."

run: all
	./$(TARGET)

.PHONY: all clean run