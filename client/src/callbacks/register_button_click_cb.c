#include "uchat.h"

void register_button_click_cb(GtkButton* self, gpointer user_data) {
    t_uchat_app* uchat = (t_uchat_app *)g_object_get_data(user_data, "uchat");
    GtkRevealer* username_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "username-is-taken-revealer"));
    GtkRevealer* unmatch_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "password-unmatch-revealer"));
    const char* username = NULL;
    const char* name = NULL;
    const char* password = NULL;
    const char* confirm_password = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;
    cJSON* response = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-username-entry")));
    name = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-name-entry")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-password-entry")));
    confirm_password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-password-confirm-entry")));

    gtk_revealer_set_reveal_child(unmatch_revealer, strcmp(password, confirm_password) != 0);

    if(check_password_strength(password, uchat->builder)) {   
        headers = cJSON_CreateObject();   
        data = cJSON_CreateObject();

        cJSON_AddStringToObject(data, "username", username);
        cJSON_AddStringToObject(data, "name", name);
        cJSON_AddStringToObject(data, "password", password);

        response = send_request(uchat->servsock, create_request(METHOD_POST, "/users", data, headers));

        if(response != NULL && cJSON_HasObjectItem(response, "status")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if(status == 201) {
                gtk_revealer_set_reveal_child(username_revealer, FALSE);
                window_switch_child(uchat->builder, "login-page", "homepage");
            }
            else {
                gtk_revealer_set_reveal_child(username_revealer, TRUE);
            }
        }
        else {
            handle_error("[ERROR]: Receiving response from server");
        }
    }
}

