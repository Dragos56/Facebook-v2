#pragma once

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <pthread.h>

int db_init(const char* filename);
void db_close();

int db_user_exists(const char* username);
int db_add_user(const char* username, const char* password_hash);
int db_check_login(const char* username, const char* password_hash, int* user_id);
