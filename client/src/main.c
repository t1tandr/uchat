#include "uchat.h"

int servsock;

static void window_switch_child(GtkBuilder* builder, char* prev_id, char* next_id) {
    GtkWidget* prev = GTK_WIDGET(gtk_builder_get_object(builder, prev_id));
    GtkWidget* next = GTK_WIDGET(gtk_builder_get_object(builder, next_id));
    GtkWindow* window = GTK_WINDOW(gtk_widget_get_parent(prev));

    gtk_widget_unparent(prev);
    gtk_window_set_child(window, next);
}

void login_button_click_cb(GtkWidget *self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(builder, "login-error-revealer"));
    cJSON* data = NULL;
    cJSON* headers = NULL;
    cJSON* response = NULL;
    const char* username = NULL;
    const char* password = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "login-username-entry")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "login-password-entry")));

    headers = cJSON_CreateObject();  
    data = cJSON_CreateObject();

    cJSON_AddStringToObject(data, "username", username);
    cJSON_AddStringToObject(data, "password", password);

    response = send_request(servsock, create_request(METHOD_POST, "/login", data, headers));

    if(response != NULL && cJSON_HasObjectItem(response, "status")) {
        int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if(status == 200) {
            gtk_revealer_set_reveal_child(revealer, FALSE);
            window_switch_child(builder, "login-page", "homepage");
        }
        else {
            gtk_revealer_set_reveal_child(revealer, TRUE);
        }
    }
    else {
        handle_error("[ERROR]: Receiving response from server");
    }
}

void register_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    GtkRevealer* username_revealer = GTK_REVEALER(gtk_builder_get_object(builder, "username-is-taken-revealer"));
    GtkRevealer* unmatch_revealer = GTK_REVEALER(gtk_builder_get_object(builder, "password-unmatch-revealer"));
    const char* username = NULL;
    const char* name = NULL;
    const char* password = NULL;
    const char* confirm_password = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;
    cJSON* response = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "register-username-entry")));
    name = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "register-name-entry")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "register-password-entry")));
    confirm_password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "register-password-confirm-entry")));

    gtk_revealer_set_reveal_child(unmatch_revealer, strcmp(password, confirm_password) != 0);
    
    if(check_password_strength(password, builder)) {   
        headers = cJSON_CreateObject();   
        data = cJSON_CreateObject();

        cJSON_AddStringToObject(data, "username", username);
        cJSON_AddStringToObject(data, "name", name);
        cJSON_AddStringToObject(data, "password", password);

        response = send_request(servsock, create_request(METHOD_POST, "/users", data, headers));

        if(response != NULL && cJSON_HasObjectItem(response, "status")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if(status == 201) {
                gtk_revealer_set_reveal_child(username_revealer, FALSE);
                window_switch_child(builder, "login-page", "homepage");
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

int main(int argc, char *argv[]) {
    if(argc != 3) {
        handle_error(USAGE_ERROR);
    }
    GtkApplication* app = NULL;
    int status = 0;

    servsock = connect_to_server(argv[1], argv[2]);

    app = gtk_application_new("ua.ucode-connect.uchat", G_APPLICATION_HANDLES_OPEN);

    g_signal_connect(app, "open", G_CALLBACK(app_activate_cb), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    close(servsock);

    return status;
}

