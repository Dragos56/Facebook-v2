#include "database.h"

sqlite3 *db_conn = NULL;

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