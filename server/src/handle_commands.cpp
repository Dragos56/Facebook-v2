#include "handle_commands.h"

int send_message(int client_fd, const char* msg)
{
    int bytes = write(client_fd, msg, strlen(msg));
    return bytes;
}

int handle_register(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "REGISTER OK";
    send_message(client_fd, response);
    return 0;
}

int handle_login(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "LOGIN OK";
    send_message(client_fd, response);
    return 0;
}

int handle_logout(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "LOGOUT OK";
    send_message(client_fd, response);
    return 0;
}

int handle_delete_account(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "DELETE_ACCOUNT OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_name(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "UPDATE_NAME OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_bio(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "UPDATE_BIO OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_password(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "UPDATE_PASSWORD OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_picture(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "UPDATE_PICTURE OK";
    send_message(client_fd, response);
    return 0;
}

int handle_update_visibility(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "UPDATE_VISIBILITY OK";
    send_message(client_fd, response);
    return 0;
}

int handle_follow_request(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "FOLLOW_REQUEST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_accept_follow_request(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "ACCEPT_FOLLOW_REQUEST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_reject_follow_request(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "REJECT_FOLLOW_REQUEST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_unfollow_request(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "UNFOLLOW_REQUEST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_create_post(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "CREATE_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_edit_post_description(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "EDIT_POST_DESCRIPTION OK";
    send_message(client_fd, response);
    return 0;
}

int handle_edit_post_visibility(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "EDIT_POST_VISIBILITY OK";
    send_message(client_fd, response);
    return 0;
}

int handle_like_post(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "LIKE_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_comment_post(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "COMMENT_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_delete_post(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "DELETE_POST OK";
    send_message(client_fd, response);
    return 0;
}

int handle_send_message_friend(int client_fd, char* args, AppDatabase* db, pthread_mutex_t* db_mutex)
{
    const char* response = "SEND_MESSAGE_FRIEND OK";
    send_message(client_fd, response);
    return 0;
}

int handle_commands(int client_fd, AppDatabase* db, pthread_mutex_t* db_mutex)
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
        return handle_register(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "LOGIN") == 0)
    {
        return handle_login(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "LOGOUT") == 0)
    {
        return handle_logout(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "DELETE_ACCOUNT") == 0)
    {
        return handle_delete_account(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "UPDATE_NAME") == 0)
    {
        return handle_update_name(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "UPDATE_BIO") == 0)
    {
        return handle_update_bio(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "UPDATE_PASSWORD") == 0)
    {
        return handle_update_password(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "UPDATE_PICTURE") == 0)
    {
        return handle_update_picture(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "UPDATE_VISIBILITY") == 0)
    {
        return handle_update_visibility(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "FOLLOW") == 0)
    {
        return handle_follow_request(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "ACCEPT_FOLLOW") == 0)
    {
        return handle_accept_follow_request(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "REJECT_FOLLOW") == 0)
    {
        return handle_reject_follow_request(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "UNFOLLOW") == 0)
    {
        return handle_unfollow_request(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "POST") == 0)
    {
        return handle_create_post(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "EDIT_POST_DESCRIPTION") == 0)
    {
        return handle_edit_post_description(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "EDIT_POST_VISIBILITY") == 0)
    {
        return handle_edit_post_visibility(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "LIKE_POST") == 0)
    {
        return handle_like_post(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "COMMENT_POST") == 0)
    {
        return handle_comment_post(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "DELETE_POST") == 0)
    {
        return handle_delete_post(client_fd, args, db, db_mutex);
    }
    else if (strcmp(command, "SEND_MESSAGE_FRIEND") == 0)
    {
        return handle_send_message_friend(client_fd, args, db, db_mutex);
    }

    return 0;
}