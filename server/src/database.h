#pragma once

#include "sqlite3.h"
#include "load_save_data.h"

int db_init(const char* filename);
void load_db(AppDatabase *db);
void save_db(AppDatabase *db);
void db_close();
