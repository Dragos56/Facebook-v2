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

void post(int user_id, const char* content, int visibility, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "POST|%d|%s|%d", user_id, content, visibility);
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

void edit_post_content(int user_id, int post_id, const char* content, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "EDIT_POST_content|%d|%d|%s", user_id, post_id, content);
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

void get_profile(int user_id, char* username, char* bio, char* display_name, int* visibility, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "GET_PROFILE|%d", user_id);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0) 
    {
        buffer[n] = '\0';

        if(strncmp(buffer, "GET_PROFILE ERROR", 17) == 0)
        {
            strcpy(response, "Eroare la primire.");
            return;
        }

        char* token = strtok(buffer, "|");
        token = strtok(NULL, "|");
        strncpy(username, token, USERNAME_LENGTH - 1);
        username[USERNAME_LENGTH - 1] = '\0';

        token = strtok(NULL, "|");
        strncpy(bio, token, BIO_LENGTH - 1);
        bio[BIO_LENGTH - 1] = '\0';

        token = strtok(NULL, "|");
        strncpy(display_name, token, USERNAME_LENGTH - 1);
        display_name[USERNAME_LENGTH - 1] = '\0';

        token = strtok(NULL, "|");
        *visibility = atoi(token);

        strcpy(response, "Profilul a fost preluat cu succes.");
    } 
    else strcpy(response, "Eroare la primire.");
}

void get_friends_list(int user_id, Friend* friends, int* friend_count, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "GET_FRIENDS_LIST|%d", user_id);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0)
    {
        buffer[n] = '\0';

        if(strncmp(buffer, "GET_FRIENDS_LIST ERROR", 21) == 0)
        {
            strcpy(response, "Eroare la primire.");
            return;
        }

        char* token = strtok(buffer, "|"); // GET_FRIENDS_LIST OK
        token = strtok(NULL, "|");         // friend count
        *friend_count = atoi(token);

        for (int i = 0; i < *friend_count && i < MAX_FRIENDS; i++) {
            token = strtok(NULL, "|");
            if (!token) break;

            char* field = strtok(token, "^");
            friends[i].user_id = atoi(field);

            field = strtok(NULL, "^");
            strncpy(friends[i].username, field, USERNAME_LENGTH - 1);
            friends[i].username[USERNAME_LENGTH - 1] = '\0';
        }

        strcpy(response, "Lista de prieteni a fost preluata cu succes.");   
    } 
    else strcpy(response, "Eroare la primire.");
}

void get_follow_requests(int user_id, Request* requests, int* request_count, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "GET_FOLLOW_REQUESTS|%d", user_id);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n > 0)
    {
        buffer[n] = '\0';

        if(strncmp(buffer, "GET_FOLLOW_REQUESTS ERROR", 25) == 0)
        {
            strcpy(response, "Eroare la primire.");
            return;
        }

        char* token = strtok(buffer, "|");
        token = strtok(NULL, "|");         
        *request_count = atoi(token);

        for (int i = 0; i < *request_count && i < MAX_FRIENDS; i++) 
        {
            token = strtok(NULL, "|");
            if (!token) break;
            requests[i].request_id = atoi(token);
        }

        strcpy(response, "Cererile de urmarire au fost preluate cu succes.");
    } 
    else strcpy(response, "Eroare la primire.");
}

void get_user_posts(int user_id, Post* posts, int* post_count, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "GET_USER_POSTS|%d", user_id);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n <= 0) { strcpy(response, "Eroare la primire."); return; }

    buffer[n] = '\0';

    if (strncmp(buffer, "GET_USER_POSTS ERROR", 20) == 0) { strcpy(response, "Eroare la primire."); return; }

    char* token = strtok(buffer, "|"); // "GET_USER_POSTS OK"
    token = strtok(NULL, "|");         // number of posts
    if (!token) { strcpy(response, "Răspuns invalid de la server."); return; }

    *post_count = atoi(token);

    for (int i = 0; i < *post_count && i < MAX_POSTS; i++) 
    {
        token = strtok(NULL, "|"); // fiecare post ca string
        if (!token) break;

        int post_id, user_id, visibility, like_count;
        char content[MESSAGE_LENGTH];
        char username[USERNAME_LENGTH];

        int ret = sscanf(token, "%d^%d^%199[^^]^%d^%49[^^]^%d",
                        &post_id, &user_id, content, &visibility, username, &like_count);
        if (ret != 6) { printf("Parsing failed for token: %s\n", token); continue; }

        posts[i].post_id = post_id;
        posts[i].user_id = user_id;
        strncpy(posts[i].content, content, MESSAGE_LENGTH-1);
        posts[i].content[MESSAGE_LENGTH-1] = '\0';
        posts[i].visibility = visibility;
        strncpy(posts[i].username, username, USERNAME_LENGTH-1);
        posts[i].username[USERNAME_LENGTH-1] = '\0';
        posts[i].like_count = like_count;
        posts[i].comment_count = 0;
    }

    strcpy(response, "Postările au fost preluate cu succes.");
}

