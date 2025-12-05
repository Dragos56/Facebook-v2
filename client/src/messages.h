#pragma once

#include "network.h"

#define MESSAGE_LENGTH 1000

void register_account(const char* username, const char* password, char* response);
void login_account(const char* username, const char* password, char* response);
void logout_account(const char* username, char* response);
void delete_account(const char* username, char* response);
void update_name(const char* old_username, const char* new_username, char* response);
void update_bio(const char* username, const char* new_bio, char* response);
void update_password(const char* username, const char* old_password, const char* new_password, char* response);
void update_profile_picture(const char* username, const char* image_path, char* response);
void update_profile_visibility(const char* username, const char* visibility, char* response);
void follow_request(const char* username, const char* username_to_follow, char* response);
void accept_follow_request(const char* username, const char* username_to_accept, char* response);
void reject_follow_request(const char* username, const char* username_to_reject, char* response);
void unfollow_request(const char* username, const char* username_to_unfollow, char* response);
void post(const char* author, const char* description, const char* image_path, const char* visibility, int post_id, char* response);
void edit_post_visibility(const char* author, const char* visibility, int post_id, char* response);
void edit_post_description(const char* author, const char* description, int post_id, char* response);
void like_post(const char* username, const char* author, int post_id, char* response);
void comment_post(const char* username, const char* author, int post_id, const char* comment, char* response);
void delete_post(const char* author, int post_id, char* response);
void send_message(const char* username, const char* friend_name, const char* message, char* response);

