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

static int user_id = -1;
static bool showLogin = false;
static bool showRegister = false;
static char connected_username[USERNAME_LENGTH] = "";

static char response[MESSAGE_LENGTH];

static bool user_posts_loaded = false;
static bool friends_loaded = false;
static bool follow_request_loaded = false;
static bool other_user_posts_loaded = false;
static bool other_user_friends_loaded = false;

static ImFont* customFontBig;
    
void load_font()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/font.ttf", 20.0f);
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("assets/icons.ttf", 16.0f, &icons_config, icons_ranges);
    
    customFontBig=io.Fonts->AddFontFromFileTTF("assets/font.ttf", 26.0f);
}

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

    if (ImGui::BeginPopupModal("Login", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) 
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
                    strcpy(error_message, "Invalid username or password.");
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

    if (ImGui::BeginPopupModal("Register", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) 
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
    float top_h = h * 0.12f;
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(w, top_h));
    ImGui::Begin("TopPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    if(app_status == LOGGED_OUT)
    {
        if(ImGui::Button("Login")) { showLogin = true; }
        ImGui::SameLine();
        if(ImGui::Button("Register")) { showRegister = true; }
    }
    else
    {
        ImGui::Text("Logged in as: %s", connected_username);
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
    float top_h = h * 0.12f;
    float left_w = w * 0.18f;

    ImGui::SetNextWindowPos(ImVec2(0, top_h));
    ImGui::SetNextWindowSize(ImVec2(left_w, h - top_h));
    ImGui::Begin("LeftPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    float bw = left_w - 20;
    
    if(user_id != -1)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));

        if(ImGui::Button(ICON_FA_HOUSE " Home", ImVec2(bw, 60))) 
        {
            app_status = LOGGED_IN;
            feed_loaded = false;
        }
        
        if(ImGui::Button(ICON_FA_MAGNIFYING_GLASS " Search", ImVec2(bw, 60))) 
        {
            app_status = SEARCH;
            follow_request_loaded = false;
        }
    
        if(ImGui::Button(ICON_FA_USERS " Friends", ImVec2(bw, 60))) 
        {
            app_status = FRIENDS;
            friends_loaded = false;
        }

        if(ImGui::Button(ICON_FA_SQUARE_PLUS" Post", ImVec2(bw, 60))) app_status = POST;

        if(ImGui::Button(ICON_FA_USER" Profile", ImVec2(bw, 60))) 
        {
            app_status = PROFILE;
            user_posts_loaded = false;
        }

        ImGui::PopStyleColor();
    }
    ImGui::End();
}

