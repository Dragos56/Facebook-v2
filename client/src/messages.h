#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

#define MESSAGE_LENGTH 1000

void register_account(const char* username, const char* password, char* response);
void login_account(const char* username, const char* password, char* response);
void logout_account(int* user_id, char* response);
void delete_account(int* user_id, char* response);
void update_display_name(int user_id, const char* display_username, char* response);
void update_bio(int user_id, const char* new_bio, char* response);
void update_password(int user_id, const char* old_password, const char* new_password, char* response);
void update_profile_avatar(int user_id, const char* image_path, char* response);
void update_profile_visibility(int user_id, int visibility, char* response);
void follow_request(int user_id, const char* username_to_follow, char* response);
void accept_follow_request(int user_id, const char* username_to_accept, char* response);
void reject_follow_request(int user_id, const char* username_to_reject, char* response);
void unfollow_request(int user_id, const char* username_to_unfollow, char* response);
void post(int user_id, const char* description, const char* image_path, int visibility, int post_id, char* response);
void edit_post_visibility(int user_id, int post_id, int visibility, char* response);
void edit_post_description(int user_id, int post_id, const char* description, char* response);
void like_post(int user_id, int post_id, char* response);
void comment_post(int user_id, int post_id, const char* comment, char* response);
void delete_post(int user_id, int post_id, char* response);
void send_message_friend(int user_id, const char* friend_name, const char* message, char* response);
