#include "render_ui.h"
#include "messages.h"
#include <string>
#include <cstdio>
#include <cstring>

STATE app_status = LOGGED_OUT;

// Structuri globale pentru date
static Request follow_requests[MAX_FRIENDS];
static int follow_request_count = 0;

static Friend friends[MAX_FRIENDS];
static int friend_count = 0;

static Post feed[MAX_POSTS];
static int feed_count = 0;
static bool feed_loaded = false;

static Post user_posts1[MAX_POSTS];
static int user_post_count1 = 0;
static bool user_posts_loaded = false;

static int user_id = -1;
static bool showLogin = false;
static bool showRegister = false;
static char connected_username[USERNAME_LENGTH] = "";

// Mesaje de raspuns de la server
static char response[MESSAGE_LENGTH];

// ---- Functii de shutdown ---- //
void client_shutdown()
{
    if (user_id != -1) 
    {
        char response[MESSAGE_LENGTH];
        logout_account(&user_id, response);
    }
}

// ---- Login / Register ---- //
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
        static char error_message[100] = "";
        static bool has_error = false;

        ImGui::InputText("Username", username, USERNAME_LENGTH);
        ImGui::InputText("Password", password, PASSWORD_LENGTH, ImGuiInputTextFlags_Password);

        if (has_error)
            ImGui::Text("%s", error_message);

        if (ImGui::Button("Login")) 
        {
            if(strlen(username) == 0 || strlen(password) == 0)
            {
                strcpy(error_message, "Username and password cannot be empty.");
                has_error = true;
            }
            else
            {
                login_account(username, password, response);

                if(strncmp(response,"LOGIN OK", 8) == 0)
                {
                    app_status = LOGGED_IN;
                    strncpy(connected_username, username, USERNAME_LENGTH);
                    user_id = atoi(response + 9);
                    feed_loaded = false;  // reset feed pentru home
                    showLogin = false;
                    has_error = false;
                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    app_status = LOGGED_OUT;
                    strcpy(error_message, "Login failed: Invalid username or password.");
                    has_error = true;
                }
            }
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
            response[0] = '\0';
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
        static char error_message[100] = "";
        static bool has_error = false;

        ImGui::InputText("Username", username, USERNAME_LENGTH);
        ImGui::InputText("Password", password, PASSWORD_LENGTH, ImGuiInputTextFlags_Password);

        if (has_error)
            ImGui::Text("%s", error_message);

        if (ImGui::Button("Register")) 
        {
            if(strlen(username) == 0 || strlen(password) == 0)
            {
                strcpy(error_message, "Username and password cannot be empty.");
                has_error = true;
            }
            else
            {
                register_account(username, password, response);

                if(strncmp(response,"LOGIN OK", 8) == 0)
                {
                    app_status = LOGGED_IN;
                    strncpy(connected_username, username, USERNAME_LENGTH);
                    user_id = atoi(response + 9);
                    feed_loaded = false;
                    showRegister = false;
                    has_error = false;
                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    app_status = LOGGED_OUT;
                    if(strncmp(response, "REGISTER ERROR|User already exists", 34) == 0)
                        strcpy(error_message, "User already exists.");
                    else
                        strcpy(error_message, "Registration failed. Please try again.");
                    has_error = true;
                }
            }
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
            response[0] = '\0';
        }

        ImGui::EndPopup();
    }
}

static void render_top_panel(float w, float h)
{
    float top_h = h * 0.10f;
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(w, top_h));
    ImGui::Begin("TopPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if(app_status == LOGGED_OUT)
    {
        if(ImGui::Button("Login")) showLogin = true;
        ImGui::SameLine();
        if(ImGui::Button("Register")) showRegister = true;
    }
    else
    {
        ImGui::Text("Logged in as: %s", connected_username);
        ImGui::SameLine();
        if(ImGui::Button("Logout"))
        {
            client_shutdown();
            app_status = LOGGED_OUT;
            feed_loaded = false;
            user_posts_loaded = false;
            friend_count = 0;
            follow_request_count = 0;
        }
    }

    ImGui::End();
}