static void render_right_panel(float w, float h)
{
    float top_h = h * 0.12f;
    float right_w = w * 0.18f;

    ImGui::SetNextWindowPos(ImVec2(w - right_w, top_h));
    ImGui::SetNextWindowSize(ImVec2(right_w, h - top_h));
    ImGui::Begin("RightPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if(app_status == FRIENDS)
    {
        if(!friends_loaded)
        {
            get_friends_list(user_id, friends, &friend_count, response);
            friends_loaded = true;
        }
        ImGui::Text("Close friends:");
        for(int i = 0; i < friend_count; i++)
        {
            if(friends[i].close_friend)
            {
                ImGui::Text("%s", friends[i].display_name);
                ImGui::Separator();
            }
        }
        ImGui::Separator();
        ImGui::Text("Friends:");
        for(int i = 0; i < friend_count; i++)
        {
            if(!friends[i].close_friend)
            {
                ImGui::Text("%s", friends[i].display_name);
                ImGui::Separator();
            }
        }
    }

    if(app_status == SEARCH)
    {
        ImGui::Text("Follow Requests:");
        if(!follow_request_loaded)
        {
            get_follow_requests(user_id, follow_requests, &follow_request_count, response);
            follow_request_loaded = true;
        }
        for(int i = 0; i < follow_request_count; i++)
        {
            ImGui::Text("%s", follow_requests[i].display_name);
            ImGui::NewLine();
            if (ImGui::Button(("Accept##"+std::to_string(i)).c_str()))
            {   
                accept_follow_request(user_id, follow_requests[i].display_name, response);
                follow_request_loaded = false;
            }
            ImGui::SameLine();
            if (ImGui::Button(("Reject##"+std::to_string(i)).c_str()))
            {
                reject_follow_request(user_id, follow_requests[i].display_name, response);
                follow_request_loaded = false;
            }
        }
        ImGui::Separator();
    }
    ImGui::End();
}

static void render_main_panel(float w, float h)
{
    float top_h = h * 0.12f;
    float left_w = w * 0.18f;
    float right_w = w * 0.18f;
    static int other_user_id = -1;

    ImGui::SetNextWindowPos(ImVec2(left_w, top_h));
    ImGui::SetNextWindowSize(ImVec2(w - left_w - right_w, h - top_h));
    ImGui::Begin("MainPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

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
                if(feed[i].visibility == 1 || feed[i].visibility == 2) continue;
                ImGui::Text("Post by %s:", feed[i].display_name);

                ImGui::PushFont(customFontBig);
                ImGui::Text("%s", feed[i].content);
                ImGui::PopFont();

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
                if(feed[i].visibility == 1) 
                {
                    bool is_friend = false;
                    for(int j = 0; j < friend_count; j++)
                    {
                        if(feed[i].user_id == friends[j].user_id)
                        {
                            is_friend = true;
                            break;
                        }
                    }
                    if(!is_friend) continue;
                }
                else if(feed[i].visibility == 2)
                {
                    bool is_close_friend = false;
                    for(int j = 0; j < friend_count; j++)
                    {
                        if(feed[i].user_id == friends[j].user_id && friends[j].close_friend)
                        {
                            is_close_friend = true;
                            break;
                        }
                    }
                    if(!is_close_friend) continue;
                }
                ImGui::Text("Post by %s:", feed[i].display_name);
                ImGui::PushFont(customFontBig);
                ImGui::Text("%s", feed[i].content);
                ImGui::PopFont();
                ImGui::Text("Likes: %d  Comments: %d", feed[i].like_count, feed[i].comment_count);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));

                if (ImGui::Button((ICON_FA_HEART"##"+std::to_string(i)).c_str()))
                {
                    like_post(user_id, feed[i].post_id, response);
                }
                ImGui::SameLine();
                if(add_comment[i])
                {
                    if(ImGui::Button(("Submit comment##"+std::to_string(i)).c_str()))
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
                    if (ImGui::Button((ICON_FA_COMMENT"##"+std::to_string(i)).c_str())) add_comment[i] = true;
                }

                ImGui::SameLine();

                if (ImGui::Button((ICON_FA_SHARE_NODES "##"+std::to_string(i)).c_str())) { int ok; }
                
                ImGui::PopStyleColor();
                ImGui::Separator();
            }
            break;

        case SEARCH:
            static char follow_username[USERNAME_LENGTH] = "";
            ImGui::Text("Search:");
            ImGui::InputText("Username", follow_username, USERNAME_LENGTH);
            if(ImGui::Button("Search"))
            {
                if(strlen(follow_username) > 0)
                {
                    app_status = OTHER_USERS_PROFILE;
                    search_user(follow_username, &other_user_id, response);
                    follow_username[0] = '\0';
                    other_user_posts_loaded = false;
                    other_user_friends_loaded = false;
                    friends_loaded = false;
                }
            }
            ImGui::Separator();
            break;
        
        case FRIENDS:
            ImGui::Text("Chat with your friends!");
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
            ImGui::SameLine();
            ImGui::RadioButton("Close friends", &post_visibility, 2);

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
                    if(strlen(display_name) > 0)
                        update_display_name(user_id, display_name, response);
                    if(strlen(bio) > 0)
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
                ImGui::PushFont(customFontBig);
                ImGui::Text("%s", user_posts[i].content);
                ImGui::PopFont();
                ImGui::Text("Likes: %d  Comments: %d", user_posts[i].like_count, user_posts[i].comment_count);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));

                if(ImGui::Button((ICON_FA_HEART"##"+std::to_string(i)).c_str()))
                {
                    like_post(user_id, user_posts[i].post_id, response);
                    get_user_posts(user_id, user_posts, &user_post_count, response);
                }

                ImGui::SameLine();

                if(add_comment_profile_post[i])
                {
                    if(ImGui::Button(("Submit comment##"+std::to_string(i)).c_str()))
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
                    if(ImGui::Button((ICON_FA_COMMENT"##"+std::to_string(i)).c_str())) add_comment_profile_post[i] = true;
                }

                ImGui::SameLine();

                if (ImGui::Button((ICON_FA_SHARE_NODES "##"+std::to_string(i)).c_str())) { int ok; }
                
                ImGui::PopStyleColor();
                ImGui::Separator();
            }
            break;

        case OTHER_USERS_PROFILE:
            static bool add_comment_profile_post_other_user[MAX_POSTS] = { false };
            static char username_other_user[USERNAME_LENGTH] = "";
            static char display_name_other_user[USERNAME_LENGTH] = "";
            static char bio_other_user[BIO_LENGTH] = "";
            static int profile_visibility_other_user;
            static Post other_user_posts[MAX_POSTS];
            static int other_user_post_count = 0;
            Friend other_user_friends[MAX_FRIENDS];
            int other_user_friend_count = 0;
            if(!other_user_friends_loaded)
            {
                get_friends_list(other_user_id, other_user_friends, &other_user_friend_count, response);
                other_user_friends_loaded = true;
            }
            if(!friends_loaded)
            {
                get_friends_list(user_id, friends, &friend_count, response);
                friends_loaded = true;
            }
            if(!other_user_posts_loaded)
            {
                get_profile(other_user_id, username_other_user, bio_other_user, display_name_other_user, &profile_visibility_other_user, response);
                get_user_posts(other_user_id, other_user_posts, &other_user_post_count, response);
                other_user_posts_loaded = true;
            }

            ImGui::Text("Username: %s", username_other_user);
            ImGui::Text("Display Name: %s", display_name_other_user);
            ImGui::Text("Bio: %s", bio_other_user);

            bool is_following = false;
            bool is_close_friend = false;
            for (int i = 0; i < friend_count; i++)
            {
                if (friends[i].user_id == other_user_id)
                {
                    is_following = true;
                    if(friends[i].close_friend)
                        is_close_friend = friends[i].close_friend;
                    break;
                }
            }
            
            bool is_followed = false;
            for (int i = 0; i < other_user_friend_count; i++)
            {
                if (other_user_friends[i].user_id == user_id)
                {
                    is_followed = true;   
                    break;
                }
            }

            if (other_user_id != user_id)
            {
                if (!is_following && is_followed)
                {
                    if (ImGui::Button("Follow back"))
                    {
                        follow_request(user_id, username_other_user, response);
                    }
                }
                else if (!is_following && !is_followed)
                {
                    if (ImGui::Button("Follow"))
                    {
                        follow_request(user_id, username_other_user, response);
                    }
                }
                else if (is_following)
                {
                    if(is_close_friend)
                    {
                        if(ImGui::Button("Remove Close Friend"))
                        {
                            remove_close_friend(user_id, other_user_id, response);
                            friends_loaded=false;
                        }
                    }
                    else
                    {
                        if(ImGui::Button("Make Close Friend"))
                        {
                            add_close_friend(user_id, other_user_id, response);
                            friends_loaded=false;
                        }
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Unfollow"))
                    {
                        unfollow_request(user_id, username_other_user, response);
                    }
                }
            }
            ImGui::Separator();
            if(profile_visibility_other_user == 1 && !is_following)
            {
                ImGui::Text("This profile is private.");
                break;
            }
            ImGui::Text("Your Posts:");
            static char comment_buffer_profile_other_user[MAX_COMMENTS][COMMENT_LENGTH] = { "" };
            for(int i = 0; i < other_user_post_count; i++)
            {
                ImGui::PushFont(customFontBig);
                ImGui::Text("%s", other_user_posts[i].content);
                ImGui::PopFont();
                ImGui::Text("Likes: %d  Comments: %d", other_user_posts[i].like_count, other_user_posts[i].comment_count);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));

                if(ImGui::Button((ICON_FA_HEART"##"+std::to_string(i)).c_str()))
                {
                    like_post(other_user_id, other_user_posts[i].post_id, response);
                    get_user_posts(other_user_id, other_user_posts, &other_user_post_count, response);
                }

                ImGui::SameLine();

                if(add_comment_profile_post_other_user[i])
                {
                    if(ImGui::Button(("Submit comment##"+std::to_string(i)).c_str()))
                    {
                        if(strcmp(comment_buffer_profile_other_user[i], "") != 0)
                        {
                            comment_post(other_user_id, other_user_posts[i].post_id, comment_buffer_profile_other_user[i], response);
                            get_user_posts(other_user_id, other_user_posts, &other_user_post_count, response);
                        }
                        add_comment_profile_post_other_user[i] = false;
                    }
                }
                else
                {
                    if(ImGui::Button((ICON_FA_COMMENT"##"+std::to_string(i)).c_str())) add_comment_profile_post_other_user[i] = true;
                }

                ImGui::SameLine();

                if (ImGui::Button((ICON_FA_SHARE_NODES "##"+std::to_string(i)).c_str())) { int ok; }

                ImGui::PopStyleColor();
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