void get_feed(int user_id, Post* posts, int* post_count, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "GET_FEED|%d", user_id);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n <= 0) { strcpy(response, "Eroare la primire."); return; }

    buffer[n] = '\0';

    if(strncmp(buffer, "GET_FEED ERROR", 14) == 0) { strcpy(response, "Eroare la primire."); return; }

    char* token = strtok(buffer, "|"); // GET_FEED OK
    token = strtok(NULL, "|");         // post count
    *post_count = atoi(token);

    for (int i = 0; i < *post_count && i < MAX_POSTS; i++) 
    {
        token = strtok(NULL, "|");
        if (!token) break;

        int post_id, user_id, visibility, like_count;
        char content[MESSAGE_LENGTH];
        char username[USERNAME_LENGTH];

        int ret = sscanf(token, "%d^%d^%199[^^]^%d^%49[^^]^%d",
                        &post_id, &user_id, content, &visibility, username, &like_count);
        if (ret != 6) { printf("Parsing failed for token: %s\n", token); continue; }

        posts[i].post_id = post_id;
        posts[i].user_id = user_id;
        strncpy(posts[i].content, content, MESSAGE_LENGTH-1);
        posts[i].content[MESSAGE_LENGTH-1] = '\0';
        posts[i].visibility = visibility;
        strncpy(posts[i].username, username, USERNAME_LENGTH-1);
        posts[i].username[USERNAME_LENGTH-1] = '\0';
        posts[i].like_count = like_count;
        posts[i].comment_count = 0;
    }

    strcpy(response, "Feed-ul a fost preluat cu succes.");
}

void get_post_likes(int post_id, int* user_ids, int* like_count, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "GET_POST_LIKES|%d", post_id);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n <= 0) { strcpy(response, "Eroare la primire."); return; }

    buffer[n] = '\0';
    if(strncmp(buffer, "GET_POST_LIKES ERROR", 20) == 0) { strcpy(response, "Eroare la primire."); return; }

    char* token = strtok(buffer, "|");
    token = strtok(NULL, "|");
    *like_count = atoi(token);

    for (int i = 0; i < *like_count; i++) 
    {
        token = strtok(NULL, "|");
        user_ids[i] = atoi(token);
    }

    strcpy(response, "Like-urile postarii au fost preluate cu succes.");
}

void get_post_comments(int post_id, int* user_ids, char comments[][COMMENT_LENGTH], int* comment_count, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "GET_POST_COMMENTS|%d", post_id);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n <= 0) { strcpy(response, "Eroare la primire."); return; }

    buffer[n] = '\0';
    if(strncmp(buffer, "GET_POST_COMMENTS ERROR", 23) == 0) { strcpy(response, "Eroare la primire."); return; }

    char* token = strtok(buffer, "|");
    token = strtok(NULL, "|");
    *comment_count = atoi(token);

    for (int i = 0; i < *comment_count; i++) 
    {
        token = strtok(NULL, "|");
        if (!token) break;

        char* field = strtok(token, "^");
        user_ids[i] = atoi(field);

        field = strtok(NULL, "^");
        strncpy(comments[i], field, COMMENT_LENGTH - 1);
        comments[i][COMMENT_LENGTH - 1] = '\0';
    }

    strcpy(response, "Comentariile postarii au fost preluate cu succes.");
}

void get_username_by_id(int user_id, char* username, char* response)
{
    char command[MESSAGE_LENGTH];
    snprintf(command, sizeof(command), "GET_USERNAME_BY_ID|%d", user_id);
    send_message(command);

    char buffer[MESSAGE_LENGTH];
    int n = receive_message(buffer);
    if (n <= 0) { strcpy(response, "Eroare la primire."); return; }

    buffer[n] = '\0';
    if(strncmp(buffer, "GET_USERNAME_BY_ID ERROR", 23) == 0) { strcpy(response, "Eroare la primire."); return; }

    char* token = strtok(buffer, "|");
    token = strtok(NULL, "|");
    strncpy(username, token, USERNAME_LENGTH - 1);
    username[USERNAME_LENGTH - 1] = '\0';

    strcpy(response, "Numele de utilizator a fost preluat cu succes.");
}
