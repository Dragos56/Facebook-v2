#pragma once
#include <time.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_TEXT 256
#define MAX_PATH 100

#define MAX_USERS_GLOBAL 100
#define MAX_GROUPS_GLOBAL 100
#define MAX_CONVERSATIONS_GLOBAL 100

#define MAX_FRIENDS 50
#define MAX_POSTS 50
#define MAX_COMMENTS 10
#define MAX_LIKES 100
#define MAX_MESSAGES 100
#define MAX_REQUESTS 50
#define MAX_GROUP_NAME 50
#define MAX_GROUP_MEMBERS 50

typedef enum
{
    PUBLIC = 0,
    FRIENDS = 1,
    PRIVATE = 2
} Visibility;

typedef struct
{
    int id;
    int author_id;
    char text[MAX_TEXT];
    time_t timestamp;
} Comment;

typedef struct
{
    int id;
    int user_id;
    
    char path[MAX_PATH];
    char description[MAX_TEXT];
    Visibility visibility;
    
    int liked_by_users_ids[MAX_LIKES];
    int like_count;
    
    Comment comments[MAX_COMMENTS];
    int comment_count;
    time_t timestamp;
} Post;

typedef struct
{
    int sender_id;
    int receiver_id;
    bool accepted;
    time_t timestamp;
} FriendRequest;

typedef struct
{
    int id;
    int sender_id;     
    char text[MAX_TEXT];
    time_t timestamp;
} Message;

typedef struct
{
    int id;
    int user1_id;       
    int user2_id;       
    
    Message messages[MAX_MESSAGES];
    int message_count; 
} Conversation;

typedef struct
{
    int id;
    char name[MAX_GROUP_NAME];
    
    int member_ids[MAX_GROUP_MEMBERS];
    int member_count;
    
    Message messages[MAX_MESSAGES];
    int message_count;
} Group;

typedef struct
{
    int id;
    char username[MAX_USERNAME];
    char password_hash[MAX_PASSWORD];
    
    char display_name[MAX_USERNAME];
    char bio[MAX_TEXT];
    Visibility profile_visibility;
    
    int friend_ids[MAX_FRIENDS];
    int friend_count;
    
    Post posts[MAX_POSTS];
    int post_count;
    
    FriendRequest requests[MAX_REQUESTS];
    int request_count;
    
    int conversation_ids[MAX_CONVERSATIONS_GLOBAL];
    int conversation_count;

    int group_ids[MAX_GROUPS_GLOBAL];
    int group_count;
    int is_online;
    
    int current_socket;
} User;

typedef struct
{
    User users[MAX_USERS_GLOBAL];
    int total_users;
    
    Group groups[MAX_GROUPS_GLOBAL];
    int total_groups;

    Conversation conversations[MAX_CONVERSATIONS_GLOBAL]; 
    int total_conversations;

    int last_user_id;
    int last_post_id;
    int last_group_id;
    int last_conversation_id; 
} AppDatabase;
