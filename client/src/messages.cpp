#include "messages.h"

void hash_password(const char* password, char* hashed_password)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *password++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    snprintf(hashed_password, 65, "%lu", hash);
}

void register_account(const char* username, const char* password, char* response)
{
    char command[MESSAGE_LENGTH];
    char hashed_password[65];
    hash_password(password, hashed_password);
    snprintf(command, sizeof(command), "REGISTER|%s|%s", username, hashed_password);
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
    char hashed_password[65];
    hash_password(password, hashed_password);
    snprintf(command, sizeof(command), "LOGIN|%s|%s", username, hashed_password);
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

void logout_account(int* user_id, const char* username, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "LOGOUT|%d|%s", *user_id, username);
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
    if(strncmp(response, "LOGOUT OK", 9) == 0)
    {
        *user_id = -1;
    }
    printf("Received response: %s\n", response);
}

void delete_account(int* user_id, const char* username, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "DELETE_ACCOUNT|%d|%s", *user_id, username);
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

    if(strncmp(response, "DELETE OK", 9) == 0)
    {
        *user_id = -1;
    }

    printf("Received response: %s\n", response);
}

void update_name(int user_id, const char* old_username, const char* new_username, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_NAME|%d|%s|%s", user_id, old_username, new_username);
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

void update_bio(int user_id, const char* username, const char* new_bio, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_BIO|%d|%s|%s", user_id, username, new_bio);
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

void update_password(int user_id, const char* username, const char* old_password, const char* new_password, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_PASSWORD|%d|%s|%s|%s", user_id, username, old_password, new_password);
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

void update_profile_picture(int user_id, const char* username, const char* image_path, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_PICTURE|%d|%s|%s", user_id, username, image_path);
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

void update_profile_visibility(int user_id, const char* username, const char* visibility, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_VISIBILITY|%d|%s|%s", user_id, username, visibility);
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

void follow_request(int user_id, const char* username, const char* username_to_follow, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "FOLLOW|%d|%s|%s", user_id, username, username_to_follow);
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

void accept_follow_request(int user_id, const char* username, const char* username_to_accept, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "ACCEPT_FOLLOW|%d|%s|%s", user_id, username, username_to_accept);
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

void reject_follow_request(int user_id, const char* username, const char* username_to_reject, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "REJECT_FOLLOW|%d|%s|%s", user_id, username, username_to_reject);
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

void unfollow_request(int user_id, const char* username, const char* username_to_unfollow, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UNFOLLOW|%d|%s|%s", user_id, username, username_to_unfollow);
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

void post(int user_id, const char* author, const char* description, const char* image_path, const char* visibility, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "POST|%d|%s|%s|%s|%s|%d", user_id, author, description, image_path, visibility, post_id);
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

void edit_post_visibility(int user_id, const char* author, const char* visibility, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "EDIT_POST_VISIBILITY|%d|%s|%s|%d", user_id, author, visibility, post_id);
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

void edit_post_description(int user_id, const char* author, const char* description, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "EDIT_POST_DESCRIPTION|%d|%s|%s|%d", user_id, author, description, post_id);
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

void like_post(int user_id, const char* username, const char* author, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "LIKE_POST|%d|%s|%s|%d", user_id, username, author, post_id);
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

void comment_post(int user_id, const char* username, const char* author, int post_id, const char* comment, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "COMMENT_POST|%d|%s|%s|%d|%s", user_id, username, author, post_id, comment);
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

void delete_post(int user_id, const char* author, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "DELETE_POST|%d|%s|%d", user_id, author, post_id);
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

void send_message_friend(int user_id, const char* username, const char* friend_name, const char* message, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "SEND_MESSAGE_FRIEND|%d|%s|%s|%s", user_id, username, friend_name, message);
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