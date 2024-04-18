#include "uchat.h"

void user_search_entry_started_cb(GtkSearchEntry* self, gpointer user_data) {
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(user_data, "uchat");
    const char* username = gtk_editable_get_text(GTK_EDITABLE(self));
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    if (strlen(mx_strtrim(username)) > 0) {
        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();

        request = create_request(METHOD_GET, "/users", data, headers);

        response = send_request(uchat->servsock, request);

        cJSON_Delete(request);

        if (response != NULL && cJSON_HasObjectItem(response, "status") && cJSON_HasObjectItem(response, "data")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 200) {
                printf("%s", cJSON_Print(response));
            }

            cJSON_Delete(response);
        }
        else {
            cJSON_Delete(response);
            handle_error("uchat: error getting response from server");
        }
    }
}