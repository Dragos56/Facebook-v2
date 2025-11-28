#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "network.h"

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Test", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!connect_to_server("10.100.0.30", 5656)) {
        printf("Nu m-am putut conecta la server!\n");
    } else {
        printf("Conectat cu succes!\n");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static char command[256] = "";
        string response;
        
        ImGui::Begin("Client VirtualSoc");

        ImGui::InputText("Comanda", command, IM_ARRAYSIZE(command));

        if (ImGui::Button("Trimite")) {
            send_message(command);

            int n = receive_message(response);
            if (n <= 0) {
                response = "Eroare la primire.";
            }
        }

        ImGui::Text("Raspuns server:");
        ImGui::TextWrapped("%s", response.c_str());

        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}