#include "uchat.h"

void user_search_entry_started_cb(GtkSearchEntry* self, gpointer user_data) {
    const char* entry_value = NULL;
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    entry_value = gtk_editable_get_text(GTK_EDITABLE(self));

    if (strlen(mx_strtrim(entry_value)) > 0) {
        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();

        request = create_request(METHOD_GET, "/users", data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "GET /users");
        }

        response = g_async_queue_pop(uchat->responses);
        
        if (cJSON_HasObjectItem(response, "status")) {
            status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 200) {
                cJSON* users_arr = cJSON_GetObjectItemCaseSensitive(response, "data");
                int size = cJSON_GetArraySize(users_arr);
                
                for (int i = 0 ; i < size; i++) {
                    cJSON* user_json = cJSON_GetArrayItem(users_arr, i);
                    const char* username = cJSON_GetObjectItemCaseSensitive(user_json, "username")->valuestring;

                    if (strcmp(entry_value, username) == 0 && strcmp(entry_value, uchat->user->username) != 0) {
                        t_user* user = user_parse_from_json(user_json);

                        if (user != NULL) {
                            GtkListBox* list = GTK_LIST_BOX(gtk_builder_get_object(uchat->builder, "new-chat-members-list"));
                            GtkListBoxRow* row = NULL;
                            GtkWidget* user_box = NULL;
                            bool create = true;
                            guint j = 0;

                            while ((row = gtk_list_box_get_row_at_index(list, j++)) != NULL && create) {
                                user_box = gtk_list_box_row_get_child(row);
                                create = strcmp(username, uchat_user_box_get_username(UCHAT_USER_BOX(user_box))) != 0;
                            }

                            if (create) {
                                gtk_list_box_append(list, GTK_WIDGET(uchat_user_box_new(user)));
                            }
                        }
                    }
                }
            }
            
            cJSON_Delete(response);
        }
        else {
            handle_error(RESPONSE_ERROR, "GET /users");
        }
    }

    gtk_editable_set_text(GTK_EDITABLE(self), "");
}