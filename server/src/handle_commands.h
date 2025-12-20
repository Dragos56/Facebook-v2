#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "database.h"

#define MESSAGE_LENGTH 1000
#define USERNAME_LENGTH 50
#define PASSWORD_LENGTH 65


int send_message(int client_fd, const char* msg);
int handle_commands(int client_fd);

int handle_login(int client_fd, char* args);
int handle_register(int client_fd, char* args);
int handle_logout(int client_fd, char* args);
int handle_delete_account(int client_fd, char* args);

int handle_update_name(int client_fd, char* args);
int handle_update_bio(int client_fd, char* args);
int handle_update_password(int client_fd, char* args);
int handle_update_picture(int client_fd, char* args);
int handle_update_visibility(int client_fd, char* args);

int handle_follow_request(int client_fd, char* args);
int handle_accept_follow_request(int client_fd, char* args);
int handle_reject_follow_request(int client_fd, char* args);
int handle_unfollow_request(int client_fd, char* args);

int handle_create_post(int client_fd, char* args);
int handle_edit_post_description(int client_fd, char* args);
int handle_edit_post_visibility(int client_fd, char* args);
int handle_delete_post(int client_fd, char* args);
int handle_like_post(int client_fd, char* args);
int handle_comment_post(int client_fd, char* args);

int handle_send_message_friend(int client_fd, char* args);