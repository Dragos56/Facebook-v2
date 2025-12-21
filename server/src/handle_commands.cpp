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

int handle_update_avatar_path(int client_fd, char* args)
{
    char* p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char avatar_path[100];
    strcpy(avatar_path, p);
    if(db_update_profile_avatar_path(user_id, avatar_path)==-1)
    {
        const char* response = "UPDATE_PICTURE ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "UPDATE_PICTURE OK";
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

int handle_create_post(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    char description[200];
    strcpy(description, p);
    p=strtok(NULL, "|");
    char image_path[100];
    strcpy(image_path, p);
    p=strtok(NULL, "|");
    int visibility = atoi(p);
    p=strtok(NULL, "|");
    int post_id = atoi(p);

    if(db_create_post(user_id, description, image_path, visibility, &post_id)==-1)
    {
        const char* response = "CREATE_POST ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }
    
    const char* response = "CREATE_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_edit_post_description(int client_fd, char* args)
{
    char *p=strtok(args, "|");
    int user_id = atoi(p);
    p=strtok(NULL, "|");
    int post_id = atoi(p);
    p=strtok(NULL, "|");
    char new_description[200];
    strcpy(new_description, p);

    if(db_edit_post_description(user_id, post_id, new_description)==-1)
    {
        const char* response = "EDIT_POST_DESCRIPTION ERROR|Database error";
        send_message(client_fd, response);
        return 0;
    }

    const char* response = "EDIT_POST_DESCRIPTION OK";
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
    const char* response = "SEND_MESSAGE_FRIEND OK";
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
    else if (strcmp(command, "UPDATE_PICTURE") == 0)
    {
        return handle_update_avatar_path(client_fd, args);
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
    else if (strcmp(command, "EDIT_POST_DESCRIPTION") == 0)
    {
        return handle_edit_post_description(client_fd, args);
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

    return 0;
}