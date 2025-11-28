#pragma once

#include "network.h"
#include <string>

using namespace std;

string register_account(string username, string password)
{
    string command="REGISTER|" + username + "|" + password;
    send_message(command);

    string response;
    int n = receive_message(&response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string login_account(string username, string password)
{
    string command="LOGIN|" + username + "|" + password;
    send_message(command);

    string response;
    int n = receive_message(&response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string logout_account(string username)
{
    string command="LOGOUT|" + username;
    send_message(command);

    string response;
    int n = receive_message(&response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string delete_account(string username)
{
    string command="DELETE|" + username;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string update_name(string old_username, string new_username)
{
    string command="UPDATE_NAME|" + old_username + "|" + new_username;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string update_password(string username, string old_password,string new_password)
{
    string command="UPDATE_PASSWORD|" + username + "|" + old_password + "|" + new_password;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string update_profile_picture(string username, string image_path)
{
    string command="UPDATE_PICTURE|" + username + "|" + image_path;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string update_profile_visibility(string username, string visibility)
{
    string command="UPDATE_VISIBILITY|" + username + "|" + visibility;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string follow_request(string username, string username_to_follow)
{
    string command="FOLLOW|" + username + "|" + username_to_follow;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string accept_follow_request(string username, string username_to_accept)
{
    string command="ACCEPT_FOLLOW|" + username + "|" + username_to_accept;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string reject_follow_request(string username, string username_to_reject)
{
    string command="REJECT_FOLLOW|" + username + "|" + username_to_reject;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string unfollow_request(string username, string username_to_unfollow)
{
    string command="UNFOLLOW|" + username + "|" + username_to_unfollow;
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string post(string author, string description, string image_path, string visibility, int post_id)
{
    string command="POST|" + author + "|" + description + "|" + image_path + "|" + visibility + "|" + to_string(post_id);
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string edit_post_visibility(string author, string visibility, int post_id)
{
    string command="EDIT_POST_VISIBILITY|" + author + "|" + visibility + "|" + to_string(post_id);
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string edit_post_description(string author, string description, int post_id)
{
    string command="EDIT_POST_DESCRIPTION|" + author + "|" + description + "|" + to_string(post_id);
    send_message(command);
    
    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string like_post(string username, string author, int post_id)
{
    string command="LIKE|" + username + "|" + author + "|" + to_string(post_id);
    send_message(command);

    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string comment_post(string username, string author, int post_id, string comment)
{
    string command="COMMENT|" + username + "|" + author + "|" + to_string(post_id) + "|" + comment;
    send_message(command);
    
    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

string delete_post(string author, int post_id)
{
    string command="DELETE|" + author + "|" + to_string(post_id);
    send_message(command);
    
    string response;
    int n = receive_message(response);
    if(n <= 0)
    {
        response="Eroare la primire.";
    }
    return response;
}

