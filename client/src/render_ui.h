#pragma once

#include "imgui.h"
#include "messages.h"

#define USERNAME_LENGTH 100
#define PASSWORD_LENGTH 100
#define MESSAGE_LENGTH 1000
#define BIO_LENGTH 1000
#define PATH_LENGTH 100
#define VISIBILITY_LENGTH 10

enum STATE
{
    LOGGED_OUT,
    LOGGED_IN,
    FRIENDS,
    POST,
    NOTIFICATIONS,
    PROFILE
};

void render_login_window(float total_width, float total_height);
void render_register_window(float total_width, float total_height);
void render_friends_ui();
void render_post_ui();
void render_notifications_ui();
void render_profile_ui();
void render_loggedOut_ui();
void render_loggedIn_ui();
void render_ui();