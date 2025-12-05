#include "messages.h"

void register_account(const char* username, const char* password, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "REGISTER|%s|%s", username, password);
    printf("Sending command: %s\n", command);
    send_message(command);
 
    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void login_account(const char* username, const char* password, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "LOGIN|%s|%s", username, password);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void logout_account(const char* username, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "LOGOUT|%s", username);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void delete_account(const char* username, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "DELETE_ACCOUNT|%s", username);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void update_name(const char* old_username, const char* new_username, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_NAME|%s|%s", old_username, new_username);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void update_bio(const char* username, const char* new_bio, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_BIO|%s|%s", username, new_bio);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void update_password(const char* username, const char* old_password, const char* new_password, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_PASSWORD|%s|%s|%s", username, old_password, new_password);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void update_profile_picture(const char* username, const char* image_path, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_PICTURE|%s|%s", username, image_path);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void update_profile_visibility(const char* username, const char* visibility, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_VISIBILITY|%s|%s", username, visibility);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void follow_request(const char* username, const char* username_to_follow, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "FOLLOW|%s|%s", username, username_to_follow);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void accept_follow_request(const char* username, const char* username_to_accept, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "ACCEPT_FOLLOW|%s|%s", username, username_to_accept);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void reject_follow_request(const char* username, const char* username_to_reject, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "REJECT_FOLLOW|%s|%s", username, username_to_reject);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void unfollow_request(const char* username, const char* username_to_unfollow, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UNFOLLOW|%s|%s", username, username_to_unfollow);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void post(const char* author, const char* description, const char* image_path, const char* visibility, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "POST|%s|%s|%s|%s|%d", author, description, image_path, visibility, post_id);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void edit_post_visibility(const char* author, const char* visibility, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "EDIT_POST_VISIBILITY|%s|%s|%d", author, visibility, post_id);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void edit_post_description(const char* author, const char* description, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "EDIT_POST_DESCRIPTION|%s|%s|%d", author, description, post_id);
    printf("Sending command: %s\n", command);
    send_message(command);
    
    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void like_post(const char* username, const char* author, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "LIKE_POST|%s|%s|%d", username, author, post_id);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void comment_post(const char* username, const char* author, int post_id, const char* comment, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "COMMENT_POST|%s|%s|%d|%s", username, author, post_id, comment);
    printf("Sending command: %s\n", command);
    send_message(command);
    
    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void delete_post(const char* author, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "DELETE_POST|%s|%d", author, post_id);
    printf("Sending command: %s\n", command);
    send_message(command);
    
    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}

void send_message(const char* username, const char* friend_name, const char* message, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "SEND_MESSAGE|%s|%s|%s", username, friend_name, message);
    printf("Sending command: %s\n", command);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';
        strcpy(response, buffer);
    } 
    else 
    {
        strcpy(response, "Eroare la primire.");
    }
    printf("Received response: %s\n", response);
}