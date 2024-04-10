#include "server.h"

extern t_list *clients;

void handle_routes(cJSON *req, sqlite3 *db, int sock_fd) {
    if (!cJSON_HasObjectItem(req, "method")
        || !cJSON_HasObjectItem(req, "route")
        || !cJSON_HasObjectItem(req, "headers")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    // TEMP, think of a better solution
    client_connection_handler(req, db, sock_fd);

    t_list *temp = clients;
    while (temp != NULL) {
        connection *conn = (connection *) temp->data;
        printf("%d %s\n", conn->sock_fd, conn->session_id);
        temp = temp->next;
    }
    // --------------------------------

    char *route = cJSON_GetObjectItemCaseSensitive(req, "route")->valuestring;
    char *method = cJSON_GetObjectItemCaseSensitive(req, "method")->valuestring;

    if (strncmp(route, "/users", strlen("/users")) == 0) {
        int id;

        if (strcmp(method, "POST") == 0) {
            create_user_controller(req, db, sock_fd);
        } else if (strcmp(method, "GET") == 0 && sscanf(route, "/users/%d", &id) == 1) {
            get_user_controller(id, db, sock_fd);
        } else if (strcmp(method, "GET") == 0) {
            get_users_controller(db, sock_fd);
        } else if (strcmp(method, "PUT") == 0 && sscanf(route, "/users/%d", &id) == 1) {
            update_user_controller(id, req, db, sock_fd);
        } else if (strcmp(method, "DELETE") == 0 && sscanf(route, "/users/%d", &id) == 1) {
            delete_user_controller(id, req, db, sock_fd);
        }
    } else if (strncmp(route, "/login", strlen("/login")) == 0) {
        if (strcmp(method, "POST") == 0) {
            login_controller(req, db, sock_fd);
        }
    } else if (strncmp(route, "/logout", strlen("/logout")) == 0) {
        if (strcmp(method, "POST") == 0) {
            logout_controller(req, db, sock_fd);
        }
    } else if (strncmp(route, "/chats", strlen("/chats")) == 0) {
        int id;

        if (strcmp(method, "POST") == 0) {
            create_chat_controller(req, db, sock_fd);
        } else if (strcmp(method, "GET") == 0) {
            get_chats_controller(req, db, sock_fd);
        } else if (strcmp(method, "PUT") == 0 && sscanf(route, "/chats/%d", &id) == 1) {
            update_chat_controller(id, req, db, sock_fd);
        } else if (strcmp(method, "DELETE") == 0 && sscanf(route, "/chats/%d", &id) == 1) {
            delete_chat_controller(id, req, db, sock_fd);
        }
    } else if (strncmp(route, "/chat-members", strlen("/chat-members")) == 0) {
        int id;

        if (strcmp(method, "POST") == 0) {
            create_chat_member_controller(req, db, sock_fd);
        } else if (strcmp(method, "GET") == 0 && sscanf(route, "/chat-members/%d", &id) == 1) {
            get_chat_members_controller(id, req, db, sock_fd);
        }
        // else if (strcmp(method, "PUT") == 0 && sscanf(route, "/chats/%d", &id) == 1) {
        //     update_chat_controller(id, req, db, sock_fd);
        // } else if (strcmp(method, "DELETE") == 0 && sscanf(route, "/chats/%d", &id) == 1) {
        //     delete_chat_controller(id, req, db, sock_fd);
        // }
    } else if (strncmp(route, "/message", strlen("/message")) == 0) {
        if (strcmp(method, "POST") == 0) {
            create_message_controller(req, db, sock_fd);
        }
    } else {
        error_handler(sock_fd, "Unknown endpoint", 404);
    }
}

