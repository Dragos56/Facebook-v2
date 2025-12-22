#pragma once

#define USERNAME_LENGTH 100
#define PASSWORD_LENGTH 100
#define MESSAGE_LENGTH 1000
#define BIO_LENGTH 1000
#define PATH_LENGTH 100
#define VISIBILITY_LENGTH 10
#define MAX_POSTS 100
#define MAX_FRIENDS 100
#define MAX_LIKES 100
#define MAX_COMMENTS 100
#define COMMENT_LENGTH 256

enum STATE
{
    LOGGED_OUT,
    LOGGED_IN,
    FRIENDS,
    POST,
    NOTIFICATIONS,
    PROFILE
};

typedef struct {
    int post_id;
    int user_ids[MAX_LIKES];
    int like_count;
} Likes;

typedef struct {
    int post_id;
    int user_ids[MAX_COMMENTS];
    char comments[MAX_COMMENTS][COMMENT_LENGTH];
    int comment_count;
} Comments;

typedef struct{
    int request_id;
    char username[USERNAME_LENGTH];
} Request;

typedef struct {
    int user_id;
    char username[USERNAME_LENGTH];
} Friend;

typedef struct {
    int post_id;
    int user_id;
    char description[MESSAGE_LENGTH];
    int visibility;
    char image_path[PATH_LENGTH];
    char username[USERNAME_LENGTH];
    
    int like_count;
    int likes[MAX_LIKES];

    int comment_count;
    int comment_user_ids[MAX_COMMENTS];
    char comments[MAX_COMMENTS][COMMENT_LENGTH];
} Post;