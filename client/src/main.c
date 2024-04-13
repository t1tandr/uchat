#include "uchat.h"

int servsock;

void login_button_click_cb(GtkWidget *self, gpointer user_data) {
    GtkBuilder* builder = GTK_BUILDER(user_data);
    cJSON* data = NULL;
    cJSON* response = NULL;
    const char* username = NULL;
    const char* password = NULL;

    username = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "username-entry")));
    password = gtk_editable_get_text(GTK_EDITABLE(gtk_builder_get_object(builder, "password-entry")));

    data = cJSON_CreateObject();

    cJSON_AddStringToObject(data, "username", username);
    cJSON_AddStringToObject(data, "password", password);

    response = send_request(servsock, create_request(METHOD_POST, "/login", data));

    if(response != NULL && cJSON_HasObjectItem(response, "status")) {
        GtkRevealer* revealer = GTK_REVEALER(gtk_builder_get_object(builder, "login-error-revealer"));
        int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if(status == 200) {
            gtk_revealer_set_reveal_child(revealer, FALSE);

            GtkWidget* homepage = GTK_WIDGET(gtk_builder_get_object(builder, "homepage"));
            GtkWidget* login_page = GTK_WIDGET(gtk_builder_get_object(builder, "login-page"));
            GtkWidget* window = GTK_WIDGET(gtk_widget_get_parent(login_page));

            gtk_widget_unparent(login_page);
            gtk_widget_set_parent(homepage, window);
        }
        else {
            gtk_revealer_set_reveal_child(revealer, TRUE);
        }
    }
    else {
        handle_error("[ERROR]: Receiving response from server");
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

    return status;
}

