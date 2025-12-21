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

int db_update_profile_bio(int user_id, const char* bio)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE users SET bio = ? WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, bio, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_update_profile_display_name(int user_id, const char* display_name)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE users SET display_name = ? WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, display_name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_update_profile_avatar_path(int user_id, const char* avatar_path)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE users SET avatar_path = ? WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, avatar_path, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_update_profile_visibility(int user_id, int visibility)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE users SET visibility = ? WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, visibility);
    sqlite3_bind_int(stmt, 2, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_update_password(int user_id, const char* old_password_hash, const char* new_password_hash)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE users SET password = ? WHERE id = ? AND password = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, new_password_hash, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_text(stmt, 3, old_password_hash, -1, SQLITE_STATIC);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db_conn) > 0) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_is_online(int user_id, int is_online)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE users SET is_online = ? WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, is_online);
    sqlite3_bind_int(stmt, 2, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_delete_user(int user_id)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "DELETE FROM users WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db_conn) > 0) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}

int db_follow_user(int user_id, const char* username_to_follow)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "INSERT INTO friend_requests(sender_id, receiver_id) "
                                 "SELECT ?, id FROM users WHERE username = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, username_to_follow, -1, SQLITE_STATIC);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}

int db_accept_follow_request(int user_id, const char* username_to_accept)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE friend_requests SET status = 1 "
                                 "WHERE receiver_id = ? AND sender_id = (SELECT id FROM users WHERE username = ?)", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, username_to_accept, -1, SQLITE_STATIC);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db_conn) > 0) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}

int db_reject_follow_request(int user_id, const char* username_to_reject)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "DELETE FROM friend_requests "
                                 "WHERE receiver_id = ? AND sender_id = (SELECT id FROM users WHERE username = ?)", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, username_to_reject, -1, SQLITE_STATIC);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db_conn) > 0) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}

int db_unfollow_user(int user_id, const char* username_to_unfollow)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "DELETE FROM friend_requests "
                                 "WHERE sender_id = ? AND receiver_id = (SELECT id FROM users WHERE username = ?)", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, username_to_unfollow, -1, SQLITE_STATIC);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db_conn) > 0) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}

int db_create_post(int user_id, const char* description, const char* image_path, int visibility, int* post_id)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "INSERT INTO posts(user_id, description, image_path, visibility) VALUES (?, ?, ?, ?)", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, image_path, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, visibility);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    if (rc == 0) {
        *post_id = (int)sqlite3_last_insert_rowid(db_conn);
    }
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_edit_post_description(int user_id, int post_id, const char* new_description)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE posts SET description = ? WHERE id = ? AND user_id = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, new_description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, post_id);
    sqlite3_bind_int(stmt, 3, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db_conn) > 0) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_edit_post_visibility(int user_id, int post_id, int new_visibility)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "UPDATE posts SET visibility = ? WHERE id = ? AND user_id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, new_visibility);
    sqlite3_bind_int(stmt, 2, post_id);
    sqlite3_bind_int(stmt, 3, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db_conn) > 0) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_delete_post(int user_id, int post_id)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "DELETE FROM posts WHERE id = ? AND user_id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, post_id);
    sqlite3_bind_int(stmt, 2, user_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db_conn) > 0) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_like_post(int user_id, int post_id)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "INSERT INTO likes(user_id, post_id) VALUES (?, ?)", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, post_id);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}
int db_comment_post(int user_id, int post_id, const char* comment)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "INSERT INTO comments(post_id, user_id, content) VALUES (?, ?, ?)", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, post_id);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_text(stmt, 3, comment, -1, SQLITE_STATIC);
    int rc = (sqlite3_step(stmt) == SQLITE_DONE) ? 0 : -1;
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}

int db_get_profile(int user_id, char* username, char* bio, char* display_name, char* avatar_path, int* visibility)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, "SELECT username, bio, display_name, avatar_path, visibility FROM users WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    int rc = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* uname = sqlite3_column_text(stmt, 0);
        const unsigned char* b = sqlite3_column_text(stmt, 1);
        const unsigned char* dname = sqlite3_column_text(stmt, 2);
        const unsigned char* avatar = sqlite3_column_text(stmt, 3);
        int vis = sqlite3_column_int(stmt, 4);

        if (uname) strcpy(username, (const char*)uname); else username[0] = '\0';
        if (b) strcpy(bio, (const char*)b); else bio[0] = '\0';
        if (dname) strcpy(display_name, (const char*)dname); else display_name[0] = '\0';
        if (avatar) strcpy(avatar_path, (const char*)avatar); else avatar_path[0] = '\0';
        *visibility = vis;

        rc = 0;
    }
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return rc;
}

int db_get_friends_list(int user_id, int* friend_ids, int max_friends, int* friend_count)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, 
        "SELECT sender_id FROM friend_requests WHERE receiver_id = ? AND status = 1 "
        "UNION "
        "SELECT receiver_id FROM friend_requests WHERE sender_id = ? AND status = 1", 
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, user_id);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_friends) {
        friend_ids[count++] = sqlite3_column_int(stmt, 0);
    }
    *friend_count = count;

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return 0;
}

