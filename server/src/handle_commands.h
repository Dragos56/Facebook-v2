#pragma once
#define MESSAGE_LENGTH 1000

int handle_register(int client_fd, char* msg);
int handle_login(int client_fd, char* msg);
int handle_logout(int client_fd, char* msg);
int handle_delete_account(int client_fd, char* msg);
int handle_update_name(int client_fd, char* msg);
int handle_update_bio(int client_fd, char* msg);
int handle_update_password(int client_fd, char* msg);
int handle_update_picture(int client_fd, char* msg);
int handle_update_visibility(int client_fd, char* msg);
int handle_follow_request(int client_fd, char* msg);
int handle_accept_follow_request(int client_fd, char* msg);
int handle_reject_follow_request(int client_fd, char* msg);
int handle_unfollow_request(int client_fd, char* msg);
int handle_create_post(int client_fd, char* msg);
int handle_edit_post_description(int client_fd, char* msg);
int handle_edit_post_visibility(int client_fd, char* msg);
int handle_like_post(int client_fd, char* msg);
int handle_comment_post(int client_fd, char* msg);
int handle_delete_post(int client_fd, char* msg);

int handle_commands(int client_fd)
{
    int msg_length;	
    char msg[MESSAGE_LENGTH];

    
    msg_length = read (client_fd, msg, sizeof (msg));
    if (msg_length < 0)
    {
        perror ("Eroare la read() de la client.\n");
        return 0;
    }
    msg[msg_length] = '\0';
    printf ("[server]Mesajul a fost receptionat...%s\n", msg);
    fflush (stdout);

    char* command = strtok(msg, "|");

    if(strncmp(command, "REGISTER", 8) ==0)
    {
        return handle_register(client_fd, msg);
    }
    else if(strncmp(command, "LOGIN", 5) ==0)
    {
        return handle_login(client_fd, msg);
    }
    else if (strncmp(command, "LOGOUT", 6) ==0)
    {
        return handle_logout(client_fd, msg);
    }
    else if (strncmp(command, "DELETE_ACCOUNT", 14) ==0)
    {
        return handle_delete_account(client_fd, msg);
    }
    else if (strncmp(command, "UPDATE_NAME", 11) ==0)
    {
        return handle_update_name(client_fd, msg);
    }
    else if (strncmp(command, "UPDATE_BIO", 10) ==0)
    {
        return handle_update_bio(client_fd, msg);
    }
    else if (strncmp(command, "UPDATE_PASSWORD", 15) ==0)
    {
        return handle_update_password(client_fd, msg);
    }
    else if (strncmp(command, "UPDATE_PICTURE", 14) ==0)
    {
        return handle_update_picture(client_fd, msg);
    }
    else if (strncmp(command, "UPDATE_VISIBILITY", 17) ==0)
    {
        return handle_update_visibility(client_fd, msg);
    }
    else if (strncmp(command, "FOLLOW", 6) ==0)
    {
        return handle_follow_request(client_fd, msg);
    }
    else if (strncmp(command, "ACCEPT_FOLLOW", 13) ==0)
    {
        return handle_accept_follow_request(client_fd, msg);
    }
    else if (strncmp(command, "REJECT_FOLLOW", 13) ==0)
    {
        return handle_reject_follow_request(client_fd, msg);
    }
    else if (strncmp(command, "UNFOLLOW", 8) ==0)
    {
        return handle_unfollow_request(client_fd, msg);
    }
    else if (strncmp(command, "POST", 4) ==0)
    {
        return handle_create_post(client_fd, msg);
    }
    else if (strncmp(command, "EDIT_POST_DESCRIPTION", 21) ==0)
    {
        return handle_edit_post_description(client_fd, msg);
    }
    else if (strncmp(command, "EDIT_POST_VISIBILITY", 20) ==0)
    {
        return handle_edit_post_visibility(client_fd, msg);
    }
    else if (strncmp(command, "LIKE_POST", 9) ==0)
    {
        return handle_like_post(client_fd, msg);
    }
    else if (strncmp(command, "COMMENT_POST", 12) ==0)
    {
        return handle_comment_post(client_fd, msg);
    }
    else if (strncmp(command, "DELETE_POST", 11) ==0)
    {
        return handle_delete_post(client_fd, msg);
    }
    
    return 0;
}