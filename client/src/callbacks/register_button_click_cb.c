#include "uchat.h"

void register_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkRevealer* username_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "username-is-taken-revealer"));
    GtkRevealer* unmatch_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "password-unmatch-revealer"));
    const char* username = NULL;
    const char* name = NULL;
    const char* password = NULL;
    const char* confirm_password = NULL;
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-username")));
    name = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-name")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-password")));
    confirm_password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "register-password-confirm")));

    gtk_revealer_set_reveal_child(unmatch_revealer, strcmp(password, confirm_password) != 0);

    if(check_password_strength(password, uchat->builder)) {   
        headers = cJSON_CreateObject();

        data = cJSON_CreateObject();
        cJSON_AddStringToObject(data, "username", username);
        cJSON_AddStringToObject(data, "name", name);
        cJSON_AddStringToObject(data, "password", password);

        request = create_request(METHOD_POST, "/users", data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "POST /users");
        }

        response = g_async_queue_pop(uchat->responses);

        if (response != NULL && cJSON_HasObjectItem(response, "status")) {
            status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                gtk_revealer_set_reveal_child(username_revealer, FALSE);
                
                GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "after-login"));
                gtk_window_set_transient_for(GTK_WINDOW(dialog), gtk_application_get_active_window(uchat->app));
                gtk_widget_show(dialog);
            }
            else {
                gtk_revealer_set_reveal_child(username_revealer, TRUE);
            }
            
            cJSON_Delete(response);
        }
        else {
            handle_error(RESPONSE_ERROR, "POST /users");
        }
    }
}

