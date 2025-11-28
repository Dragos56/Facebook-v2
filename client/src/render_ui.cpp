#include "render_ui.h"

static bool showLogin = false;
static bool showRegister = false;

void render_login_window(float total_width, float total_height)
{
    ImVec2 popup_size = ImVec2(total_width * 0.25f, total_height * 0.20f);
    ImVec2 window_size = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(
        (window_size.x - popup_size.x) * 0.5f, 
        (window_size.y - popup_size.y) * 0.5f
    ), ImGuiCond_Appearing);

    ImGui::SetNextWindowSize(popup_size, ImGuiCond_Appearing);
    ImGui::OpenPopup("Login");
    if (ImGui::BeginPopupModal("Login", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        static char username[USERNAME_LENTH] = "";
        static char password[PASSWORD_LENGTH] = "";
        static char message[MESSAGE_LENGTH] = "";

        ImGui::InputText("Username", username, USERNAME_LENTH);
        ImGui::InputText("Password", password, PASSWORD_LENGTH, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Login")) {
            login_account(username, password, message);
            ImGui::CloseCurrentPopup();
            showLogin = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            showLogin = false;
        }

        if(!showLogin)
        {
            username[0] = '\0';
            password[0] = '\0';
            message[0] = '\0';
        }

        ImGui::EndPopup();
    }
}

void render_register_window(float total_width, float total_height)
{
    ImVec2 popup_size = ImVec2(total_width * 0.25f, total_height * 0.20f);
    ImVec2 window_size = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(
        (window_size.x - popup_size.x) * 0.5f, 
        (window_size.y - popup_size.y) * 0.5f
    ), ImGuiCond_Appearing);

    ImGui::SetNextWindowSize(popup_size, ImGuiCond_Appearing);
    ImGui::OpenPopup("Register");
    if (ImGui::BeginPopupModal("Register", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {

        static char username[USERNAME_LENTH] = "";
        static char password[PASSWORD_LENGTH] = "";
        static char message[MESSAGE_LENGTH] = "";

        ImGui::InputText("Username", username, USERNAME_LENTH);
        ImGui::InputText("Password", password, PASSWORD_LENGTH, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Register")) {
            register_account(username, password, message);
            ImGui::CloseCurrentPopup();
            showRegister = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            showRegister = false;
        }
        
        if(!showRegister)
        {
            username[0] = '\0';
            password[0] = '\0';
            message[0] = '\0';
        }

        ImGui::EndPopup();
    }
}

void render_loggedOut_ui() 
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("VirtualSoc", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    float total_width = ImGui::GetContentRegionAvail().x;
    float total_height = ImGui::GetContentRegionAvail().y;

    ImGui::BeginChild("TopPanel", ImVec2(total_width, total_height * 0.10f), true);
    float button_width = total_width * 0.2f;
    float button_height = (total_height * 0.10f) * 0.50f;

    ImGui::SetCursorPosX((total_width - button_width * 2) * 0.5f);
    
    if (ImGui::Button("LOGIN", ImVec2(button_width, button_height))) 
    {
        showLogin = true;
    }
    if(showLogin)
        render_login_window(total_width, total_height);
    
    ImGui::SameLine();

    if (ImGui::Button("REGISTER", ImVec2(button_width, button_height))) 
    {
        showRegister=true;
    }
    if(showRegister)
        render_register_window(total_width, total_height);
    
    ImGui::EndChild();

    ImGui::BeginChild("LeftPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    ImGui::Text("PARTEA DIN STANGA");
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MiddlePanel", ImVec2(total_width * 0.50f, total_height * 0.90f), true);
    ImGui::Text("PARTEA DIN MIJLOC");
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    ImGui::Text("PARTEA DIN DREAPTA");
    ImGui::EndChild();


    ImGui::SameLine();

    ImGui::End();
}

void render_loggedIn_ui() 
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("VirtualSoc", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    float total_width = ImGui::GetContentRegionAvail().x;
    float total_height = ImGui::GetContentRegionAvail().y;

    ImGui::BeginChild("TopPanel", ImVec2(total_width, total_height * 0.10f), true);
    ImGui::Text("PARTEA DE SUS");
    ImGui::EndChild();

    ImGui::BeginChild("LeftPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    ImGui::Text("PARTEA DIN STANGA");
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MiddlePanel", ImVec2(total_width * 0.50f, total_height * 0.90f), true);
    ImGui::Text("PARTEA DIN MIJLOC");
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    ImGui::Text("PARTEA DIN DREAPTA");
    ImGui::EndChild();


    ImGui::SameLine();

    ImGui::End();
}