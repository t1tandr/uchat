#include "server.h"

bool is_file_exists(char *filename) {
    return access(filename, F_OK) == 0;
}

void create_dir_if_not_exists(char *path) {
    struct stat st = {0};
    char *path_copy = mx_strdup(path);
    char *dir = path_copy;

    while ((dir = strchr(dir + 1, '/')) != NULL) {
        *dir = '\0';
        if (stat(path_copy, &st) == -1) {
            mkdir(path_copy, 0700);
        }
        *dir = '/';
    }

    if (stat(path_copy, &st) == -1) {
        mkdir(path_copy, 0700);
    }

    free(path_copy);
}

bool contains_space(char *string) {
    for (int i = 0; i < mx_strlen(string); i++) {
        if (mx_isspace(string[i])) {
            return true;
        }
    }

    return false;
}

cJSON *stmt_to_user_json(sqlite3_stmt *stmt) {
    cJSON *user = cJSON_CreateObject();

    cJSON_AddNumberToObject(user, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(user, "username", (const char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(user, "name", (const char *) sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(user, "avatar", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(user, "bio", (const char *) sqlite3_column_text(stmt, 4));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 5));
    cJSON_AddStringToObject(user, "created_at", (const char *) sqlite3_column_text(stmt, 6));

    return user;
}

cJSON *stmt_to_message_json(sqlite3_stmt *stmt) {
    cJSON *message = cJSON_CreateObject();

    cJSON_AddNumberToObject(message, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddNumberToObject(message, "chat_id", sqlite3_column_int(stmt, 1));
    cJSON_AddNumberToObject(message, "user_id", sqlite3_column_int(stmt, 2));
    cJSON_AddStringToObject(message, "type", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(message, "content", (const char *) sqlite3_column_text(stmt, 4));
    cJSON_AddStringToObject(message, "created_at", (const char *) sqlite3_column_text(stmt, 5));
    cJSON_AddStringToObject(message, "updated_at", (const char *) sqlite3_column_text(stmt, 6));

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

cJSON *stmt_to_chat_member_json(sqlite3_stmt *stmt) {
    cJSON *chat_member = cJSON_CreateObject();

    cJSON_AddNumberToObject(chat_member, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddNumberToObject(chat_member, "chat_id", sqlite3_column_int(stmt, 1));
    cJSON_AddNumberToObject(chat_member, "user_id", sqlite3_column_int(stmt, 2));
    cJSON_AddStringToObject(chat_member, "role", (const char *) sqlite3_column_text(stmt, 3));

    return chat_member;
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

GHashTable *chat_members_to_user_ids_set(cJSON *chat_members) {
    GHashTable *user_ids = g_hash_table_new(g_str_hash, g_str_equal);;

    for (int i = 0; i < cJSON_GetArraySize(chat_members); i++) {
        cJSON *chat_member = cJSON_GetArrayItem(chat_members, i);
        int chat_member_id = cJSON_GetObjectItem(chat_member, "user_id")->valueint;

        g_hash_table_add(user_ids, (void *) mx_itoa(chat_member_id));
    }

    return user_ids;
}

