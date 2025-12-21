#pragma once

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <pthread.h>

int db_init(const char* filename);
void db_close();

int db_user_exists(const char* username);
int db_add_user(const char* username, const char* password_hash);
int db_check_login(const char* username, const char* password_hash, int* user_id);

int db_update_profile_bio(int user_id, const char* bio);
int db_update_profile_display_name(int user_id, const char* display_name);
int db_update_profile_avatar_path(int user_id, const char* avatar_path);
int db_update_profile_visibility(int user_id, int visibility);
int db_update_password(int user_id, const char* old_password_hash, const char* new_password_hash);
int db_is_online(int user_id, int is_online);
int db_delete_user(int user_id);

int db_follow_user(int user_id, const char* username_to_follow);
int db_accept_follow_request(int user_id, const char* username_to_accept);
int db_reject_follow_request(int user_id, const char* username_to_reject);
int db_unfollow_user(int user_id, const char* username_to_unfollow);

int db_create_post(int user_id, const char* description, const char* image_path, int visibility, int* post_id);
int db_edit_post_description(int user_id, int post_id, const char* new_description);
int db_edit_post_visibility(int user_id, int post_id, int new_visibility);
int db_delete_post(int user_id, int post_id);
int db_like_post(int user_id, int post_id);
int db_comment_post(int user_id, int post_id, const char* comment);

int db_get_profile(int user_id, char* username, char* bio, char* display_name, char* avatar_path, int* visibility);

int db_get_friends_list(int user_id, int* friend_ids, int max_friends, int* friend_count);
int db_get_follow_requests(int user_id, int* request_ids, int max_requests, int* request_count);

typedef struct {
    int post_id;
    int user_id;
    char username[50];
    char description[512];
    char image_path[256];
    int visibility;
    int like_count;
    int comment_count;
} Post;

int db_get_user_posts(int user_id, Post* posts, int max_posts, int* post_count);
int db_get_feed(int user_id, Post* posts, int max_posts, int* post_count);
int db_get_post_likes(int post_id, int* user_ids, int max_likes, int* like_count);
int db_get_post_comments(int post_id, int* user_ids, char comments[][256], int max_comments, int* comment_count);



