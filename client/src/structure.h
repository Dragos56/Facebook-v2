#pragma once

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

enum STATE
{
    LOGGED_OUT,
    LOGGED_IN,
    SEARCH,
    FRIENDS,
    POST,
    OTHER_USERS_PROFILE,
    PROFILE
};

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