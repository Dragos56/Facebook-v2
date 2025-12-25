#pragma once

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <pthread.h>

#define USERNAME_LENGTH 100
#define PASSWORD_LENGTH 100
#define MESSAGE_LENGTH 1000
#define BIO_LENGTH 1000
#define VISIBILITY_LENGTH 10
#define MAX_POSTS 100
#define MAX_FRIENDS 100
#define MAX_LIKES 100
#define MAX_COMMENTS 100
#define MAX_FOLLOW_REQUESTS 100
#define COMMENT_LENGTH 256

typedef struct {
    int user_id;
    char display_name[USERNAME_LENGTH];
} Like;

typedef struct {
    int user_id;
    char display_name[USERNAME_LENGTH];
    char comment[COMMENT_LENGTH];
} Comment;

typedef struct{
    int request_id;
    char display_name[USERNAME_LENGTH];
} Request;

typedef struct {
    int user_id;
    char display_name[USERNAME_LENGTH];
    int close_friend;
} Friend;

typedef struct {
    int post_id;
    int user_id;
    char content[MESSAGE_LENGTH];
    int visibility;
    char display_name[USERNAME_LENGTH];

    Like likes[MAX_LIKES];
    int like_count;

    Comment comments[MAX_COMMENTS];
    int comment_count;
} Post;

int db_init(const char* filename);
void db_close();

int db_user_exists(const char* username);
int db_add_user(const char* username, const char* password_hash);
int db_check_login(const char* username, const char* password_hash, int* user_id);

int db_is_online(int user_id, int is_online);
int db_update_profile_bio(int user_id, const char* bio);
int db_update_profile_display_name(int user_id, const char* display_name);
int db_update_profile_visibility(int user_id, int visibility);
int db_update_password(int user_id, const char* old_password_hash, const char* new_password_hash);
int db_delete_user(int user_id);

int db_follow_user(int user_id, const char* username_to_follow);
int db_accept_follow_request(int user_id, const char* username_to_accept);
int db_reject_follow_request(int user_id, const char* username_to_reject);
int db_unfollow_user(int user_id, const char* username_to_unfollow);
int db_add_close_friend(int user_id, int friend_id);
int db_remove_close_friend(int user_id, int friend_id);

int db_create_post(int user_id, const char* content, int visibility, int* post_id);
int db_edit_post_content(int user_id, int post_id, const char* new_content);
int db_edit_post_visibility(int user_id, int post_id, int new_visibility);
int db_delete_post(int user_id, int post_id);
int db_like_post(int user_id, int post_id);
int db_comment_post(int user_id, int post_id, const char* comment);

int db_get_profile(int user_id, char* username, char* bio, char* display_name, int* visibility);
int db_get_friends_list(int user_id, Friend* friends, int max_friends, int* friend_count);
int db_get_follow_requests(int user_id, Request* requests, int max_requests, int* request_count);
int db_get_user_posts(int user_id, Post* posts, int max_posts, int* post_count);
int db_get_feed(int user_id, Post* posts, int max_posts, int* post_count);
int db_get_post_likes(int post_id, Like* likes, int max_likes, int* like_count);
int db_get_post_comments(int post_id, Comment* comments, int max_comments, int* comment_count);
int db_get_username_by_id(int user_id, char* username);

int db_search_user(const char* display_name, int* user_id);

