#include "uchat.h"

static void add_members_to_chat(t_chat* chat, t_uchat* uchat) {
    for (t_list* i = chat->members; i != NULL; i = i->next) {
        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;
        t_user* user = (t_user *)i->data;

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "chat_id", chat->id);
        cJSON_AddNumberToObject(data, "user_id", user->id);

        request = create_request(METHOD_POST, "/chat-members", data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "POST /messages");
        }

        response = g_async_queue_pop(uchat->responses);
        
        if (cJSON_HasObjectItem(response, "status")) {
            status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
                t_chat_member* member = chat_member_parse_from_json(response_data);

                mx_push_back(&(chat->members), member);
            }

            cJSON_Delete(response);
        }
        else {
            handle_error(RESPONSE_ERROR, "POST /messages");
        }
    }
}

void chat_new_dialog_accept_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "chat-new-dialog"));
    GtkEntry* chatname_entry = GTK_ENTRY(gtk_builder_get_object(uchat->builder, "chat-new-name-entry"));
    UchatAvatarBox* avatar = UCHAT_AVATAR_BOX(gtk_builder_get_object(uchat->builder, "chat-new-avatar"));
    const char* chatname = gtk_editable_get_text(GTK_EDITABLE(chatname_entry));
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    char *filename = uchat_avatar_box_get_file(avatar);
    data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "name", chatname);
    if(uchat_avatar_box_get_file(avatar) != NULL){
        cJSON_AddStringToObject(data, "img", filename);
    }

    request = create_request(METHOD_POST, "/chats", data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "POST /chats");
    }

    response = g_async_queue_pop(uchat->responses);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 201) {
            GtkListBox* list = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "new-chat-members-list"));
            GtkListBoxRow* row = NULL;
            UchatUserBox* user_box = NULL;
            guint index = 0;
            t_chat* chat = NULL;
            t_list* members = NULL;

            chat = chat_parse_from_json(cJSON_GetObjectItemCaseSensitive(response, "data"));

            while ((row = gtk_list_box_get_row_at_index(list, index++)) != NULL) {
                user_box = UCHAT_USER_BOX(gtk_list_box_row_get_child(row));
                mx_push_back(&members, uchat_user_box_get_user(user_box));
            }

            if(mx_list_size(members) > 0) {
                GtkListBox* chat_list = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "chat-list"));
                add_members_to_chat(chat, uchat);

                gtk_list_box_prepend(chat_list, GTK_WIDGET(uchat_chat_box_new(chat)));
                mx_push_back(&(uchat->user->chats), chat);
            }
        }

        cJSON_Delete(response);
    }
    else {
        handle_error(RESPONSE_ERROR, "POST /chats");
    }

    gtk_widget_hide(dialog);
}

