#include "server.h"

extern t_list *clients;

void send_response_message_all(cJSON *headers, cJSON *message, int status, int sock_fd, sqlite3 *db) {
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", status);

    if (message != NULL) {
        cJSON_AddItemToObject(res, "data", message);
    }

    char *res_str = cJSON_Print(res);
    int length = strlen(res_str);

    int chat_id = cJSON_GetObjectItem(message, "chat_id")->valueint;
    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);

    for (int i = 0; i < cJSON_GetArraySize(chat_members); i++) {
        cJSON *chat_member = cJSON_GetArrayItem(chat_members, i);
        int chat_member_id = cJSON_GetObjectItem(chat_member, "user_id")->valueint;

        t_list *temp = clients;
        while (temp != NULL) {
            connection *conn = (connection *) temp->data;
            
            if (chat_member_id == conn->user_id) {
                int nbytes = send(conn->sock_fd, &length, sizeof(length), MSG_NOSIGNAL);
    
                if(nbytes > -1) {
                    send(conn->sock_fd, res_str, length, MSG_NOSIGNAL);
                }
            }
            temp = temp->next;
        }
    }

    cJSON_Delete(res);
    cJSON_Delete(chat_members);
    cJSON_free(res_str);
}