static void render_left_panel(float w, float h)
{
    float top_h = h * 0.10f;
    float left_w = w * 0.18f;

    ImGui::SetNextWindowPos(ImVec2(0, top_h));
    ImGui::SetNextWindowSize(ImVec2(left_w, h - top_h));
    ImGui::Begin("LeftPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if(app_status != LOGGED_OUT)
    {
        float button_width  = left_w - 20;
        float button_height = 50.0f;

        if(ImGui::Button("Home", ImVec2(button_width, button_height))) 
        { 
            app_status = LOGGED_IN; 
            feed_loaded = false;  // forțăm reload feed
        }

        if(ImGui::Button("Friends", ImVec2(button_width, button_height)))
        {
            app_status = FRIENDS;
            if(friend_count == 0) get_friends_list(user_id, friends, &friend_count, response);
            if(follow_request_count == 0) get_follow_requests(user_id, follow_requests, &follow_request_count, response);
        }

        if(ImGui::Button("Post", ImVec2(button_width, button_height))) app_status = POST;
        if(ImGui::Button("Notifications", ImVec2(button_width, button_height))) app_status = NOTIFICATIONS;
        if(ImGui::Button("Profile", ImVec2(button_width, button_height))) 
        { 
            app_status = PROFILE;
            user_posts_loaded = false;
        }
    }

    ImGui::End();
}

static void render_right_panel(float w, float h)
{
    float top_h   = h * 0.10f;
    float right_w = w * 0.18f;

    ImGui::SetNextWindowPos(ImVec2(w - right_w, top_h));
    ImGui::SetNextWindowSize(ImVec2(right_w, h - top_h));
    ImGui::Begin("RightPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if(app_status == FRIENDS)
    {
        ImGui::Text("Friends Online:");
        for(int i = 0; i < friend_count; i++)
            ImGui::BulletText("%s", friends[i].username);
    }
    else
    {
        ImGui::Text("Right panel empty");
    }

    ImGui::End();
}

static void render_main_panel(float w, float h)
{
    float top_h = h * 0.10f;
    float left_w = w * 0.18f;
    float right_w = w * 0.18f;

    ImGui::SetNextWindowPos(ImVec2(left_w, top_h));
    ImGui::SetNextWindowSize(ImVec2(w - left_w - right_w, h - top_h));
    ImGui::Begin("MainPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    switch(app_status)
    {
        case LOGGED_OUT:
            ImGui::Text("Please login or register.");
            break;

        case LOGGED_IN: // Home feed
            if(!feed_loaded)
            {
                get_feed(user_id, feed, &feed_count, response);
                feed_loaded = true;
            }

            ImGui::Text("Home Feed:");
            ImGui::Separator();
            for(int i = 0; i < feed_count; i++)
            {
                ImGui::Text("%s:", feed[i].username);
                ImGui::TextWrapped("%s", feed[i].description);
                ImGui::Text("Likes: %d  Comments: %d", feed[i].like_count, feed[i].comment_count);

                // Like button
                ImGui::SameLine();
                if(ImGui::Button(("Like##"+std::to_string(feed[i].post_id)).c_str()))
                {
                    like_post(user_id, feed[i].post_id, response);
                    feed_loaded = false;
                }

                // Comments
                static char comment_buf[MAX_POSTS][COMMENT_LENGTH] = {};
                for(int j = 0; j < feed[i].comment_count; j++)
                {
                    char commenter_name[USERNAME_LENGTH];
                    get_username_by_id(feed[i].comment_user_ids[j], commenter_name, response);
                    ImGui::Text("%s: %s", commenter_name, feed[i].comments[j]);
                }

                ImGui::InputText(("Comment##"+std::to_string(feed[i].post_id)).c_str(), comment_buf[feed[i].post_id], COMMENT_LENGTH);
                ImGui::SameLine();
                if(ImGui::Button(("Send##"+std::to_string(feed[i].post_id)).c_str()))
                {
                    comment_post(user_id, feed[i].post_id, comment_buf[feed[i].post_id], response);
                    feed_loaded = false;
                    comment_buf[feed[i].post_id][0] = '\0';
                }

                ImGui::Separator();
            }
            break;

        case FRIENDS:
            ImGui::Text("Friends & Follow Requests:");
            ImGui::Separator();
            for(int i = 0; i < follow_request_count; i++)
            {
                ImGui::Text("%s wants to follow you.", follow_requests[i].username);
                ImGui::SameLine();
                if(ImGui::Button(("Accept##" + std::to_string(i)).c_str())) { /* accept logic */ }
                ImGui::SameLine();
                if(ImGui::Button(("Reject##" + std::to_string(i)).c_str())) { /* reject logic */ }
            }

            ImGui::Separator();
            ImGui::Text("Your Friends:");
            for(int i = 0; i < friend_count; i++)
                ImGui::BulletText("%s", friends[i].username);
            break;

        case POST:
        {
            static char new_post_text[MESSAGE_LENGTH] = "";
            static char new_post_image[PATH_LENGTH] = "";

            ImGui::Text("Create a new post:");
            ImGui::InputTextMultiline("Description", new_post_text, MESSAGE_LENGTH, ImVec2(-FLT_MIN/2, 50));
            ImGui::InputText("Image path", new_post_image, PATH_LENGTH);

            if(ImGui::Button("Post"))
            {
                post(user_id, new_post_text, new_post_image, 1, 0, response);
                new_post_text[0] = '\0';
                new_post_image[0] = '\0';
                feed_loaded = false; 
            }
        }
        break;

        case NOTIFICATIONS:
            ImGui::Text("Notifications page.");
            break;

        case PROFILE:
        {
            static bool edit_mode = false;
            static char username[USERNAME_LENGTH];
            static char display_name[USERNAME_LENGTH];
            static char bio[BIO_LENGTH];
            static char avatar_path[PATH_LENGTH];
            static int visibility = 1;
            static Post user_posts[MAX_POSTS];
            static int user_post_count = 0;

            if(!user_posts_loaded)
            {
                get_profile(user_id, username, bio, display_name, avatar_path, &visibility, response);
                get_user_posts(user_id, user_posts, &user_post_count, response);
                user_posts_loaded = true;
            }

            ImGui::Text("Profile:");
            ImGui::Text("Username: %s", username);
            ImGui::TextWrapped("Bio: %s", bio);
            ImGui::Text("Display Name: %s", display_name);
            ImGui::Text("Avatar: %s", avatar_path);

            if(edit_mode)
            {
                ImGui::InputText("Display Name", display_name, USERNAME_LENGTH);
                ImGui::InputTextMultiline("Bio", bio, BIO_LENGTH);
                ImGui::InputText("Avatar Path", avatar_path, PATH_LENGTH);

                if(ImGui::Button("Save Changes"))
                {
                    update_display_name(user_id, display_name, response);
                    update_bio(user_id, bio, response);
                    update_profile_avatar(user_id, avatar_path, response);
                    edit_mode = false;
                    user_posts_loaded = false;
                }
                ImGui::SameLine();
                if(ImGui::Button("Cancel")) edit_mode = false;
            }
            else
            {
                if(ImGui::Button("Edit Profile")) edit_mode = true;
            }
            
            ImGui::Separator();
            ImGui::Text("My Posts:");
            for(int i = 0; i < user_post_count; i++)
            {    
                ImGui::TextWrapped("%s", user_posts[i].description);

                ImGui::Text("Likes: %d  Comments: %d", user_posts[i].like_count, user_posts[i].comment_count);

                if(ImGui::Button(("Like##" + std::to_string(i)).c_str())) 
                {
                    like_post(user_id, user_posts[i].post_id, response);
                    get_user_posts(user_id, user_posts, &user_post_count, response);
                }
                
                ImGui::SameLine();
                if(ImGui::Button(("Comment##" + std::to_string(i)).c_str())) 
                {
                    // logic comment
                }

                ImGui::Spacing();
                ImGui::Separator();
            }   
        }
        break;
    }

    ImGui::End();
}

// ---- Functie principala ---- //
void render_ui(float window_width, float window_height)
{
    render_top_panel(window_width, window_height);
    render_left_panel(window_width, window_height);
    render_main_panel(window_width, window_height);
    render_right_panel(window_width, window_height);

    if(showLogin)    render_login_window(window_width, window_height);
    if(showRegister) render_register_window(window_width, window_height);
}