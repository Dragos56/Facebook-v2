#pragma once

#include "imgui.h"
#include "messages.h"

#define USERNAME_LENTH 100
#define PASSWORD_LENGTH 100
#define MESSAGE_LENGTH 1000

void render_login_window(float total_width, float total_height);
void render_register_window(float total_width, float total_height);
void render_loggedOut_ui();
void render_loggedIn_ui();