int db_get_follow_requests(int user_id, int* request_ids, int max_requests, int* request_count)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, 
        "SELECT sender_id FROM friend_requests WHERE receiver_id = ? AND status = 0", 
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_requests) {
        request_ids[count++] = sqlite3_column_int(stmt, 0);
    }
    *request_count = count;

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return 0;
}

int db_get_user_posts(int user_id, Post* posts, int max_posts, int* post_count)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, 
        "SELECT p.id, p.user_id, u.username, p.description, p.image_path, p.visibility, "
        "(SELECT COUNT(*) FROM likes l WHERE l.post_id = p.id) AS like_count, "
        "(SELECT COUNT(*) FROM comments c WHERE c.post_id = p.id) AS comment_count "
        "FROM posts p JOIN users u ON p.user_id = u.id "
        "WHERE p.user_id = ? "
        "ORDER BY p.created_at DESC", 
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_posts) {
        posts[count].post_id = sqlite3_column_int(stmt, 0);
        posts[count].user_id = sqlite3_column_int(stmt, 1);
        const unsigned char* uname = sqlite3_column_text(stmt, 2);
        const unsigned char* desc = sqlite3_column_text(stmt, 3);
        const unsigned char* img_path = sqlite3_column_text(stmt, 4);
        posts[count].visibility = sqlite3_column_int(stmt, 5);
        posts[count].like_count = sqlite3_column_int(stmt, 6);
        posts[count].comment_count = sqlite3_column_int(stmt, 7);

        if (uname) strcpy(posts[count].username, (const char*)uname); else posts[count].username[0] = '\0';
        if (desc) strcpy(posts[count].description, (const char*)desc); else posts[count].description[0] = '\0';
        if (img_path) strcpy(posts[count].image_path, (const char*)img_path); else posts[count].image_path[0] = '\0';

        count++;
    }
    *post_count = count;

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return 0;
}

int db_get_feed(int user_id, Post* posts, int max_posts, int* post_count)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, 
        "SELECT p.id, p.user_id, u.username, p.description, p.image_path, p.visibility, "
        "(SELECT COUNT(*) FROM likes l WHERE l.post_id = p.id) AS like_count, "
        "(SELECT COUNT(*) FROM comments c WHERE c.post_id = p.id) AS comment_count "
        "FROM posts p JOIN users u ON p.user_id = u.id "
        "WHERE p.user_id IN ("
        "   SELECT CASE WHEN sender_id = ? THEN receiver_id ELSE sender_id END AS friend_id "
        "   FROM friend_requests "
        "   WHERE (sender_id = ? OR receiver_id = ?) AND status = 1"
        ") "
        "ORDER BY p.created_at DESC", 
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_int(stmt, 3, user_id);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_posts) {
        posts[count].post_id = sqlite3_column_int(stmt, 0);
        posts[count].user_id = sqlite3_column_int(stmt, 1);
        const unsigned char* uname = sqlite3_column_text(stmt, 2);
        const unsigned char* desc = sqlite3_column_text(stmt, 3);
        const unsigned char* img_path = sqlite3_column_text(stmt, 4);
        posts[count].visibility = sqlite3_column_int(stmt, 5);
        posts[count].like_count = sqlite3_column_int(stmt, 6);
        posts[count].comment_count = sqlite3_column_int(stmt, 7);

        if (uname) strcpy(posts[count].username, (const char*)uname); else posts[count].username[0] = '\0';
        if (desc) strcpy(posts[count].description, (const char*)desc); else posts[count].description[0] = '\0';
        if (img_path) strcpy(posts[count].image_path, (const char*)img_path); else posts[count].image_path[0] = '\0';

        count++;
    }
    *post_count = count;

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return 0;
}

int db_get_post_likes(int post_id, int* user_ids, int max_likes, int* like_count)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, 
        "SELECT user_id FROM likes WHERE post_id = ?", 
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, post_id);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_likes) {
        user_ids[count++] = sqlite3_column_int(stmt, 0);
    }
    *like_count = count;

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return 0;
}

int db_get_post_comments(int post_id, int* user_ids, char comments[][256], int max_comments, int* comment_count)
{
    pthread_mutex_lock(&db_mutex);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_conn, 
        "SELECT user_id, content FROM comments WHERE post_id = ?", 
        -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, post_id);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max_comments) {
        user_ids[count] = sqlite3_column_int(stmt, 0);
        const unsigned char* comment_text = sqlite3_column_text(stmt, 1);
        if (comment_text) {
            strncpy(comments[count], (const char*)comment_text, 255);
            comments[count][255] = '\0'; 
        } else {
            comments[count][0] = '\0';
        }
        count++;
    }
    *comment_count = count;

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);
    return 0;
}