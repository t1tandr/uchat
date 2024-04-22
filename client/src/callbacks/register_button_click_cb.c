#include "uchat.h"

static void register_dialog_response_cb(GtkDialog* self, int response_id, gpointer user_data) {
    GtkRevealer* login_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "login-revealer"));
    GtkRevealer* register_revealer = GTK_REVEALER(gtk_builder_get_object(uchat->builder, "register-revealer"));

    gtk_window_close(GTK_WINDOW(self));

    if(gtk_revealer_get_child_revealed(login_revealer)) {
        login_form_clear(uchat->builder);

        gtk_revealer_set_reveal_child(register_revealer, TRUE);
        gtk_revealer_set_reveal_child(login_revealer, FALSE);
    }
    else {
        register_form_clear(uchat->builder);

        gtk_revealer_set_reveal_child(register_revealer, FALSE);
        gtk_revealer_set_reveal_child(login_revealer, TRUE);
    }
}

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

        response = send_request(uchat->servsock, request);

        if (response != NULL && cJSON_HasObjectItem(response, "status")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                gtk_revealer_set_reveal_child(username_revealer, FALSE);

                GtkWidget* dialog = gtk_dialog_new_with_buttons("Registration successful",
                    gtk_application_get_active_window(uchat->app),
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    "Go to login",
                    GTK_RESPONSE_OK,
                    NULL
                );
                gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 225);
                gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
                
                
                GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                GtkWidget* button = gtk_dialog_get_widget_for_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
                GtkWidget* label = gtk_label_new("Hello, pusie! Start chatting after log in!");
                
                gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
                gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
                gtk_box_append(GTK_BOX(content_area), label);

                g_signal_connect(dialog, "response", G_CALLBACK(register_dialog_response_cb), user_data);
                g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_window_destroy), NULL);

                gtk_widget_show(dialog);
            }
            else {
                gtk_revealer_set_reveal_child(username_revealer, TRUE);
            }
            
            cJSON_Delete(response);
        }
        else {
            handle_error("[ERROR]: Receiving response from server");
        }
    }
}

