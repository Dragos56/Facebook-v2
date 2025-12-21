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

void logout_account(int* user_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "LOGOUT|%d", *user_id);
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

void delete_account(int* user_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "DELETE_ACCOUNT|%d", *user_id);
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

void update_display_name(int user_id, const char* display_username, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_NAME|%d|%s", user_id, display_username);
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

void update_bio(int user_id, const char* new_bio, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_BIO|%d|%s", user_id, new_bio);
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

void update_password(int user_id, const char* old_password, const char* new_password, char* response)
{
    char command[MESSAGE_LENGTH];
    char hashed_old_password[65];
    char hashed_new_password[65];
    hash_password(old_password, hashed_old_password);
    hash_password(new_password, hashed_new_password);
    snprintf(command, sizeof(command), "UPDATE_PASSWORD|%d|%s|%s", user_id, hashed_old_password, hashed_new_password);
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

void update_profile_avatar(int user_id, const char* image_path, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_PICTURE|%d|%s", user_id, image_path);
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

void update_profile_visibility(int user_id, int visibility, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UPDATE_VISIBILITY|%d|%d", user_id, visibility);
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

void follow_request(int user_id, const char* username_to_follow, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "FOLLOW|%d|%s", user_id, username_to_follow);
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

void accept_follow_request(int user_id, const char* username_to_accept, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "ACCEPT_FOLLOW|%d|%s", user_id, username_to_accept);
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

void reject_follow_request(int user_id, const char* username_to_reject, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "REJECT_FOLLOW|%d|%s", user_id, username_to_reject);
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

void unfollow_request(int user_id, const char* username_to_unfollow, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "UNFOLLOW|%d|%s", user_id, username_to_unfollow);
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

void post(int user_id, const char* description, const char* image_path, int visibility, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "POST|%d|%s|%s|%d|%d", user_id, description, image_path, visibility, post_id);
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

void edit_post_visibility(int user_id, int visibility, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "EDIT_POST_VISIBILITY|%d|%d|%d", user_id, visibility, post_id);
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

void edit_post_description(int user_id, int post_id, const char* description, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "EDIT_POST_DESCRIPTION|%d|%d|%s", user_id, post_id, description);
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

void like_post(int user_id, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "LIKE_POST|%d|%d", user_id, post_id);
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

void comment_post(int user_id, int post_id, const char* comment, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "COMMENT_POST|%d|%d|%s", user_id, post_id, comment);
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

void delete_post(int user_id, int post_id, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "DELETE_POST|%d|%d", user_id, post_id);
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

void send_message_friend(int user_id, const char* friend_name, const char* message, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "SEND_MESSAGE_FRIEND|%d|%s|%s", user_id, friend_name, message);
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