#include "uchat.h"


void on_button_in_after_save_window_clicked_cb(GtkButton* self, gpointer user_data){
    GtkBox* homepage = GTK_BOX(gtk_builder_get_object(uchat->builder, "homepage"));
    GtkNotebook* right_side = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    GtkScrolledWindow* settings = GTK_SCROLLED_WINDOW(gtk_builder_get_object(uchat->builder, "settings"));
    GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "after-save"));

    gtk_box_remove(homepage, GTK_WIDGET(settings));
    gtk_box_append(homepage, GTK_WIDGET(right_side));
    gtk_widget_hide(dialog);
}

char *get_text_from_text_view(GtkTextView *text_view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    return text;
}

void save_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkRevealer* username_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "username-is-taken-revealer-in-settings"));
    GtkRevealer* unmatch_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "password-unmatch-revealer-in-settings"));
    const char* username = NULL;
    const char* name = NULL;
    const char* password = NULL;
    const char* confirm_password = NULL;
    const char* biography = NULL;
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-username")));
    name = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-name")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-password")));
    confirm_password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(uchat->builder, "settings-get-password-confrim")));
    biography = get_text_from_text_view(GTK_TEXT_VIEW(gtk_builder_get_object(uchat->builder, "settings-get-biography")));

    gtk_revealer_set_reveal_child(unmatch_revealer, strcmp(password, confirm_password) != 0);

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();
    if(strlen(username) > 0){
        cJSON_AddStringToObject(data, "username", username);
    }
    if(strlen(name) > 0){
        cJSON_AddStringToObject(data, "name", name);
    }
    if(strlen(biography) > 0){
        cJSON_AddStringToObject(data, "bio", biography);
    }
    if(strlen(password) > 0){
        cJSON_AddStringToObject(data, "password", password);
    }   

    if(data != NULL){
        request = create_request(METHOD_PUT, mx_strjoin("/users/", mx_itoa(uchat->user->id)), data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "PUT /users/{id}");
        }

        response = g_async_queue_pop(uchat->responses);

        if (response != NULL && cJSON_HasObjectItem(response, "status")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                gtk_revealer_set_reveal_child(username_revealer, FALSE);
                // uchat->user = get_current_user_from_json(cJSON_GetObjectItemCaseSensitive(response, "data"));

                // create_session_file("session.json", cJSON_GetObjectItemCaseSensitive(response, "data"));
                uchat->user->bio = biography;
                GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "after-save"));
                gtk_window_set_transient_for(GTK_WINDOW(dialog), gtk_application_get_active_window(uchat->app));
                gtk_widget_show(dialog);
            }
            else{
                gtk_revealer_set_reveal_child(username_revealer, TRUE);
            }
            
            cJSON_Delete(response);
        }
        else {
            handle_error(RESPONSE_ERROR, "PUT /users/{id}");
        }
    }
}

