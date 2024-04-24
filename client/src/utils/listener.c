#include "uchat.h"

void handle_message_response(cJSON* json) {
    t_message* message = message_parse_from_json(json);

    for (t_list* i = uchat->user->chats; i != NULL; i = i->next) {
        t_chat* chat = (t_chat *)i->data;
        
        if (chat->id == message->chat_id) {
            chat->last_message = message;
            mx_push_back(&(chat->messages), message);
            break;
        }
    }

    GtkListBox* list = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "chat-list"));
    GtkListBoxRow* row = NULL;
    int index = 0;

    while ((row = gtk_list_box_get_row_at_index(list, index++)) != NULL) {
        UchatChatBox* chatbox = UCHAT_CHAT_BOX(gtk_list_box_row_get_child(row));
        t_chat* chat = uchat_chat_box_get_chat(chatbox);

        if (message->chat_id == chat->id) {
            uchat_chat_box_set_message(chatbox, message);
            gtk_list_box_invalidate_sort(list);
            break;
        }
    }

    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    int n_pages = gtk_notebook_get_n_pages(notebook);

    for(int i = 1; i < n_pages; i++) {
        GtkWidget* chat = gtk_notebook_get_nth_page(notebook, i);
        
        if (message->chat_id == uchat_message_box_get_chat(UCHAT_MESSAGE_BOX(chat))->id) {
            uchat_message_box_add_message(UCHAT_MESSAGE_BOX(chat), message, true);
            break;
        }
    }
}

void handle_chat_member_response(cJSON* json) {
    t_chat_member* member = chat_member_parse_from_json(json);
    GtkListBox* chat_list = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "chat-list"));

    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;
    char route[128];

    sprintf(route, "/chats/%d", member->chat_id);

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();

    request = create_request(METHOD_GET, "/chats", data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "GET /chats/{id}");
    }

    response = g_async_queue_pop(uchat->responses);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 200) {
            cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");

            t_chat* chat = chat_parse_from_json(response_data);
            mx_push_back(&(uchat->user->chats), chat);
                
            gtk_list_box_prepend(chat_list, GTK_WIDGET(uchat_chat_box_new(chat)));
        }

        cJSON_Delete(response);
    }
    else {
        handle_error(RESPONSE_ERROR, "GET /chats/{id}");
    }
}

static void handle_response(cJSON* response) {
    if (response == NULL
        || !cJSON_HasObjectItem(response, "status") 
        || !cJSON_HasObjectItem(response, "data")) {
            handle_error(RESPONSE_ERROR, "socket");
    }
    cJSON* data = cJSON_GetObjectItemCaseSensitive(response, "data");
    
    if (cJSON_HasObjectItem(data, "type") && cJSON_HasObjectItem(data, "content")) {
        handle_message_response(data);
    }
    else if (cJSON_HasObjectItem(data, "role")) {
        handle_chat_member_response(data);
    }
}

void* listen_for_response(void* arg) {
    while (true) {
        cJSON* response = recv_response(uchat->servsock);

        if (response != NULL) {
            if (cJSON_HasObjectItem(response, "type")) {
                char* type = cJSON_GetObjectItemCaseSensitive(response, "type")->valuestring;

                if (strcmp(type, "regular") == 0) {
                    g_async_queue_push(uchat->responses, response);
                }
                if (strcmp(type, "socket") == 0) {
                    handle_response(response);
                }
            }
        }
    }

    pthread_exit(NULL);
}

void init_listener_thread() {
    int status = 0;
    pthread_t id;

    status = pthread_create(&id, NULL, listen_for_response, NULL);
    if (status != 0) {
        handle_error(PTHREAD_ERROR, strerror(errno));
    }

    status = pthread_detach(id);
    if (status != 0) {
        handle_error(PTHREAD_ERROR, strerror(errno));
    }
}

