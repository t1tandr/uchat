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
        if(cJSON_GetObjectItemCaseSensitive(response, "status")->valueint == 200) {
            printf("[CLIENT]: received message:\n%s\n", cJSON_Print(response));
        }
    }
    else {
        handle_error("uchat: invalid response JSON");
    }
}

/*

void password_entry_icon_press_cb(GtkEntry* self) {
    if(gtk_entry_get_visibility(self)) {
        gtk_entry_set_visibility(self, FALSE);
        gtk_entry_set_icon_from_icon_name(self, GTK_ENTRY_ICON_SECONDARY, "password-visibility-on");
    }
    else {
        gtk_entry_set_visibility(self, TRUE);
        gtk_entry_set_icon_from_icon_name(self, GTK_ENTRY_ICON_SECONDARY, "password-visibility-off");
    }
}

*/

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

