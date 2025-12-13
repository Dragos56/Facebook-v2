#pragma once

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "load_save_data.h"

int db_init(const char* filename);
void db_close();
void load_db(AppDatabase *db);
void save_db(AppDatabase *db);
