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

static Group groups[MAX_GROUPS];
static int groups_count = 0;
static bool groups_loaded = false;

static char connected_username[USERNAME_LENGTH] = "";
static int user_id = -1;
static int other_user_id = -1;
static int friend_chat_id = -1;
static int group_chat_id = -1;

static bool showLogin = false;
static bool showRegister = false;

static Post* main_post;
static bool showPost = false;

static bool user_posts_loaded = false;
static bool friends_loaded = false;
static bool follow_request_loaded = false;
static bool other_user_posts_loaded = false;
static bool other_user_friends_loaded = false;

static char response[MESSAGE_LENGTH];

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

    customFontBig = io.Fonts->AddFontFromFileTTF("assets/font.ttf", 26.0f);
}

void client_shutdown()
{
    if (user_id != -1)
    {
        char response[MESSAGE_LENGTH];
        logout_account(&user_id, response);
    }
}

void render_posts_window(Post* main_post, float total_width, float total_height)
{
    ImVec2 popup_size = ImVec2(total_width * 0.6f, total_height * 0.7f);
    ImVec2 window_size = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(
        (window_size.x - popup_size.x) * 0.5f,
        (window_size.y - popup_size.y) * 0.5f
    ), ImGuiCond_Appearing);

    ImGui::SetNextWindowSize(ImVec2(total_width * 0.6f, total_height * 0.7f));
    ImGui::OpenPopup("Post");

    if (ImGui::BeginPopupModal("Post", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
    {
        Post post = *main_post;
        static bool is_like_loaded = false;
        static bool is_commment_loaded = false;
        static Comment comments[MAX_COMMENTS];
        static int comments_count;
        static Like likes[MAX_LIKES];
        static int likes_count;
        static float comments_timer = 0.0f;

        float delta = ImGui::GetIO().DeltaTime;

        comments_timer += delta;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        ImGui::SameLine(ImGui::GetWindowWidth() - 50);
        if (ImGui::Button(ICON_FA_XMARK))
        {
            showPost = false;
            ImGui::CloseCurrentPopup();
        }

        if (!is_commment_loaded || comments_timer >= 2.0f)
        {
            comments_timer = 0.0f;
            get_post_comments(post.post_id, comments, &comments_count, response);
            is_commment_loaded = true;
        }
        if (!is_like_loaded)
        {
            get_post_likes(post.post_id, likes, &likes_count, response);
            is_like_loaded = true;
        }

        ImGui::Text("Post by %s:", post.display_name);
        ImGui::PushFont(customFontBig);
        ImGui::Text("%s", post.content);
        ImGui::PopFont();
        ImGui::Text("Likes: %d  Comments: %d", post.like_count, post.comment_count);

        if (ImGui::Button((ICON_FA_HEART "##")))
        {
            like_post(user_id, post.post_id, response);
            feed_loaded = false;
        }

        ImGui::SameLine();

        static char comment[MESSAGE_LENGTH];
        ImGui::InputText("##comment", comment, MESSAGE_LENGTH);

        ImGui::SameLine();

        if (ImGui::Button(("Submit comment##")))
        {
            if (strcmp(comment, "") != 0)
            {
                comment_post(user_id, post.post_id, comment, response);
                feed_loaded = false;
                is_commment_loaded = false;
                post.comment_count++;
                comment[0] = '\0';
            }
        }

        ImGui::SameLine();

        if (ImGui::Button((ICON_FA_SHARE_NODES "##")))
        {
            feed_loaded = false;
        }

        ImGui::PopStyleColor();

        ImGui::Separator();

        for (int i = 0; i < comments_count; i++)
        {
            ImGui::Text("Comment by %s\n", comments[i].display_name);
            ImGui::PushFont(customFontBig);
            ImGui::Text("%s", comments[i].comment);
            ImGui::PopFont();

            ImGui::Separator();
        }

        ImGui::EndPopup();
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
        {
            ImGui::Text("%s", error_message);
        }

        if (ImGui::Button("Login"))
        {
            if (strlen(username) == 0 || strlen(password) == 0)
            {
                strcpy(error_message, "Username and password cannot be empty.");
                has_error = true;
            }
            else
            {
                login_account(username, password, response);

                if (strncmp(response, "LOGIN OK", 8) == 0)
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

        if (!showLogin)
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
        {
            ImGui::Text("%s", error_message);
        }

        if (ImGui::Button("Register"))
        {
            if (strlen(username) == 0 || strlen(password) == 0)
            {
                strcpy(error_message, "Username and password cannot be empty.");
                has_error = true;
            }
            else
            {
                register_account(username, password, response);

                if (strncmp(response, "LOGIN OK", 8) == 0)
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
                    if (strncmp(response, "REGISTER ERROR|User already exists", 34) == 0)
                    {
                        strcpy(error_message, "User already exists.");
                    }
                    else
                    {
                        strcpy(error_message, "Registration failed. Please try again.");
                    }
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

        if (!showRegister)
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
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(w, top_h));
    ImGui::Begin("TopPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    if (app_status == LOGGED_OUT)
    {
        if (ImGui::Button("Login"))
        {
            showLogin = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Register"))
        {
            showRegister = true;
        }
    }
    else
    {
        ImGui::Text("Logged in as: %s", connected_username);
        if (ImGui::Button("Logout"))
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

    if (user_id != -1)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        if (ImGui::Button(ICON_FA_HOUSE " Home", ImVec2(bw, 60)))
        {
            app_status = LOGGED_IN;
            feed_loaded = false;
        }

        if (ImGui::Button(ICON_FA_MAGNIFYING_GLASS " Search", ImVec2(bw, 60)))
        {
            app_status = SEARCH;
            follow_request_loaded = false;
        }

        if (ImGui::Button(ICON_FA_USERS " Friends", ImVec2(bw, 60)))
        {
            app_status = FRIENDS;
            friends_loaded = false;
            friend_chat_id = -1;
            group_chat_id = -1;
        }

        if (ImGui::Button(ICON_FA_SQUARE_PLUS " Post", ImVec2(bw, 60)))
        {
            app_status = POST;
        }

        if (ImGui::Button(ICON_FA_USER " Profile", ImVec2(bw, 60)))
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

    static float friends_timer = 0.0f;
    static float groups_timer = 0.0f;

    float delta = ImGui::GetIO().DeltaTime;

    friends_timer += delta;
    groups_timer += delta;

    ImGui::SetNextWindowPos(ImVec2(w - right_w, top_h));
    ImGui::SetNextWindowSize(ImVec2(right_w, h - top_h));
    ImGui::Begin("RightPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (app_status == FRIENDS || app_status == PRIVATE_CHAT || app_status == GROUP_CHAT)
    {
        if (!friends_loaded || friends_timer >= 5.0f)
        {
            friends_timer = 0.0f;
            get_friends_list(user_id, friends, &friend_count, response);
            friends_loaded = true;
        }

        if (!groups_loaded || groups_timer >= 5.0f)
        {
            groups_timer = 0.0f;
            get_groups(user_id, groups, &groups_count, response);
            groups_loaded = true;
        }

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        float button_width = ImGui::GetContentRegionAvail().x;
        float button_height = 40.0f;

        ImGui::Text("Close friends:");
        for (int i = 0; i < friend_count; i++)
        {
            if (friends[i].close_friend)
            {
                if (ImGui::Button(friends[i].display_name, ImVec2(button_width, button_height)))
                {
                    ImGui::OpenPopup(("friend##" + std::to_string(i)).c_str());
                }

                if (ImGui::BeginPopup(("friend##" + std::to_string(i)).c_str()))
                {
                    if (ImGui::Button("View Profile"))
                    {
                        other_user_id = friends[i].user_id;
                        other_user_posts_loaded = false;
                        other_user_friends_loaded = false;
                        app_status = OTHER_USERS_PROFILE;
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::Button("Send message"))
                    {
                        friend_chat_id = friends[i].user_id;
                        group_chat_id = -1;
                        friends_loaded = false;
                        app_status = PRIVATE_CHAT;
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::Button("Add to group"))
                    {
                        ImGui::OpenPopup(("group_select_popup##" + std::to_string(i)).c_str());
                    }

                    if (ImGui::BeginPopup(("group_select_popup##" + std::to_string(i)).c_str()))
                    {
                        if (groups_count == 0)
                        {
                            ImGui::Text("No groups available.");
                        }
                        else
                        {
                            for (int j = 0; j < groups_count; j++)
                            {
                                if (ImGui::Button(groups[j].name, ImVec2(button_width, button_height)))
                                {
                                    groups_loaded = false;
                                    add_user_group(groups[j].id, friends[i].user_id, response);
                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }
                        ImGui::EndPopup();
                    }

                    ImGui::EndPopup();
                }

                ImGui::Separator();
            }
        }
        ImGui::Text("Friends:");
        for (int i = 0; i < friend_count; i++)
        {
            if (!friends[i].close_friend)
            {
                if (ImGui::Button(friends[i].display_name, ImVec2(button_width, button_height)))
                {
                    ImGui::OpenPopup(("friend##" + std::to_string(i)).c_str());
                }

                if (ImGui::BeginPopup(("friend##" + std::to_string(i)).c_str()))
                {
                    if (ImGui::Button("View Profile"))
                    {
                        other_user_id = friends[i].user_id;
                        other_user_posts_loaded = false;
                        other_user_friends_loaded = false;
                        app_status = OTHER_USERS_PROFILE;
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::Button("Send message"))
                    {
                        friend_chat_id = friends[i].user_id;
                        group_chat_id = -1;
                        friends_loaded = false;
                        app_status = PRIVATE_CHAT;
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::Button("Add to group"))
                    {
                        ImGui::OpenPopup(("group_select_popup##" + std::to_string(i)).c_str());
                    }

                    if (ImGui::BeginPopup(("group_select_popup##" + std::to_string(i)).c_str()))
                    {
                        if (groups_count == 0)
                        {
                            ImGui::Text("No groups available.");
                        }
                        else
                        {
                            for (int j = 0; j < groups_count; j++)
                            {
                                if (ImGui::Button(groups[j].name, ImVec2(button_width, button_height)))
                                {
                                    groups_loaded = false;
                                    add_user_group(groups[j].id, friends[i].user_id, response);
                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }
                        ImGui::EndPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::Separator();
            }
        }

        ImGui::Text("Groups:");

        static char group_name[USERNAME_LENGTH];
        ImGui::InputText("name", group_name, USERNAME_LENGTH);

        if (ImGui::Button("Create group", ImVec2(button_width, button_height)))
        {
            if (strlen(group_name) > 0)
            {
                groups_loaded = false;
                create_group(group_name, user_id, response);
                group_name[0] = '\0';
            }
        }

        for (int i = 0; i < groups_count; i++)
        {
            if (ImGui::Button(groups[i].name, ImVec2(button_width, button_height)))
            {
                friend_chat_id = -1;
                group_chat_id = groups[i].id;
                groups_loaded = false;
                app_status = GROUP_CHAT;
            }
            ImGui::Separator();
        }

        ImGui::PopStyleColor();
    }

    if (app_status == SEARCH)
    {
        ImGui::Text("Follow Requests:");
        if (!follow_request_loaded || friends_timer >= 2.0f)
        {
            friends_timer = 0.0f;
            get_follow_requests(user_id, follow_requests, &follow_request_count, response);
            follow_request_loaded = true;
        }
        for (int i = 0; i < follow_request_count; i++)
        {
            ImGui::Text("%s", follow_requests[i].display_name);
            ImGui::NewLine();
            if (ImGui::Button(("Accept##" + std::to_string(i)).c_str()))
            {
                accept_follow_request(user_id, follow_requests[i].display_name, response);
                follow_request_loaded = false;
            }
            ImGui::SameLine();
            if (ImGui::Button(("Reject##" + std::to_string(i)).c_str()))
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
    static float chat_timer = 0.0f;
    static float feed_timer = 0.0f;
    static float friends_timer = 0.0f;
    static float other_friends_timer = 0.0f;
    static float profile_timer = 0.0f;

    float delta = ImGui::GetIO().DeltaTime;

    chat_timer += delta;
    feed_timer += delta;
    friends_timer += delta;
    other_friends_timer += delta;
    profile_timer += delta;

    float top_h = h * 0.12f;
    float left_w = w * 0.18f;
    float right_w = w * 0.18f;

    ImGui::SetNextWindowPos(ImVec2(left_w, top_h));
    ImGui::SetNextWindowSize(ImVec2(w - left_w - right_w, h - top_h));
    ImGui::Begin("MainPanel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    switch (app_status)
    {
    case LOGGED_OUT:
        if (!feed_loaded || feed_timer >= 5.0f)
        {
            feed_timer = 0.0f;

            get_feed(user_id, feed, &feed_count, response);
            feed_loaded = true;
        }
        for (int i = 0; i < feed_count; i++)
        {
            if (feed[i].visibility == 1 || feed[i].visibility == 2)
            {
                continue;
            }
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
        if (!feed_loaded || (feed_timer >= 4.0f && showPost == false) || (feed_timer >= 15.0f && showPost == true))
        {
            feed_timer = 0.0f;
            get_feed(user_id, feed, &feed_count, response);
            feed_loaded = true;
        }

        for (int i = 0; i < feed_count; i++)
        {
            if (feed[i].visibility == 1)
            {
                bool is_friend = false;
                for (int j = 0; j < friend_count; j++)
                {
                    if (friends[j].user_id == feed[i].user_id)
                    {
                        is_friend = true;
                        break;
                    }
                }
                if (!is_friend)
                {
                    continue;
                }
            }
            else if (feed[i].visibility == 2)
            {
                bool is_close_friend = false;
                for (int j = 0; j < friend_count; j++)
                {
                    if (friends[j].user_id == feed[i].user_id && friends[j].close_friend)
                    {
                        is_close_friend = true;
                        break;
                    }
                }
                if (!is_close_friend)
                {
                    continue;
                }
            }
            ImGui::Text("Post by %s:", feed[i].display_name);
            ImGui::PushFont(customFontBig);
            ImGui::Text("%s", feed[i].content);
            ImGui::PopFont();
            ImGui::Text("Likes: %d  Comments: %d", feed[i].like_count, feed[i].comment_count);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

            if (ImGui::Button((ICON_FA_HEART "##" + std::to_string(i)).c_str()))
            {
                like_post(user_id, feed[i].post_id, response);
                feed_loaded = false;
            }

            ImGui::SameLine();

            if (ImGui::Button((ICON_FA_COMMENT "##" + std::to_string(i)).c_str()))
            {
                showPost = true;
                main_post = &feed[i];
            }

            ImGui::SameLine();

            if (ImGui::Button((ICON_FA_SHARE_NODES "##" + std::to_string(i)).c_str()))
            {
                feed_loaded = false;
            }

            ImGui::PopStyleColor();
            ImGui::Separator();
        }
        break;

    case SEARCH:
        static char follow_username[USERNAME_LENGTH] = "";
        static bool found = true;
        
        ImGui::Text("Search:");
        ImGui::InputText("Username", follow_username, USERNAME_LENGTH);
        if (ImGui::Button("Search"))
        {
            if (strlen(follow_username) > 0)
            {
                search_user(follow_username, &other_user_id, response);
                if(strcmp(response, "SEARCH_USER ERROR|Database error")==0)
                {
                    found = false;
                }
                else
                {
                    app_status = OTHER_USERS_PROFILE;
                    follow_username[0] = '\0';
                    other_user_posts_loaded = false;
                    other_user_friends_loaded = false;
                    friends_loaded = false;
                    found = true;
                }
            }
            else
                found = true;
        }
        if(found == false)
        {
            ImGui::Text("User not found!");
        }
        ImGui::Separator();
        break;
        
    case FRIENDS:{
        ImGui::Text("Chat with your friends or choose a group!");
        ImGui::Separator();

        if (friend_chat_id == -1 && group_chat_id == -1)
        {
            ImGui::Text("Select a friend or a group from the right panel to start chatting.");
            break;
        }
        break;
    }

    case PRIVATE_CHAT:{
        ImGui::Text("Chat with your friends!");
        ImGui::Separator();

        if (friend_chat_id == -1)
        {
            ImGui::Text("Select a friend from the right panel to start chatting.");
            break;
        }

        static PrivateMessage private_messages[MAX_MESSAGES];
        static int private_messages_count = 0;

        static float private_chat_timer = 0.0f;
        private_chat_timer += delta;

        if (friend_chat_id != -1 && (!friends_loaded || private_chat_timer >= 0.5f))
        {
            private_chat_timer = 0.0f;
            get_private_messages(user_id, friend_chat_id, private_messages, &private_messages_count, response);
            friends_loaded = true;
        }

        ImGui::BeginChild("MessagesScroll", ImVec2(0, h * 0.7f), true, ImGuiWindowFlags_HorizontalScrollbar);

        for (int i = 0; i < private_messages_count; i++)
        {
            if (private_messages[i].sender_id == user_id)
            {
                ImGui::Text("%s: %s", connected_username, private_messages[i].content);
            }
            else
            {
                ImGui::Text("%s: %s", private_messages[i].sender_display_name, private_messages[i].content);
            }
        }

        if (private_messages_count > 0)
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();

        static char chat_input[MESSAGE_LENGTH] = "";
        ImGui::InputText("##chat_input", chat_input, MESSAGE_LENGTH);
        ImGui::SameLine();
        if (ImGui::Button("Send") && strlen(chat_input) > 0)
        {
            if (friend_chat_id != -1)
            {
                send_private_message(user_id, friend_chat_id, chat_input, response);
                friends_loaded = false;
            }

            chat_input[0] = '\0';
        }
        break;
    }
    case GROUP_CHAT:{
        ImGui::Text("Chat with your groups!");
        ImGui::Separator();

        if (group_chat_id == -1)
        {
            ImGui::Text("Select a group from the right panel to start chatting.");
            break;
        }

        static GroupMessage group_messages[MAX_MESSAGES];
        static int group_messages_count = 0;

        static float group_chat_timer = 0.0f;
        group_chat_timer += delta;

        if (group_chat_id != -1 && (!groups_loaded || group_chat_timer >= 0.5f))
        {
            group_chat_timer = 0.0f;
            get_group_messages(group_chat_id, group_messages, &group_messages_count, response);
            groups_loaded = true;
        }

        ImGui::BeginChild("MessagesScroll", ImVec2(0, h * 0.7f), true, ImGuiWindowFlags_HorizontalScrollbar);

        for (int i = 0; i < group_messages_count; i++)
        {
            if (group_messages[i].sender_id == user_id)
            {
                ImGui::Text("%s: %s", connected_username, group_messages[i].content);
            }
            else
            {
                ImGui::Text("%s: %s", group_messages[i].sender_display_name, group_messages[i].content);
            }
        }

        if (group_messages_count > 0)
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();

        static char chat_input[MESSAGE_LENGTH] = "";
        ImGui::InputText("##chat_input", chat_input, MESSAGE_LENGTH);
        ImGui::SameLine();
        if (ImGui::Button("Send") && strlen(chat_input) > 0)
        {
            if (group_chat_id != -1)
            {
                send_group_message(group_chat_id, user_id, chat_input, response);
                groups_loaded = false;
                
            }

            chat_input[0] = '\0';
        }
        break;
    }

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

        if (ImGui::Button("Submit Post"))
        {
            create_post(user_id, content, post_visibility, response);
            content[0] = '\0';
        }
        break;

    case PROFILE:
        ImGui::Text("User Profile:");
        ImGui::Separator();

        static bool edit_mode = false;
        static char username[USERNAME_LENGTH] = "";
        static char display_name[USERNAME_LENGTH] = "";
        static char bio[BIO_LENGTH] = "";
        static int profile_visibility;
        static Post user_posts[MAX_POSTS];
        static int user_post_count = 0;

        if (!user_posts_loaded || (profile_timer >= 5.0f && showPost == false) || (profile_timer >= 20.0f && showPost == true))
        {
            profile_timer = 0.0f;

            get_profile(user_id, username, bio, display_name, &profile_visibility, response);
            get_user_posts(user_id, user_posts, &user_post_count, response);
            user_posts_loaded = true;
        }

        ImGui::Text("Username: %s", username);
        ImGui::Text("Display Name: %s", display_name);
        ImGui::Text("Bio: %s", bio);
        ImGui::Text("Visibility: %d", profile_visibility);
        if (edit_mode)
        {
            ImGui::InputText("Display Name", display_name, USERNAME_LENGTH);
            ImGui::InputTextMultiline("Bio", bio, BIO_LENGTH);

            ImGui::Text("Visibility:");
            ImGui::SameLine();
            ImGui::RadioButton("Public", &profile_visibility, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Private", &profile_visibility, 1);

            if (ImGui::Button("Save Changes"))
            {
                if (strlen(display_name) > 0)
                {
                    update_display_name(user_id, display_name, response);
                }
                if (strlen(bio) > 0)
                {
                    update_bio(user_id, bio, response);
                }
                update_profile_visibility(user_id, profile_visibility, response);
                edit_mode = false;
                user_posts_loaded = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                edit_mode = false;
            }
        }
        else
        {
            if (ImGui::Button("Edit Profile"))
            {
                edit_mode = true;
            }
        }
        ImGui::Separator();
        ImGui::Text("Your Posts:");

        for (int i = 0; i < user_post_count; i++)
        {
            ImGui::PushFont(customFontBig);
            ImGui::Text("%s", user_posts[i].content);
            ImGui::PopFont();
            ImGui::Text("Likes: %d  Comments: %d", user_posts[i].like_count, user_posts[i].comment_count);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

            if (ImGui::Button((ICON_FA_HEART "##" + std::to_string(i)).c_str()))
            {
                like_post(user_id, user_posts[i].post_id, response);
                user_posts_loaded = false;
            }

            ImGui::SameLine();

            if (ImGui::Button((ICON_FA_COMMENT "##" + std::to_string(i)).c_str()))
            {
                showPost = true;
                main_post = &user_posts[i];
                user_posts_loaded = false;
            }

            ImGui::SameLine();

            if (ImGui::Button((ICON_FA_SHARE_NODES "##" + std::to_string(i)).c_str()))
            {
                user_posts_loaded = false;
            }

            ImGui::PopStyleColor();
            ImGui::Separator();
        }
        break;

    case OTHER_USERS_PROFILE:

        static char username_other_user[USERNAME_LENGTH] = "";
        static char display_name_other_user[USERNAME_LENGTH] = "";
        static char bio_other_user[BIO_LENGTH] = "";
        static int profile_visibility_other_user;

        static Post other_user_posts[MAX_POSTS];
        static int other_user_post_count = 0;

        if (!friends_loaded || friends_timer >= 2.0f)
        {
            friends_timer = 0.0f;
            get_friends_list(user_id, friends, &friend_count, response);
            friends_loaded = true;
        }

        if (!other_user_posts_loaded || profile_timer >= 2.0f)
        {
            profile_timer = 0.0f;
            get_profile(other_user_id, username_other_user, bio_other_user, display_name_other_user, &profile_visibility_other_user, response);
            get_user_posts(other_user_id, other_user_posts, &other_user_post_count, response);
            other_user_posts_loaded = true;
        }

        ImGui::Text("Username: %s", username_other_user);
        ImGui::Text("Display Name: %s", display_name_other_user);
        ImGui::Text("Bio: %s", bio_other_user);

        bool is_friend = false;
        bool is_close_friend = false;

        for (int i = 0; i < friend_count; i++)
        {
            if (friends[i].user_id == other_user_id)
            {
                is_friend = true;
                if (friends[i].close_friend)
                {
                    is_close_friend = true;
                }
                break;
            }
        }

        if (other_user_id != user_id)
        {
            if (!is_friend)
            {
                if (ImGui::Button("Follow"))
                {
                    follow_request(user_id, username_other_user, response);
                    friends_loaded = false;
                    other_user_posts_loaded = false;
                }
            }
            else if (is_friend)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                if (ImGui::Button("Following"))
                {
                    ImGui::OpenPopup("FollowingMenu");
                    friends_loaded = false;
                    other_user_posts_loaded = false;
                }
                if (ImGui::BeginPopup("FollowingMenu"))
                {
                    if (ImGui::Button("Unfollow"))
                    {
                        unfollow_request(user_id, username_other_user, response);
                        friends_loaded = false;
                        other_user_posts_loaded = false;
                        ImGui::CloseCurrentPopup();
                    }

                    if (is_close_friend)
                    {
                        if (ImGui::Button("Remove close friend"))
                        {
                            remove_close_friend(user_id, other_user_id, response);
                            friends_loaded = false;
                            other_user_posts_loaded = false;
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    else
                    {
                        if (ImGui::Button("Add close friend"))
                        {
                            add_close_friend(user_id, other_user_id, response);
                            friends_loaded = false;
                            other_user_posts_loaded = false;
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndPopup();
                }
                ImGui::PopStyleColor();
            }
        }
        ImGui::Separator();
        if (profile_visibility_other_user == 1 && !is_friend)
        {
            ImGui::Text("This profile is private.");
            break;
        }
        ImGui::Text("Your Posts:");

        for (int i = 0; i < other_user_post_count; i++)
        {
            if (other_user_posts[i].visibility == 1 && !is_friend)
            {
                continue;
            }

            if (other_user_posts[i].visibility == 2 && !is_close_friend)
            {
                continue;
            }

            ImGui::PushFont(customFontBig);
            ImGui::Text("%s", other_user_posts[i].content);
            ImGui::PopFont();
            ImGui::Text("Likes: %d  Comments: %d", other_user_posts[i].like_count, other_user_posts[i].comment_count);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

            if (ImGui::Button((ICON_FA_HEART "##" + std::to_string(i)).c_str()))
            {
                like_post(other_user_id, other_user_posts[i].post_id, response);
                friends_loaded = false;
                other_user_posts_loaded = false;
            }

            ImGui::SameLine();

            if (ImGui::Button((ICON_FA_COMMENT "##" + std::to_string(i)).c_str()))
            {
                showPost = true;
                main_post = &other_user_posts[i];
                friends_loaded = false;
                other_user_posts_loaded = false;
            }

            ImGui::SameLine();

            if (ImGui::Button((ICON_FA_SHARE_NODES "##" + std::to_string(i)).c_str()))
            {
                friends_loaded = false;
                other_user_posts_loaded = false;
            }

            ImGui::PopStyleColor();
            ImGui::Separator();
        }
        break;
    }
    ImGui::End();
}

void render_ui(float w, float h)
{
    render_top_panel(w, h);
    render_left_panel(w, h);
    render_main_panel(w, h);
    render_right_panel(w, h);

    if (showLogin)
    {
        render_login_window(w, h);
    }
    if (showRegister)
    {
        render_register_window(w, h);
    }
    if (showPost)
    {
        render_posts_window(main_post, w, h);
    }
}