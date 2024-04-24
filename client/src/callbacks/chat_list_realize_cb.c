#include "uchat.h"

static gboolean filter_func(GtkListBoxRow* row, gpointer user_data) {
    UchatChatBox* chat = UCHAT_CHAT_BOX(gtk_list_box_row_get_child(row));
    const char* text = gtk_editable_get_text(GTK_EDITABLE(user_data));

    return strncmp(uchat_chat_box_get_name(chat), text, strlen(text)) == 0;
}

static gboolean sort_func(GtkListBoxRow* row1, GtkListBoxRow* row2, gpointer user_data) {
    UchatChatBox* chatbox1 = UCHAT_CHAT_BOX(gtk_list_box_row_get_child(row1));
    UchatChatBox* chatbox2 = UCHAT_CHAT_BOX(gtk_list_box_row_get_child(row2));
    t_chat* chat1 = uchat_chat_box_get_chat(chatbox1);
    t_chat* chat2 = uchat_chat_box_get_chat(chatbox2);

    if (chat1->last_message == NULL && chat2->last_message == NULL) {
        return strcmp(chat1->created_at, chat2->created_at) < 0;
    }

    if (chat1->last_message == NULL) {
        return strcmp(chat1->created_at, chat2->last_message->time) < 0;
    }

    if (chat2->last_message == NULL) {
        return strcmp(chat1->last_message->time, chat2->created_at) < 0;
    }

    return strcmp(chat1->last_message->time, chat2->last_message->time) < 0;
}

// static void chat_get_messages(t_chat* chat) {
//     if (chat->messages == NULL) {
//         cJSON* request = NULL;
//         cJSON* response = NULL;
//         cJSON* data = NULL;
//         cJSON* headers = NULL;
//         char route[128];

//         sprintf(route, "/messages/%d", chat->id);

//         headers = cJSON_CreateObject();
//         cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

//         data = cJSON_CreateObject();

//         request = create_request(METHOD_GET, route, data, headers);

//         int status = send_request(uchat->servsock, request);

//         if (status != REQUEST_SUCCESS) {
//             handle_error(REQUEST_ERROR, "GET /messages/{id}");
//         }

//         response = g_async_queue_pop(uchat->responses);

//         if (response != NULL && cJSON_HasObjectItem(response, "status")) {
//             status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

//             if (status == 200) {
//                 cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");

//                 chat->messages = message_parse_from_json_arr(response_data);

//                 for (t_list* j = chat->members; j != NULL; j = j->next) {
//                     t_chat_member* member = (t_chat_member *)j->data;
//                     member->username = chat_member_get_username_by_id(member->user_id);
//                 }

//                 for (t_list* i = chat->messages; i != NULL; i = i->next) {
//                     t_message* message = (t_message *)i->data;

//                     for (t_list* j = chat->members; j != NULL; j = j->next) {
//                         t_chat_member* member = (t_chat_member *)j->data;
//                         if (message->user_id == member->user_id) {
//                             message->author = strdup(member->username);
//                             break;
//                         }
//                     }

//                     if (i->next == NULL) {
//                         chat->last_message = message;
//                     }
//                 }
//             }

//             cJSON_Delete(response);
//         }
//         else {
//             handle_error(RESPONSE_ERROR, "GET /messages/{id}");
//         }
//     }
// }

void chat_list_realize_cb(GtkListBox* self, gpointer user_data) {
    GObject* search_entry = gtk_builder_get_object(uchat->builder, "chat-search");

    gtk_list_box_set_filter_func(self, filter_func, search_entry, NULL);
    gtk_list_box_set_sort_func(self, sort_func, NULL, NULL);

    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();

    request = create_request(METHOD_GET, "/chats", data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "GET /chats");
    }

    response = g_async_queue_pop(uchat->responses);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 201) {
            cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");

            uchat->user->chats = chat_parse_from_json_arr(response_data);
            
            for (t_list* i = uchat->user->chats; i != NULL; i = i->next) {
                t_chat* chat = (t_chat *)i->data;
                UchatChatBox* box = uchat_chat_box_new(chat);

                chat_add_members(chat);

                gtk_list_box_append(self, GTK_WIDGET(box));
                gtk_list_box_invalidate_sort(self);
            }
        }

        cJSON_Delete(response);
    }
    else {
        handle_error(RESPONSE_ERROR, "GET /chats");
    }
}

