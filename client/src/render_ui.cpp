#include "render_ui.h"
#include "messages.h"
#include <string>
#include <cstdio>
#include <cstring>

STATE app_status = LOGGED_OUT;

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

static char response[MESSAGE_LENGTH];

static bool friends_loaded = false;
static bool follow_request_loaded = false;


void client_shutdown()
{
    if (user_id != -1) 
    {
        char response[MESSAGE_LENGTH];
        logout_account(&user_id, response);
    }
}

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
                    feed_loaded = false;
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
    ImGui::Begin("TopPanel", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if(app_status == LOGGED_OUT)
    {
        if(ImGui::Button("Login")) { showLogin = true; }
        ImGui::SameLine();
        if(ImGui::Button("Register")) { showRegister = true; }
    }
    else
    {
        ImGui::Text("Logged in as: %s", connected_username);
        ImGui::SameLine();
        if(ImGui::Button("Logout"))
        {
            app_status = LOGGED_OUT;
            logout_account(&user_id, response);
            connected_username[0] = '\0';
            feed_loaded = false;
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
    ImGui::Begin("LeftPanel", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    float bw = left_w - 20;
    
    if(user_id != -1)
    {
        if(ImGui::Button("Home", ImVec2(bw, 45))) 
        {
            app_status = LOGGED_IN;
            feed_loaded = false;
        }

        if(ImGui::Button("Friends", ImVec2(bw, 45))) 
        {
            app_status = FRIENDS;
            friends_loaded = false;
            follow_request_loaded = false;
        }

        if(ImGui::Button("Post", ImVec2(bw, 45))) app_status = POST;
        if(ImGui::Button("Profile", ImVec2(bw, 45))) 
        {
            app_status = PROFILE;
            user_posts_loaded = false;
        }
    }
    ImGui::End();
}

static void render_right_panel(float w, float h)
{
    float top_h = h * 0.10f;
    float right_w = w * 0.18f;

    ImGui::SetNextWindowPos(ImVec2(w - right_w, top_h));
    ImGui::SetNextWindowSize(ImVec2(right_w, h - top_h));
    ImGui::Begin("RightPanel", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if(app_status == FRIENDS)
    {
        ImGui::Text("Friends:");
        if(!friends_loaded)
        {
            get_friends_list(user_id, friends, &friend_count, response);
            friends_loaded = true;
        }
        for(int i = 0; i < friend_count; i++)
        {
            ImGui::Text("%s", friends[i].display_name);
            ImGui::Separator();
        }
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
    ImGui::Begin("MainPanel", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    switch(app_status)
    {
        case LOGGED_OUT:
            if(!feed_loaded)
            {
                get_feed(user_id, feed, &feed_count, response);
                feed_loaded = true;
            }
            for(int i = 0; i < feed_count; i++)
            {
                ImGui::Text("Post by %s:", feed[i].display_name);
                ImGui::Text("%s", feed[i].content);

                ImGui::Text("Likes: %d  Comments: %d", feed[i].like_count, feed[i].comment_count);

                ImGui::Separator();
            }
            break;
        case LOGGED_IN:
            ImGui::Text("Home Feed");
            ImGui::Separator();
            if(!feed_loaded)
            {
                get_feed(user_id, feed, &feed_count, response);
                feed_loaded = true;
            }
            static bool add_comment[MAX_POSTS] = { false };
            static char comment_buffer[MAX_COMMENTS][COMMENT_LENGTH] = { "" };
            for(int i = 0; i < feed_count; i++)
            {
                ImGui::Text("Post by %s:", feed[i].display_name);
                ImGui::Text("%s", feed[i].content);
                ImGui::Text("Likes: %d  Comments: %d", feed[i].like_count, feed[i].comment_count);
                if (ImGui::Button(("Like##"+std::to_string(i)).c_str()))
                {
                    like_post(user_id, feed[i].post_id, response);
                    feed[i].like_count++;
                }
                ImGui::SameLine();
                if(add_comment[i])
                {

                    ImGui::InputText("Comment", comment_buffer[i], COMMENT_LENGTH);
                    ImGui::SameLine();
                    if(ImGui::Button(("Submit##"+std::to_string(i)).c_str()))
                    {
                        if(strcmp(comment_buffer[i], "") != 0)
                        {
                            comment_post(user_id, feed[i].post_id, comment_buffer[i], response);
                            get_user_posts(user_id, feed, &feed_count, response);
                        }
                        add_comment[i] = false;
                    }
                }
                else
                {
                    if (ImGui::Button(("Comment##"+std::to_string(i)).c_str())) add_comment[i] = true;
                }
                ImGui::Separator();
            }
            break;

        case FRIENDS:
            static char follow_username[USERNAME_LENGTH] = "";
            ImGui::Text("Add Friend:");
            ImGui::InputText("Username", follow_username, USERNAME_LENGTH);
            if(ImGui::Button("Send Follow Request"))
            {
                follow_request(user_id, follow_username, response);
                follow_username[0] = '\0';
            }
            ImGui::Separator();
            ImGui::Text("Follow Requests:");
            if(!follow_request_loaded)
            {
                get_follow_requests(user_id, follow_requests, &follow_request_count, response);
                follow_request_loaded = true;
            }
            for(int i = 0; i < follow_request_count; i++)
            {
                ImGui::Text("%s", follow_requests[i].display_name);
                ImGui::SameLine();
                if (ImGui::Button(("Accept##"+std::to_string(i)).c_str()))
                {   
                    accept_follow_request(user_id, follow_requests[i].display_name, response);
                    follow_request_loaded = false;
                }
                ImGui::SameLine();
                if (ImGui::Button(("Reject##"+std::to_string(i)).c_str()))
                {
                    reject_follow_request(user_id, follow_requests[i].display_name, response);
                }
            }
            ImGui::Separator();
            break;

        case POST:
            ImGui::Text("Create Post:");
            ImGui::Separator();
            static char content[MESSAGE_LENGTH] = "";
            static int post_visibility = 0;
            ImGui::InputTextMultiline("Content", content, MESSAGE_LENGTH, ImVec2(300, 100));

            ImGui::Text("Visibility:");
            ImGui::SameLine();
            ImGui::RadioButton("Public", &post_visibility, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Private", &post_visibility, 1);

            if(ImGui::Button("Submit Post"))
            {
                post(user_id, content, post_visibility, response);
                content[0] = '\0';
            }
            break;

        case PROFILE:
            ImGui::Text("User Profile:");
            ImGui::Separator();

            static bool edit_mode = false;
            static bool add_comment_profile_post[MAX_POSTS] = { false };
            static char username[USERNAME_LENGTH] = "";
            static char display_name[USERNAME_LENGTH] = "";
            static char bio[BIO_LENGTH] = "";
            static int profile_visibility;
            static Post user_posts[MAX_POSTS];
            static int user_post_count = 0;
            
            if(!user_posts_loaded)
            {
                get_profile(user_id, username, bio, display_name, &profile_visibility, response);
                get_user_posts(user_id, user_posts, &user_post_count, response);
                user_posts_loaded = true;
            }
            
            ImGui::Text("Username: %s", username);
            ImGui::Text("Display Name: %s", display_name);
            ImGui::Text("Bio: %s", bio);
            ImGui::Text("Visibility: %d", profile_visibility);
            if(edit_mode)
            {
                ImGui::InputText("Display Name", display_name, USERNAME_LENGTH);
                ImGui::InputTextMultiline("Bio", bio, BIO_LENGTH);

                ImGui::Text("Visibility:");
                ImGui::SameLine();
                ImGui::RadioButton("Public", &profile_visibility, 0);
                ImGui::SameLine();
                ImGui::RadioButton("Private", &profile_visibility, 1);

                if(ImGui::Button("Save Changes"))
                {
                    update_display_name(user_id, display_name, response);
                    update_bio(user_id, bio, response);
                    update_profile_visibility(user_id, profile_visibility, response);
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
            ImGui::Text("Your Posts:");
            static char comment_buffer_profile[MAX_COMMENTS][COMMENT_LENGTH] = { "" };
            for(int i = 0; i < user_post_count; i++)
            {
                ImGui::Text("%s", user_posts[i].content);
                ImGui::Text("Likes: %d  Comments: %d", user_posts[i].like_count, user_posts[i].comment_count);

                if(ImGui::Button(("Like##"+std::to_string(i)).c_str()))
                {
                    like_post(user_id, user_posts[i].post_id, response);
                    get_user_posts(user_id, user_posts, &user_post_count, response);
                }

                ImGui::SameLine();

                if(add_comment_profile_post[i])
                {
                    ImGui::InputText("Comment", comment_buffer_profile[i], COMMENT_LENGTH);
                    ImGui::SameLine();
                    if(ImGui::Button(("Submit##"+std::to_string(i)).c_str()))
                    {
                        if(strcmp(comment_buffer_profile[i], "") != 0)
                        {
                            comment_post(user_id, user_posts[i].post_id, comment_buffer_profile[i], response);
                            get_user_posts(user_id, user_posts, &user_post_count, response);
                        }
                        add_comment_profile_post[i] = false;
                    }
                }
                else
                {
                    if(ImGui::Button(("Comment##"+std::to_string(i)).c_str())) add_comment_profile_post[i] = true;
                }
                ImGui::Separator();
            }
            break;
    }

    ImGui::End();
}

void render_ui(float w, float h)
{
    render_top_panel(w,h);
    render_left_panel(w,h);
    render_main_panel(w,h);
    render_right_panel(w,h);

    if(showLogin)    render_login_window(w,h);
    if(showRegister) render_register_window(w,h);
}