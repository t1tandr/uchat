#include "uchat.h"

void login_button_click_cb(GtkWidget *self, gpointer user_data) {
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(user_data, "uchat");
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "login-error-revealer"));
    cJSON* data = NULL;
    cJSON* headers = NULL;
    cJSON* response = NULL;
    const char* username = NULL;
    const char* password = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "login-username-entry")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "login-password-entry")));

    headers = cJSON_CreateObject();  
    data = cJSON_CreateObject();

    cJSON_AddStringToObject(data, "username", username);
    cJSON_AddStringToObject(data, "password", password);

    response = send_request(uchat->servsock, create_request(METHOD_POST, "/login", data, headers));

    if(response != NULL && cJSON_HasObjectItem(response, "status")) {
        int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if(status == 200) {
            gtk_revealer_set_reveal_child(revealer, FALSE);
            window_switch_child(uchat->builder, "login-page", "homepage");
        }
        else {
            gtk_revealer_set_reveal_child(revealer, TRUE);
        }
    }
    else {
        handle_error("[ERROR]: Receiving response from server");
    }
}

