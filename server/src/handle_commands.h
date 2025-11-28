#pragma once
#define MESSAGE_LENGTH 1000
#include <cstdio>      
#include <cstring>     
#include <unistd.h>   
#include <sys/types.h> 
#include <sys/socket.h>

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
int handle_commands(int client_fd);