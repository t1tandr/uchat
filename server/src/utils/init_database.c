#include "server.h"
#include <string.h>

void init_database() {
    if (is_file_exists("uchat.db")) return;

    sqlite3 *db;
    char *error_message;
    int rc;

    db = database_connect();

    char *sql =
        "CREATE TABLE users ("
        "id INTEGER PRIMARY KEY NOT NULL,"
        "username VARCHAR(30) NOT NULL UNIQUE,"
        "name VARCHAR(30) NOT NULL,"
        "avatar VARCHAR(100),"
        "bio VARCHAR(200),"
        "password TEXT NOT NULL,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ");"
        "CREATE TABLE sessions ("
        "id TEXT PRIMARY KEY NOT NULL,"
        "user_id INTEGER NOT NULL,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "expires_at TIMESTAMP NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"
        "CREATE TABLE chats ("
        "id INTEGER PRIMARY KEY NOT NULL,"
        "name VARCHAR(30) NOT NULL,"
        "img TEXT,"
        "link VARCHAR(100) UNIQUE,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ");"
        "CREATE TABLE chat_members ("
        "id INTEGER PRIMARY KEY NOT NULL,"
        "chat_id INTEGER NOT NULL,"
        "user_id INTEGER NOT NULL,"
        "role VARCHAR(30) NOT NULL CHECK(role IN ('USER', 'ADMIN')) DEFAULT USER,"
        "FOREIGN KEY (chat_id) REFERENCES chats(id) ON DELETE CASCADE,"
        "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");"
        "CREATE TABLE messages ("
        "id INTEGER PRIMARY KEY NOT NULL,"
        "chat_id INTEGER NOT NULL,"
        "user_id INTEGER NOT NULL,"
        "type VARCHAR(30) NOT NULL CHECK(type IN ('text', 'photo', 'video', 'audio')) DEFAULT text,"
        "content TEXT NOT NULL,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (chat_id) REFERENCES chats(id) ON DELETE CASCADE,"
        "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");";

    rc = sqlite3_exec(db, sql, NULL, 0, &error_message);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
    } else {
        fprintf(stdout, "Database initiated successfully\n");
    }

    sqlite3_close(db);
}

