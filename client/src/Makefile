CXX = g++
CXXFLAGS = -Iimgui -Iimgui/backends -lglfw -lGL -ldl -lpthread

IMGUI_SRC = imgui/imgui.cpp \
            imgui/imgui_draw.cpp \
            imgui/imgui_widgets.cpp \
            imgui/imgui_tables.cpp \
            imgui/imgui_demo.cpp \
            imgui/backends/imgui_impl_glfw.cpp \
            imgui/backends/imgui_impl_opengl3.cpp

main: main.cpp $(IMGUI_SRC)
	$(CXX) main.cpp $(IMGUI_SRC) $(CXXFLAGS) -o app