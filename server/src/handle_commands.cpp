#include "handle_commands.h"

int send_message(int client_fd, const char* msg)
{
    int bytes = write(client_fd, msg, strlen(msg));
    return bytes;
}

int handle_register(int client_fd, char* args)
{
    char args_copy[256];
    strcpy(args_copy, args);
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    char *p=strtok(args_copy, "|");
    strcpy(username, p);
    p=strtok(NULL, "|");
    strcpy(password, p);

    if(db_user_exists(username)==1)
    {
        const char* response = "REGISTER ERROR|User already exists";
        send_message(client_fd, response);
        return 0;
    };

    if(db_add_user(username, password)==-1)
    {
        const char* response = "REGISTER ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    };

    handle_login(client_fd, args);
    return 0;
}

int handle_login(int client_fd, char* args)
{
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    char *p=strtok(args, "|");
    strcpy(username, p);
    p=strtok(NULL, "|");
    strcpy(password, p);

    int user_id;
    if(db_check_login(username, password, &user_id)==-1)
    {
        const char* response = "LOGIN ERROR|Invalid username or password";
        send_message(client_fd, response);
        return 0;
    }

    if(db_is_online(user_id, 1)==-1)
    {
        const char* response = "LOGIN ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[50] = "LOGIN OK";
    snprintf(response, sizeof(response), "LOGIN OK|%d", user_id);
    send_message(client_fd, response);
    return 0;
}

int handle_logout(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);  
    if(db_is_online(user_id, 0)==-1)
    {
        const char* response = "LOGOUT ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "LOGOUT OK";
    send_message(client_fd, response);
    return 0;
}

int handle_delete_account(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");

    if(db_delete_user(user_id)==-1)
    {
        const char* response = "DELETE_ACCOUNT ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "DELETE_ACCOUNT OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_display_name(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char display_name[50];
    strcpy(display_name, p);

    if(db_update_profile_display_name(user_id, display_name)==-1)
    {
        const char* response = "UPDATE_NAME ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "UPDATE_NAME OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_bio(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char bio[200];
    strcpy(bio, p);

    if(db_update_profile_bio(user_id, bio)==-1)
    {
        const char* response = "UPDATE_BIO ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "UPDATE_BIO OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_password(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char old_password[PASSWORD_LENGTH];
    strcpy(old_password, p);
    p=strtok(NULL, "|");
    char new_password[PASSWORD_LENGTH];
    strcpy(new_password, p);

    if(db_update_password(user_id, old_password, new_password)==-1)
    {
        const char* response = "UPDATE_PASSWORD ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "UPDATE_PASSWORD OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_visibility(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int visibility = atoi(p);
    if(db_update_profile_visibility(user_id, visibility)==-1)
    {
        const char* response = "UPDATE_VISIBILITY ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }
    const char* response = "UPDATE_VISIBILITY OK";
    send_message(client_fd, response);
    return 0;
}

int handle_follow_request(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char username_to_follow[50];
    strcpy(username_to_follow, p);

    if(db_follow_user(user_id, username_to_follow)==-1)
    {
        const char* response = "FOLLOW_REQUEST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "FOLLOW_REQUEST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_accept_follow_request(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char username_to_accept[50];
    strcpy(username_to_accept, p);
    
    if(db_accept_follow_request(user_id, username_to_accept)==-1)
    {
        const char* response = "ACCEPT_FOLLOW_REQUEST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "ACCEPT_FOLLOW_REQUEST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_reject_follow_request(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char username_to_reject[50];
    strcpy(username_to_reject, p);

    if(db_reject_follow_request(user_id, username_to_reject)==-1)
    {
        const char* response = "REJECT_FOLLOW_REQUEST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "REJECT_FOLLOW_REQUEST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_unfollow_request(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char username_to_unfollow[50];
    strcpy(username_to_unfollow, p);

    if(db_unfollow_user(user_id, username_to_unfollow)==-1)
    {
        const char* response = "UNFOLLOW_REQUEST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }
    
    const char* response = "UNFOLLOW_REQUEST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_add_close_friend(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int close_friend_id = atoi(p);

    if(db_add_close_friend(user_id, close_friend_id)==-1)
    {
        const char* response = "ADD_CLOSE_FRIEND ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "ADD_CLOSE_FRIEND OK";
    send_message(client_fd, response);
    return 0;
}

int handle_remove_close_friend(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int close_friend_id = atoi(p);

    if(db_remove_close_friend(user_id, close_friend_id)==-1)
    {
        const char* response = "REMOVE_CLOSE_FRIEND ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "REMOVE_CLOSE_FRIEND OK";
    send_message(client_fd, response);
    return 0;
}

int handle_create_post(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char content[200];
    strcpy(content, p);
    p=strtok(NULL, "|");
    int visibility = atoi(p);

    int post_id;

    if(db_create_post(user_id, content, visibility, &post_id)==-1)
    {
        const char* response = "CREATE_POST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }
    
    const char* response = "CREATE_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_edit_post_content(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int post_id = atoi(p);
    p=strtok(NULL, "|");
    char new_content[200];
    strcpy(new_content, p);

    if(db_edit_post_content(user_id, post_id, new_content)==-1)
    {
        const char* response = "EDIT_POST_CONTENT ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "EDIT_POST_CONTENT OK";
    send_message(client_fd, response);
    return 0;
}

int handle_edit_post_visibility(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int post_id = atoi(p);
    p=strtok(NULL, "|");
    int new_visibility = atoi(p);

    if(db_edit_post_visibility(user_id, post_id, new_visibility)==-1)
    {
        const char* response = "EDIT_POST_VISIBILITY ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "EDIT_POST_VISIBILITY OK";
    send_message(client_fd, response);
    return 0;
}

int handle_like_post(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int post_id = atoi(p);
    
    if(db_like_post(user_id, post_id)==-1)
    {
        const char* response = "LIKE_POST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "LIKE_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_comment_post(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int post_id = atoi(p);
    p=strtok(NULL, "|");
    char comment[300];
    strcpy(comment, p);

    if(db_comment_post(user_id, post_id, comment)==-1)
    {
        const char* response = "COMMENT_POST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "COMMENT_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_delete_post(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int post_id = atoi(p);

    if(db_delete_post(user_id, post_id)==-1)
    {
        const char* response = "DELETE_POST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "DELETE_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_send_message_friend(int client_fd, char* args)
{
    char *p=strtok(args,"|");
    int user_id = atoi(p);
    printf("%d",user_id);
    const char* response = "SEND_MESSAGE_FRIEND OK";
    send_message(client_fd, response);
    return 0;
}

int handle_get_profile(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    
    char username[USERNAME_LENGTH];
    char bio[BIO_LENGTH];
    char display_name[USERNAME_LENGTH];
    int visibility;

    if(db_get_profile(user_id, username, bio, display_name, &visibility)==-1)
    {
        const char* response = "GET_PROFILE ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[2000];
    snprintf(response, sizeof(response), "GET_PROFILE OK|%s|%s|%s|%d", username, bio, display_name, visibility);
    printf("get profile: %s\n", response);
    send_message(client_fd, response);
    return 0;
}

int handle_get_friends_list(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);

    Friend friends[MAX_FRIENDS];
    int friend_count = 0;


    if(db_get_friends_list(user_id, friends, MAX_FRIENDS, &friend_count)==-1)
    {
        const char* response = "GET_FRIENDS_LIST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[MESSAGE_LENGTH];
    int offset = snprintf(response, sizeof(response), "GET_FRIENDS_LIST OK|%d", friend_count);
    for(int i=0; i<friend_count; i++)
    {
        offset += snprintf(response + offset, sizeof(response) - offset, "|%d^%s^%d", friends[i].user_id, friends[i].display_name, friends[i].close_friend);
    }
    printf("get friends list: %s\n", response);
    send_message(client_fd, response);
    return 0;
}

int handle_get_follow_requests(int client_fd, char* args)
{
    char *p = strtok(args, "|");
    int user_id = atoi(p);

    Request requests[MAX_FOLLOW_REQUESTS];
    int request_count = 0;

    if(db_get_follow_requests(user_id, requests, MAX_FOLLOW_REQUESTS, &request_count) == -1)
    {
        const char* response = "GET_FOLLOW_REQUESTS ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[MESSAGE_LENGTH];
    int offset = snprintf(response, sizeof(response), "GET_FOLLOW_REQUESTS OK|%d", request_count);

    for(int i = 0; i < request_count; i++)
    {
        offset += snprintf(response + offset, sizeof(response) - offset, "|%d^%s", requests[i].request_id, requests[i].display_name);
    }

    printf("get follow requests: %s\n", response);
    send_message(client_fd, response);
    return 0;
}

int handle_get_user_posts(int client_fd, char* args)
{
    char *p = strtok(args, "|");
    int user_id = atoi(p);

    Post posts[MAX_POSTS];
    int post_count = 0;

    if(db_get_user_posts(user_id, posts, MAX_POSTS, &post_count) == -1)
    {
        const char* response = "GET_USER_POSTS ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[MESSAGE_LENGTH];
    int offset = snprintf(response, sizeof(response), "GET_USER_POSTS OK|%d", post_count);

    for(int i = 0; i < post_count; i++)
    {
        char cont[MESSAGE_LENGTH] = "";
        char uname[USERNAME_LENGTH] = "";

        strncpy(cont, posts[i].content, MESSAGE_LENGTH-1);
        cont[MESSAGE_LENGTH-1] = '\0';

        strncpy(uname, posts[i].display_name, USERNAME_LENGTH-1);
        uname[USERNAME_LENGTH-1] = '\0';

        offset += snprintf(response + offset, sizeof(response) - offset, "|%d^%d^%s^%d^%s^%d^%d", posts[i].post_id, posts[i].user_id, cont, posts[i].visibility, uname, posts[i].like_count, posts[i].comment_count);

        if(offset >= MESSAGE_LENGTH - 100) break;
    }
    printf("get user posts: %s\n", response);
    send_message(client_fd, response);
    return 0;
}


int handle_get_feed(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);

    Post posts[MAX_POSTS];
    int post_count = 0;
    if(db_get_feed(user_id, posts, MAX_POSTS, &post_count)==-1)
    {
        const char* response = "GET_FEED ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }
    char response[MESSAGE_LENGTH];
    int offset = snprintf(response, sizeof(response), "GET_FEED OK|%d", post_count);
    for(int i=0; i<post_count; i++)
    {
        char cont[MESSAGE_LENGTH] = "";
        char uname[USERNAME_LENGTH] = "";

        strncpy(cont, posts[i].content, MESSAGE_LENGTH-1);
        cont[MESSAGE_LENGTH-1] = '\0';

        strncpy(uname, posts[i].display_name, USERNAME_LENGTH-1);
        uname[USERNAME_LENGTH-1] = '\0';
        
        offset += snprintf(response + offset, sizeof(response) - offset, "|%d^%d^%s^%d^%s^%d^%d", posts[i].post_id, posts[i].user_id, cont, posts[i].visibility, uname, posts[i].like_count, posts[i].comment_count);
    }
    printf("get feed: %s\n", response);
    send_message(client_fd, response);
    return 0;
}

int handle_get_post_likes(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int post_id = atoi(p);

    Like likes[MAX_LIKES];
    int like_count = 0;

    if(db_get_post_likes(post_id, likes, MAX_LIKES, &like_count)==-1)
    {
        const char* response = "GET_POST_LIKES ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[MESSAGE_LENGTH];
    int offset = snprintf(response, sizeof(response), "GET_POST_LIKES OK|%d", like_count);
    for(int i=0; i<like_count; i++)
    {
        offset += snprintf(response + offset, sizeof(response) - offset, "|%d^%s", likes[i].user_id, likes[i].display_name);
    }
    printf("get post likes: %s\n", response);
    send_message(client_fd, response);
    return 0;
}

int handle_get_post_comments(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int post_id = atoi(p);

    Comment comments[MAX_COMMENTS];
    int comment_count = 0;

    if(db_get_post_comments(post_id, comments, MAX_COMMENTS, &comment_count)==-1)
    {
        const char* response = "GET_POST_COMMENTS ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[MESSAGE_LENGTH];
    int offset = snprintf(response, sizeof(response), "GET_POST_COMMENTS OK|%d", comment_count);
    for(int i=0; i<comment_count; i++)
    {
        offset += snprintf(response + offset, sizeof(response) - offset, "|%d^%s^%s", comments[i].user_id, comments[i].display_name, comments[i].comment);
    }
    printf("get post comments: %s\n", response);
    send_message(client_fd, response);
    return 0;
}

int handle_get_username_by_id(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);

    char username[USERNAME_LENGTH];

    if(db_get_username_by_id(user_id, username)==-1)
    {
        const char* response = "GET_USERNAME_BY_ID ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[MESSAGE_LENGTH];
    snprintf(response, sizeof(response), "GET_USERNAME_BY_ID OK|%s", username);
    printf("get username by id: %s\n", response);
    send_message(client_fd, response);
    return 0;
}

int handle_search_user(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    char display_name[USERNAME_LENGTH];
    strcpy(display_name, p);

    int user_id;

    if(db_search_user(display_name, &user_id)==-1)
    {
        const char* response = "SEARCH_USER ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    char response[MESSAGE_LENGTH];
    snprintf(response, sizeof(response), "SEARCH_USER OK|%d", user_id);
    printf("search user: %s\n", response);
    send_message(client_fd, response);
    return 0;
}

int handle_commands(int client_fd)
{
    int msg_length;
    char msg[MESSAGE_LENGTH];

    msg_length = read(client_fd, msg, sizeof(msg) - 1);
    if (msg_length < 0)
    {
        perror("Eroare la read() de la client.\n");
        return 0;
    }
    else if (msg_length == 0)
    {
        printf("[server] Clientul s-a inchis.\n");
        return 1;
    }

    msg[msg_length] = '\0';
    
    if (msg_length > 0 && msg[msg_length - 1] == '\n')
    {
        msg[msg_length - 1] = '\0';
    }

    printf("[server] Mesaj receptionat: %s\n", msg);
    fflush(stdout);

    char* command = strtok(msg, "|");
    char* args = strtok(NULL, ""); 

    if (command == NULL)
    {
        return 0;
    }

    if (strcmp(command, "REGISTER") == 0)
    {
        return handle_register(client_fd, args);
    }
    else if (strcmp(command, "LOGIN") == 0)
    {
        return handle_login(client_fd, args);
    }
    else if (strcmp(command, "LOGOUT") == 0)
    {
        return handle_logout(client_fd, args);
    }
    else if (strcmp(command, "DELETE_ACCOUNT") == 0)
    {
        return handle_delete_account(client_fd, args);
    }
    else if (strcmp(command, "UPDATE_NAME") == 0)
    {
        return handle_update_display_name(client_fd, args);
    }
    else if (strcmp(command, "UPDATE_BIO") == 0)
    {
        return handle_update_bio(client_fd, args);
    }
    else if (strcmp(command, "UPDATE_PASSWORD") == 0)
    {
        return handle_update_password(client_fd, args);
    }
    else if (strcmp(command, "UPDATE_VISIBILITY") == 0)
    {
        return handle_update_visibility(client_fd, args);
    }
    else if (strcmp(command, "FOLLOW") == 0)
    {
        return handle_follow_request(client_fd, args);
    }
    else if (strcmp(command, "ACCEPT_FOLLOW") == 0)
    {
        return handle_accept_follow_request(client_fd, args);
    }
    else if (strcmp(command, "REJECT_FOLLOW") == 0)
    {
        return handle_reject_follow_request(client_fd, args);
    }
    else if (strcmp(command, "UNFOLLOW") == 0)
    {
        return handle_unfollow_request(client_fd, args);
    }
    else if (strcmp(command, "POST") == 0)
    {
        return handle_create_post(client_fd, args);
    }
    else if (strcmp(command, "EDIT_POST_CONTENT") == 0)
    {
        return handle_edit_post_content(client_fd, args);
    }
    else if (strcmp(command, "EDIT_POST_VISIBILITY") == 0)
    {
        return handle_edit_post_visibility(client_fd, args);
    }
    else if (strcmp(command, "LIKE_POST") == 0)
    {
        return handle_like_post(client_fd, args);
    }
    else if (strcmp(command, "COMMENT_POST") == 0)
    {
        return handle_comment_post(client_fd, args);
    }
    else if (strcmp(command, "DELETE_POST") == 0)
    {
        return handle_delete_post(client_fd, args);
    }
    else if (strcmp(command, "SEND_MESSAGE_FRIEND") == 0)
    {
        return handle_send_message_friend(client_fd, args);
    }
    else if (strcmp(command, "GET_PROFILE") == 0)
    {
        return handle_get_profile(client_fd, args);
    }
    else if (strcmp(command, "GET_FRIENDS_LIST") == 0)
    {
        return handle_get_friends_list(client_fd, args);
    }
    else if (strcmp(command, "GET_FOLLOW_REQUESTS") == 0)
    {
        return handle_get_follow_requests(client_fd, args);
    }
    else if (strcmp(command, "GET_USER_POSTS") == 0)
    {
        return handle_get_user_posts(client_fd, args);
    }
    else if (strcmp(command, "GET_FEED") == 0)
    {
        return handle_get_feed(client_fd, args);
    }
    else if (strcmp(command, "GET_POST_LIKES") == 0)
    {
        return handle_get_post_likes(client_fd, args);
    }
    else if (strcmp(command, "GET_POST_COMMENTS") == 0)
    {
        return handle_get_post_comments(client_fd, args);
    }
    else if (strcmp(command, "GET_USERNAME_BY_ID") == 0)
    {
        return handle_get_username_by_id(client_fd, args);
    }
    else if (strcmp(command, "SEARCH_USER") == 0)
    {
        return handle_search_user(client_fd, args);
    }
    else if (strcmp(command, "ADD_CLOSE_FRIEND") == 0)
    {
        return handle_add_close_friend(client_fd, args);
    }
    else if (strcmp(command, "REMOVE_CLOSE_FRIEND") == 0)
    {
        return handle_remove_close_friend(client_fd, args);
    }

    return 0;
}