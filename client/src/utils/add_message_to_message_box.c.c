#include "uchat.h"

void add_message_to_message_box(t_message* message) {
    GtkNotebook* notebook = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    int n_pages = gtk_notebook_get_n_pages(notebook);

    for(int i = 1; i < n_pages; i++) {
        UchatMessageBox* page = UCHAT_MESSAGE_BOX(gtk_notebook_get_nth_page(notebook, i));
        
        if (message->chat_id == uchat_message_box_get_chat(page)->id) {
            uchat_message_box_add_message(page, message, message->user_id == uchat->user->id);
            return;
        }
    }
    // cJSON* request = NULL;
    // cJSON* response = NULL;
    // cJSON* data = NULL;
    // cJSON* headers = NULL;
    // const char* path = NULL;

    // headers = cJSON_CreateObject();
    // cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    // data = cJSON_CreateObject();
    // request = create_request(METHOD_GET, mx_strjoin("/messages/",mx_itoa(message->chat_id)), data, headers);

    // int status = send_request(uchat->servsock, request);

    // if (status != REQUEST_SUCCESS) {
    //     handle_error(REQUEST_ERROR, "\'GET /messages\'");
    // }
    // response = g_async_queue_pop(uchat->responses);

    // if (cJSON_HasObjectItem(response, "status")) {
    //     status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;
    //     mx_printint(80);
    //     if (status == 200) {
    //         for(int i = 0; i < cJSON_GetArraySize(response_data);i++){

    //         }
    //             cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
    //             unsigned long size;
    //             mx_printint(84);
    //             char* image = strdup(cJSON_GetObjectItemCaseSensitive(response_data, "content")->valuestring);
    //             unsigned char* from_bytes = g_base64_decode(image,&size);
    //             mx_printint(86);
    //             char* dir = mx_strjoin("storage/chat", mx_itoa(message->chat_id));
    //             int result = mkdir(dir, 0777);
    //             path = mx_strjoin(dir, mx_strjoin("/",mx_itoa(message->id)));
    //             mx_printint(91);
    //             bytes_to_file(from_bytes,size,path);
    //     }
    //     cJSON_Delete(response);
    // }
    // else {
    //     handle_error(RESPONSE_ERROR, "POST /messages");
    // }

    // UchatMessageBox* box = uchat_message_box_new(chat);
    // gtk_notebook_append_page(notebook, GTK_WIDGET(box), gtk_label_new(chat->name));
    // gtk_notebook_set_current_page(notebook, -1);
    // uchat->user->current_chat = chat;
}

