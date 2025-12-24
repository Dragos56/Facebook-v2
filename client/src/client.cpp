#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Icons.h"
#include <GLFW/glfw3.h>
#include "network.h"
#include "render_ui.h"
#include "messages.h"
void apply_style()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.FramePadding = ImVec2(10, 8);
    style.WindowPadding = ImVec2(15, 15);
    style.ItemSpacing = ImVec2(10, 12);
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;

    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    colors[ImGuiCol_ChildBg]  = ImVec4(0.09f, 0.09f, 0.12f, 0.00f);
    colors[ImGuiCol_PopupBg]  = ImVec4(0.10f, 0.10f, 0.14f, 1.00f);

    ImVec4 accentColor = ImVec4(0.00f, 0.47f, 1.00f, 1.00f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.13f, 0.13f, 0.17f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.24f, 0.24f, 0.30f, 1.00f);
    
    colors[ImGuiCol_CheckMark]        = accentColor;
    colors[ImGuiCol_SliderGrab]       = accentColor;
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.55f, 1.00f, 1.00f);
    
    colors[ImGuiCol_Button]        = accentColor;
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.55f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive]  = ImVec4(0.00f, 0.40f, 1.00f, 1.00f);

    ImVec4 separatorColor = ImVec4(0.20f, 0.20f, 0.28f, 1.00f); 
    colors[ImGuiCol_Separator] = separatorColor;
    colors[ImGuiCol_Border]    = separatorColor;
}

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "DragosBook", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!connect_to_server("127.0.0.1", 5656)) 
    {
        printf("Nu m-am putut conecta la server!\n");
    } 
    else 
    {
        printf("Conectat cu succes!\n");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    load_font();

    apply_style();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImVec2 size = ImGui::GetIO().DisplaySize;
        render_ui(size.x, size.y);

        ImGui::Render();
        glClearColor(0.05f, 0.05f, 0.07f, 1.00f); 
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    client_shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    
    return 0;
}
