#include "uchat.h"

void login_button_click_cb(GtkWidget *self, gpointer user_data) {
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "login-error-revealer"));
    const char* username = NULL;
    const char* password = NULL;
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "login-username")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "login-password")));

    headers = cJSON_CreateObject();

    data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "username", username);
    cJSON_AddStringToObject(data, "password", password);

    request = create_request(METHOD_POST, "/login", data, headers);

    response = send_request(uchat->servsock, request);

    if (response == NULL) {
        handle_error("uchat: error \'POST /login\' request to server");
    }

    if (cJSON_HasObjectItem(response, "status")) {
        int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 200) {
            gtk_revealer_set_reveal_child(revealer, FALSE);
            uchat->user = get_current_user_from_json(cJSON_GetObjectItemCaseSensitive(response, "data"));

            if (uchat->user == NULL) {
                handle_error("uchat: error getting user data");
            }

            create_session_file("session.json", cJSON_GetObjectItemCaseSensitive(response, "data"));
            window_switch_child(uchat->builder, "login-page", "homepage");
        }
        else {
            gtk_revealer_set_reveal_child(revealer, TRUE);
        }
        
        cJSON_Delete(response);
    }
    else {
        handle_error("uchat: error \'POST /login\' response from server");
    }
}

