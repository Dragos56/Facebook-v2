CXX = g++
CXXFLAGS = -std=c++17 -Iimgui -Iimgui/backends -Wall -O2
LDFLAGS = -lglfw -lGL -ldl -lpthread
TARGET = app

SRC_DIR = src
BUILD_DIR = build

CLIENT_SRC := $(wildcard $(SRC_DIR)/*.cpp)
IMGUI_SRC := imgui/imgui.cpp \
             imgui/imgui_draw.cpp \
             imgui/imgui_widgets.cpp \
             imgui/imgui_tables.cpp \
             imgui/imgui_demo.cpp \
             imgui/backends/imgui_impl_glfw.cpp \
             imgui/backends/imgui_impl_opengl3.cpp

ALL_SRC := $(CLIENT_SRC) $(IMGUI_SRC)
OBJ_FILES := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(ALL_SRC))

all: $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Cleaned."

run: all
	./$(TARGET)

.PHONY: all clean run