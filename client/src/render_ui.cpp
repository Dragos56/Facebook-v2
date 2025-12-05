#include "render_ui.h"

STATE app_status=LOGGED_OUT;

static bool showLogin = false;
static bool showRegister = false;
static char connected_username[USERNAME_LENGTH] = "";

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
    if (ImGui::BeginPopupModal("Login", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) 
    {
        static char username[USERNAME_LENGTH] = "";
        static char password[PASSWORD_LENGTH] = "";
        static char message[MESSAGE_LENGTH] = "";

        ImGui::InputText("Username", username, USERNAME_LENGTH);
        ImGui::InputText("Password", password, PASSWORD_LENGTH, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Login")) 
        {
            login_account(username, password, message);
            if(strncmp(message,"LOGIN OK", 8) == 0)
                {
                    app_status = LOGGED_IN;
                    strncpy(connected_username, username, USERNAME_LENGTH);
                }
            else
                app_status = LOGGED_OUT;
            ImGui::CloseCurrentPopup();
            showLogin = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) 
        {
            app_status = LOGGED_OUT;
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
    if (ImGui::BeginPopupModal("Register", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) 
    {

        static char username[USERNAME_LENGTH] = "";
        static char password[PASSWORD_LENGTH] = "";
        static char message[MESSAGE_LENGTH] = "";

        ImGui::InputText("Username", username, USERNAME_LENGTH);
        ImGui::InputText("Password", password, PASSWORD_LENGTH, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Register")) 
        {
            register_account(username, password, message);
            if(strncmp(message,"REGISTER OK", 11) == 0)
                app_status = LOGGED_IN;
            else
                app_status = LOGGED_OUT;
            ImGui::CloseCurrentPopup();
            showRegister = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) 
        {
            app_status = LOGGED_OUT;
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

void render_friends_ui()
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("VirtualSoc", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    float total_width = ImGui::GetContentRegionAvail().x;
    float total_height = ImGui::GetContentRegionAvail().y;

    ImGui::BeginChild("TopPanel", ImVec2(total_width, total_height * 0.10f), true);
    ImGui::Text("FRIENDS");
    ImGui::EndChild();

    ImGui::BeginChild("LeftPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    float button_width = total_width * 0.2f;
    float button_height = (total_height * 0.10f) * 0.50f;

    if (ImGui::Button("Home", ImVec2(button_width, button_height)))
    {
        app_status = LOGGED_IN;
    } 
    if (ImGui::Button("Friends", ImVec2(button_width, button_height))) 
    {
        app_status = FRIENDS;
    }
    if (ImGui::Button("Post", ImVec2(button_width, button_height))) 
    {
        app_status = POST;
    }
    if (ImGui::Button("Notifications", ImVec2(button_width, button_height))) 
    {
        app_status = NOTIFICATIONS;
    }
    if (ImGui::Button("Profile", ImVec2(button_width, button_height))) 
    {
        app_status = PROFILE;
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MiddlePanel", ImVec2(total_width * 0.50f, total_height * 0.90f), true);
     
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    button_width = total_width * 0.2f;
    button_height = (total_height * 0.10f) * 0.50f;

    static char username[USERNAME_LENGTH] = "";
    static char message[MESSAGE_LENGTH] = "";

    ImGui::InputText("Username", username, USERNAME_LENGTH);

    if (ImGui::Button("Follow", ImVec2(button_width, button_height)))
    {
        follow_request(connected_username, username, message);
        username[0] = '\0';
    }
    if (ImGui::Button("Accept", ImVec2(button_width, button_height)))
    {
        accept_follow_request(connected_username, username, message);
        username[0] = '\0';
    }
    if (ImGui::Button("Reject", ImVec2(button_width, button_height)))
    {
        reject_follow_request(connected_username, username, message);
        username[0] = '\0';
    }
    if (ImGui::Button("Unfollow", ImVec2(button_width, button_height)))
    {
        unfollow_request(connected_username, username, message);
        username[0] = '\0';
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::End();
}

void render_post_ui()
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("VirtualSoc", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    float total_width = ImGui::GetContentRegionAvail().x;
    float total_height = ImGui::GetContentRegionAvail().y;

    ImGui::BeginChild("TopPanel", ImVec2(total_width, total_height * 0.10f), true);
    ImGui::Text("POST");
    ImGui::EndChild();

    ImGui::BeginChild("LeftPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    float button_width = total_width * 0.2f;
    float button_height = (total_height * 0.10f) * 0.50f;

    if (ImGui::Button("Home", ImVec2(button_width, button_height)))
    {
        app_status = LOGGED_IN;
    } 
    if (ImGui::Button("Friends", ImVec2(button_width, button_height))) 
    {
        app_status = FRIENDS;
    }
    if (ImGui::Button("Post", ImVec2(button_width, button_height))) 
    {
        app_status = POST;
    }
    if (ImGui::Button("Notifications", ImVec2(button_width, button_height))) 
    {
        app_status = NOTIFICATIONS;
    }
    if (ImGui::Button("Profile", ImVec2(button_width, button_height))) 
    {
        app_status = PROFILE;
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MiddlePanel", ImVec2(total_width * 0.50f, total_height * 0.90f), true);
    button_width = total_width * 0.2f;
    button_height = (total_height * 0.10f) * 0.50f;

    static char description[MESSAGE_LENGTH] = "test";
    static char image_path[PATH_LENGTH] = "test";
    static char visibility[VISIBILITY_LENGTH] = "test";
    int post_id=0;
    static char message[MESSAGE_LENGTH] = "";

    if (ImGui::Button("Create post", ImVec2(button_width, button_height)))
    {
        post(connected_username, description, image_path, visibility, post_id, message);
    }
    if (ImGui::Button("Edit post visibility", ImVec2(button_width, button_height)))
    {
        edit_post_visibility(connected_username, visibility, post_id, message);
    }
    if (ImGui::Button("Edit post decription", ImVec2(button_width, button_height)))
    {
        edit_post_description(connected_username, description, post_id, message);
    }
    if (ImGui::Button("Delete post", ImVec2(button_width, button_height)))
    {
        delete_post(connected_username, post_id, message);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
     
    ImGui::EndChild();


    ImGui::SameLine();

    ImGui::End();
}

void render_notifications_ui()
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("VirtualSoc", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    float total_width = ImGui::GetContentRegionAvail().x;
    float total_height = ImGui::GetContentRegionAvail().y;

    ImGui::BeginChild("TopPanel", ImVec2(total_width, total_height * 0.10f), true);
    ImGui::Text("NOTIFICATIONS");
    ImGui::EndChild();

    ImGui::BeginChild("LeftPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    float button_width = total_width * 0.2f;
    float button_height = (total_height * 0.10f) * 0.50f;

    if (ImGui::Button("Home", ImVec2(button_width, button_height)))
    {
        app_status = LOGGED_IN;
    } 
    if (ImGui::Button("Friends", ImVec2(button_width, button_height))) 
    {
        app_status = FRIENDS;
    }
    if (ImGui::Button("Post", ImVec2(button_width, button_height))) 
    {
        app_status = POST;
    }
    if (ImGui::Button("Notifications", ImVec2(button_width, button_height))) 
    {
        app_status = NOTIFICATIONS;
    }
    if (ImGui::Button("Profile", ImVec2(button_width, button_height))) 
    {
        app_status = PROFILE;
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MiddlePanel", ImVec2(total_width * 0.50f, total_height * 0.90f), true);
     
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
     
    ImGui::EndChild();


    ImGui::SameLine();

    ImGui::End();
}

void render_profile_ui()
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("VirtualSoc", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    float total_width = ImGui::GetContentRegionAvail().x;
    float total_height = ImGui::GetContentRegionAvail().y;

    ImGui::BeginChild("TopPanel", ImVec2(total_width, total_height * 0.10f), true);
    ImGui::Text("PROFILE");
    ImGui::EndChild();

    ImGui::BeginChild("LeftPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    float button_width = total_width * 0.2f;
    float button_height = (total_height * 0.10f) * 0.50f;

    if (ImGui::Button("Home", ImVec2(button_width, button_height)))
    {
        app_status = LOGGED_IN;
    } 
    if (ImGui::Button("Friends", ImVec2(button_width, button_height))) 
    {
        app_status = FRIENDS;
    }
    if (ImGui::Button("Post", ImVec2(button_width, button_height))) 
    {
        app_status = POST;
    }
    if (ImGui::Button("Notifications", ImVec2(button_width, button_height))) 
    {
        app_status = NOTIFICATIONS;
    }
    if (ImGui::Button("Profile", ImVec2(button_width, button_height))) 
    {
        app_status = PROFILE;
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MiddlePanel", ImVec2(total_width * 0.50f, total_height * 0.90f), true);
     
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    button_width = total_width * 0.2f;
    button_height = (total_height * 0.10f) * 0.50f;

    static char username[USERNAME_LENGTH] = "";
    static char old_password[PASSWORD_LENGTH] = "";
    static char new_password[PASSWORD_LENGTH] = "";
    static char bio[BIO_LENGTH] = "";
    static char image_path[PATH_LENGTH] = "";
    static char visibility[VISIBILITY_LENGTH] = "";
    static char message[MESSAGE_LENGTH] = "";

    ImGui::InputText("Username", username, USERNAME_LENGTH);

    if (ImGui::Button("update name", ImVec2(button_width, button_height)))
    {
        update_name(connected_username, username, message);
        strncpy(connected_username, username, USERNAME_LENGTH);
        username[0] = '\0';
    }

    ImGui::InputText("Bio", bio, BIO_LENGTH);

    if (ImGui::Button("update bio", ImVec2(button_width, button_height)))
    {
        update_bio(connected_username, bio, message);
        bio[0] = '\0';
    }

    ImGui::InputText("Old Password", old_password, PASSWORD_LENGTH);
    ImGui::InputText("New Password", new_password, PASSWORD_LENGTH);

    if (ImGui::Button("update password", ImVec2(button_width, button_height)))
    {
        update_password(connected_username, old_password, new_password, message);
        old_password[0] = '\0';
        new_password[0] = '\0';
    }

    ImGui::InputText("Image Path", image_path, PATH_LENGTH);

    if (ImGui::Button("update image", ImVec2(button_width, button_height)))
    {
        update_profile_picture(connected_username, image_path, message);
        image_path[0] = '\0';
    }

    ImGui::InputText("Visibility", visibility, VISIBILITY_LENGTH);

    if (ImGui::Button("update visibility", ImVec2(button_width, button_height)))
    {
        update_profile_visibility(connected_username, visibility, message);
        visibility[0] = '\0';
    }

    if (ImGui::Button("Delete account", ImVec2(button_width, button_height)))
    {
        delete_account(connected_username, message);
        if(strncmp(message,"DELETE_ACCOUNT OK", 9) == 0)
        {
            app_status = LOGGED_OUT;
            connected_username[0] = '\0';
        }
        else
            app_status = LOGGED_IN;
    }

    ImGui::EndChild();


    ImGui::SameLine();

    ImGui::End();
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
     
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
     
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
    float button_width = total_width * 0.2f;
    float button_height = (total_height * 0.10f) * 0.50f;

    ImGui::SetCursorPosX((total_width - button_width * 2) * 0.5f);

    if (ImGui::Button("Logout", ImVec2(button_width, button_height))) 
        {
            static char message[MESSAGE_LENGTH] = "";

            logout_account(connected_username, message);
            if(strncmp(message,"LOGOUT OK", 9) == 0)
                {
                    app_status = LOGGED_OUT;
                    connected_username[0] = '\0';
                }
            else
                app_status = LOGGED_IN;
            ImGui::CloseCurrentPopup();
            showLogin = false;
        }
    ImGui::EndChild();

    ImGui::BeginChild("LeftPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
    button_width = total_width * 0.2f;
    button_height = (total_height * 0.10f) * 0.50f;

    if (ImGui::Button("Home", ImVec2(button_width, button_height)))
    {
        app_status = LOGGED_IN;
    } 
    if (ImGui::Button("Friends", ImVec2(button_width, button_height))) 
    {
        app_status = FRIENDS;
    }
    if (ImGui::Button("Post", ImVec2(button_width, button_height))) 
    {
        app_status = POST;
    }
    if (ImGui::Button("Notifications", ImVec2(button_width, button_height))) 
    {
        app_status = NOTIFICATIONS;
    }
    if (ImGui::Button("Profile", ImVec2(button_width, button_height))) 
    {
        app_status = PROFILE;
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MiddlePanel", ImVec2(total_width * 0.50f, total_height * 0.90f), true);
    button_width = total_width * 0.2f;
    button_height = (total_height * 0.10f) * 0.50f;

    static char author[USERNAME_LENGTH]="test";
    static char comment[MESSAGE_LENGTH]="test";
    int post_id=0;
    static char message[MESSAGE_LENGTH]="";

    if (ImGui::Button("Like", ImVec2(button_width, button_height))) 
    {
        like_post(connected_username, author, post_id, message);
    }
    if (ImGui::Button("Comment", ImVec2(button_width, button_height))) 
    {
        comment_post(connected_username, author, post_id, comment, message);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(total_width * 0.24f, total_height * 0.90f), true);
     
    ImGui::EndChild();


    ImGui::SameLine();

    ImGui::End();
}

void render_ui()
{
    if(app_status == LOGGED_OUT)
        render_loggedOut_ui();
    else if(app_status == LOGGED_IN)
        render_loggedIn_ui();
    else if(app_status == FRIENDS)
        render_friends_ui();
    else if(app_status == POST)
        render_post_ui();
    else if(app_status == NOTIFICATIONS)
        render_notifications_ui();
    else if(app_status == PROFILE)
        render_profile_ui();
}