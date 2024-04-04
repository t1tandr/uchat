#include "server.h"

extern t_list *clients;

void send_response_message_all(int sock_fd, sqlite3 *db, cJSON *message, int status) {
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", status);

    if (message != NULL) {
        cJSON_AddItemToObject(res, "data", message);
    }

    char *res_str = cJSON_Print(res);

    int chat_id = cJSON_GetObjectItem(message, "chat_id")->valueint;
    cJSON *chat_members = get_chat_members_service(chat_id, db, sock_fd);

    for (int i = 0; i < cJSON_GetArraySize(chat_members); i++) {
        cJSON *chat_member = cJSON_GetArrayItem(chat_members, i);
        int chat_member_id = cJSON_GetObjectItem(chat_member, "user_id")->valueint;

        t_list *temp = clients;
        while (temp != NULL) {
            connection *conn = (connection *) temp->data;
            
            if (chat_member_id == conn->user_id) {
                send(conn->sock_fd, res_str, strlen(res_str), 0);
            }

            temp = temp->next;
        }
    }

    cJSON_Delete(res);
    cJSON_Delete(chat_members);
    cJSON_free(res_str);
}

