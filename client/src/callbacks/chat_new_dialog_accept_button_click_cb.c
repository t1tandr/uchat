#include "uchat.h"

void add_members_to_chat(t_chat* chat, t_uchat_app* uchat) {
    for (t_list* i = chat->members; i != NULL; i = i->next) {
        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;
        t_user* member = (t_user *)i->data;

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "chat_id", chat->id);
        cJSON_AddNumberToObject(data, "user_id", member->id);

        request = create_request(METHOD_POST, "/chat-members", data, headers);

        response = send_request(uchat->servsock, request);

        cJSON_Delete(request);

        if (response != NULL && cJSON_HasObjectItem(response, "status")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status != 201) {
                handle_error("uchat: error creating chat");
            }
            cJSON_Delete(response);
        }
        else {
            handle_error("uchat: error \'POST /chat-members\' response from server");
        }
    }
}

void chat_new_dialog_accept_button_click_cb(GtkButton* self, gpointer user_data) {
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(user_data, "uchat");
    GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "chat-new-dialog"));
    GtkEntry* chatname_entry = GTK_ENTRY(gtk_builder_get_object(uchat->builder, "chat-new-name-entry"));
    const char* chatname = gtk_editable_get_text(GTK_EDITABLE(chatname_entry));
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "name", chatname);

    request = create_request(METHOD_POST, "/chats", data, headers);

    response = send_request(uchat->servsock, request);

    cJSON_Delete(request);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 201) {
            GtkListBox* list = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "new-chat-members-list"));
            GtkListBoxRow* row = NULL;
            UchatUserBox* user_box = NULL;
            guint index = 0;
            t_chat* chat = NULL;

            chat = get_chat_from_json(cJSON_GetObjectItemCaseSensitive(response, "data"));

            mx_push_back(&chat->members, uchat->user);

            while ((row = gtk_list_box_get_row_at_index(list, index++)) != NULL) {
                user_box = UCHAT_USER_BOX(gtk_list_box_row_get_child(row));
                mx_push_back(&chat->members, uchat_user_box_get_user(user_box));
            }

            if(mx_list_size(chat->members) > 1) {
                add_members_to_chat(chat, uchat);
            }
        }
        cJSON_Delete(response);
    }
    else {
        handle_error("uchat: error \'POST /chats\' response from server");
    }

    gtk_widget_hide(dialog);
}

