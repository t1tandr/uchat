#include "server.h"

extern GHashTable *clients;

void handle_routes(cJSON *req, sqlite3 *db, int sock_fd) {
    if (!cJSON_HasObjectItem(req, "method")
        || !cJSON_HasObjectItem(req, "route")
        || !cJSON_HasObjectItem(req, "headers")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    // TEMP, think of a better solution
    client_connection_handler(req, db, sock_fd);

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init (&iter, clients);
    while (g_hash_table_iter_next (&iter, &key, &value)) {
        connection_data *conn = (connection_data *) value;

        printf("%d %s\n", conn->sock_fd, (char *) key);
    }
    // --------------------------------

    char *route = cJSON_GetObjectItemCaseSensitive(req, "route")->valuestring;
    char *method = cJSON_GetObjectItemCaseSensitive(req, "method")->valuestring;

    query_params_handler(req, route);

    if (strncmp(route, "/users", strlen("/users")) == 0) {
        int id;

        if (strcmp(method, "POST") == 0) {
            create_user_controller(req, db, sock_fd);
        } else if (strcmp(method, "GET") == 0 && sscanf(route, "/users/%d", &id) == 1) {
            get_user_controller(id, db, sock_fd);
        } else if (strcmp(method, "GET") == 0) {
            get_users_controller(db, sock_fd);
        } else if (strcmp(method, "PUT") == 0 && sscanf(route, "/users/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            update_user_controller(id, req, db, sock_fd);
        } else if (strcmp(method, "DELETE") == 0 && sscanf(route, "/users/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            delete_user_controller(id, req, db, sock_fd);
        }
    } else if (strncmp(route, "/login", strlen("/login")) == 0) {
        if (strcmp(method, "POST") == 0) {
            login_controller(req, db, sock_fd);
        }
    } else if (strncmp(route, "/logout", strlen("/logout")) == 0) {
        if (strcmp(method, "POST") == 0) {
            if (auth_handler(req, db, sock_fd) == false) return;
            logout_controller(req, db, sock_fd);
        }
    } else if (strncmp(route, "/chats", strlen("/chats")) == 0) {
        int id;

        if (strcmp(method, "POST") == 0) {
            if (auth_handler(req, db, sock_fd) == false) return;
            create_chat_controller(req, db, sock_fd);
        } else if (strcmp(method, "GET") == 0) {
            if (auth_handler(req, db, sock_fd) == false) return;
            get_chats_controller(req, db, sock_fd);
        } else if (strcmp(method, "PUT") == 0 && sscanf(route, "/chats/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            update_chat_controller(id, req, db, sock_fd);
        } else if (strcmp(method, "DELETE") == 0 && sscanf(route, "/chats/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            delete_chat_controller(id, req, db, sock_fd);
        }
    } else if (strncmp(route, "/chat-members", strlen("/chat-members")) == 0) {
        int id;

        if (strcmp(method, "POST") == 0) {
            if (auth_handler(req, db, sock_fd) == false) return;
            create_chat_member_controller(req, db, sock_fd);
        } else if (strcmp(method, "GET") == 0 && sscanf(route, "/chat-members/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            get_chat_members_controller(id, req, db, sock_fd);
        } else if (strcmp(method, "PUT") == 0 && sscanf(route, "/chat-members/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            update_chat_member_controller(id, req, db, sock_fd);
        } else if (strcmp(method, "DELETE") == 0 && sscanf(route, "/chat-members/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            delete_chat_member_controller(id, req, db, sock_fd);
        }
    } else if (strncmp(route, "/messages", strlen("/messages")) == 0) {
        int id;

        if (strcmp(method, "POST") == 0) {
            if (auth_handler(req, db, sock_fd) == false) return;
            create_message_controller(req, db, sock_fd);
        } else if (strcmp(method, "GET") == 0 && sscanf(route, "/messages/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            get_messages_controller(id, req, db, sock_fd);
        } else if (strcmp(method, "PUT") == 0 && sscanf(route, "/messages/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            update_message_controller(id, req, db, sock_fd);
        } else if (strcmp(method, "DELETE") == 0 && sscanf(route, "/messages/%d", &id) == 1) {
            if (auth_handler(req, db, sock_fd) == false) return;
            delete_message_controller(id, req, db, sock_fd);
        }
    } else {
        error_handler(sock_fd, "Unknown endpoint", 404);
    }
}

