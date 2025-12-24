#pragma once

#include "imgui.h"
#include "messages.h"
#include "structure.h"
#include "Icons.h"

void client_shutdown();

void load_font();

void render_login_window(float total_width, float total_height);
void render_register_window(float total_width, float total_height);

void render_friends_ui();
void render_post_ui();
void render_notifications_ui();
void render_profile_ui();

void render_loggedOut_ui();
void render_loggedIn_ui();

void render_ui(float window_width, float window_height);
