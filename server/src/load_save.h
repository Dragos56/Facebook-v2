#pragma once

#define MAX_STR 256
#define MAX_POSTS 100
#define MAX_FRIENDS 200
#define MAX_COMMENTS 200

struct Comment {
    char author[MAX_STR];
    char text[MAX_STR];
};

struct Post {
    int id;
    char path[MAX_STR];
    char description[MAX_STR];
    char visibility[MAX_STR]; 
    int likes;
    Comment comments[MAX_COMMENTS];
    int comment_count;
};

struct User {
    char username[MAX_STR];
    char password_hash[MAX_STR];

    char display_name[MAX_STR];
    char bio[MAX_STR];
    char profile_visibility[MAX_STR];

    char friends[MAX_FRIENDS][MAX_STR];
    int friend_count;

    Post posts[MAX_POSTS];
    int post_count;
};

void loadUsers();
void saveUsers();