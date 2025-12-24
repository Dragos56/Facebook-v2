#pragma once

#include "imgui.h"
#include "messages.h"
#include "structure.h"
#include "Icons.h"

// Functii generale
void client_shutdown();

void load_font();
// Ferestre specifice
void render_login_window(float total_width, float total_height);
void render_register_window(float total_width, float total_height);

// Pagini UI
void render_friends_ui();
void render_post_ui();
void render_notifications_ui();
void render_profile_ui();

// Layout-uri generale
void render_loggedOut_ui();
void render_loggedIn_ui();

// Functie principala de render
void render_ui(float window_width, float window_height);
