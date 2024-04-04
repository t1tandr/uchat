#include "server.h"

bool is_file_exists(char *filename) {
    return access(filename, F_OK) == 0;
}

cJSON *stmt_to_user_json(sqlite3_stmt *stmt) {
    cJSON *user = cJSON_CreateObject();

    cJSON_AddNumberToObject(user, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(user, "username", (const char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(user, "name", (const char *) sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(user, "bio", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 4));
    cJSON_AddStringToObject(user, "created_at", (const char *) sqlite3_column_text(stmt, 5));

    return user;
}

cJSON *stmt_to_message_json(sqlite3_stmt *stmt) {
    cJSON *message = cJSON_CreateObject();

    cJSON_AddNumberToObject(message, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddNumberToObject(message, "chat_id", sqlite3_column_int(stmt, 1));
    cJSON_AddNumberToObject(message, "user_id", sqlite3_column_int(stmt, 2));
    cJSON_AddStringToObject(message, "text", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(message, "created_at", (const char *) sqlite3_column_text(stmt, 4));
    cJSON_AddStringToObject(message, "updated_at", (const char *) sqlite3_column_text(stmt, 5));

    return message;
}

cJSON *stmt_to_chat_json(sqlite3_stmt *stmt) {
    cJSON *chat = cJSON_CreateObject();

    cJSON_AddNumberToObject(chat, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(chat, "name", (const char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(chat, "img", (const char *) sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(chat, "link", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(chat, "created_at", (const char *) sqlite3_column_text(stmt, 4));

    return chat;
}

bool is_user_chat_member(int user_id, cJSON *chat_members) {
    for (int i = 0; i < cJSON_GetArraySize(chat_members); i++) {
        cJSON *chat_member = cJSON_GetArrayItem(chat_members, i);
        int chat_member_id = cJSON_GetObjectItem(chat_member, "user_id")->valueint;

        if (chat_member_id == user_id) {
            return true;
        }
    }

    return false;
}

