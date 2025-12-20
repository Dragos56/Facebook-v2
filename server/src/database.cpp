#include "database.h"

sqlite3 *db_conn = NULL;
pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;

int db_init(const char* filename)
{
    int rc = sqlite3_open(filename, &db_conn);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error open DB: %s\n", sqlite3_errmsg(db_conn));
        return -1;
    }

    sqlite3_exec(db_conn, "PRAGMA foreign_keys = ON;", 0, 0, 0);
    sqlite3_exec(db_conn, "PRAGMA journal_mode=WAL;", 0, 0, 0);
    sqlite3_busy_timeout(db_conn, 5000);

    const char *sql_schema = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL, "
        "display_name TEXT, "
        "bio TEXT, "
        "avatar_path TEXT, "
        "visibility INTEGER DEFAULT 0, "
        "is_online INTEGER DEFAULT 0"
        ");"
        
        "CREATE TABLE IF NOT EXISTS posts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "description TEXT, "
        "image_path TEXT, "
        "visibility INTEGER DEFAULT 0, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"

        "CREATE TABLE IF NOT EXISTS friend_requests ("
        "sender_id INTEGER NOT NULL, "
        "receiver_id INTEGER NOT NULL, "
        "status INTEGER DEFAULT 0, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "PRIMARY KEY (sender_id, receiver_id), "
        "FOREIGN KEY(sender_id) REFERENCES users(id) ON DELETE CASCADE, "
        "FOREIGN KEY(receiver_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"

        "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "sender_id INTEGER NOT NULL, "
        "receiver_id INTEGER NOT NULL, "
        "content TEXT NOT NULL, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(sender_id) REFERENCES users(id) ON DELETE CASCADE, "
        "FOREIGN KEY(receiver_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"

        "CREATE TABLE IF NOT EXISTS likes ("
        "user_id INTEGER NOT NULL, "
        "post_id INTEGER NOT NULL, "
        "PRIMARY KEY (user_id, post_id),"
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE, "
        "FOREIGN KEY(post_id) REFERENCES posts(id) ON DELETE CASCADE"
        ");"

        "CREATE TABLE IF NOT EXISTS comments ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "post_id INTEGER NOT NULL, "
        "user_id INTEGER NOT NULL, "
        "content TEXT NOT NULL, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(post_id) REFERENCES posts(id) ON DELETE CASCADE, "
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"

        "CREATE TABLE IF NOT EXISTS groups ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "owner_id INTEGER NOT NULL, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(owner_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"

        "CREATE TABLE IF NOT EXISTS group_members ("
        "group_id INTEGER NOT NULL, "
        "user_id INTEGER NOT NULL, "
        "PRIMARY KEY (group_id, user_id), "
        "FOREIGN KEY(group_id) REFERENCES groups(id) ON DELETE CASCADE, "
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"

        "CREATE TABLE IF NOT EXISTS group_messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "group_id INTEGER NOT NULL, "
        "sender_id INTEGER NOT NULL, "
        "content TEXT NOT NULL, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(group_id) REFERENCES groups(id) ON DELETE CASCADE, "
        "FOREIGN KEY(sender_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"
        ;

    char *err_msg = 0;
    rc = sqlite3_exec(db_conn, sql_schema, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Init Error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    printf("[Database] Tabele verificate/create cu succes.\n");
    return 0;
}

void db_close()
{
    if (db_conn) {
        sqlite3_close(db_conn);
        db_conn = NULL;
        printf("[Database] DB inchis.\n");
    }
}

int db_user_exists(const char* username) 
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "SELECT id FROM users WHERE username = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    int exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return exists;
}

int db_add_user(const char* username, const char* password_hash) 
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "INSERT INTO users(username, password) VALUES (?, ?)", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash, -1, SQLITE_STATIC);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}

int db_check_login(const char* username, const char* password_hash, int* user_id) {
    int rc = 0;

    pthread_mutex_lock(&db_mutex);

    sqlite3_stmt* stmt;
    const char* sql = "SELECT id FROM users WHERE username = ? AND password = ?";

    if (sqlite3_prepare_v2(db_conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
        pthread_mutex_unlock(&db_mutex);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *user_id = sqlite3_column_int(stmt, 0);
        rc = 0;
    } else {
        rc = -1; 
    }

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);

    return rc;
}
